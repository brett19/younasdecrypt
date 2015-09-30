#include "winsock2.h"
#include "BotNetwork.hpp"
#include "HarmEmuNetwork.hpp"
#include "RoPackets.hpp"

//#define DEBUG_PRINT_PACKETS

namespace BotNetwork
{
	SOCKET g_Socket = INVALID_SOCKET;
	bool g_bZoneConnection = false;
	F_PacketHandler g_fRecvHandler = nullptr;
	std::map<unsigned short, unsigned short> g_rPacketLen;

	RoPacket g_xBuffer;
	unsigned int g_uiBufRead;
	unsigned int g_uiBufTotal;

	void Init( F_PacketHandler fRecvHandler )
	{
		WSADATA wsa;
		WSAStartup( MAKEWORD(2,2), &wsa );

		FILE* fh = fopen( "D:\\RoBotData\\recvpackets.txt", "rb" );
		unsigned int uiPakId = 0, uiPakSize = 0;
		while( !feof(fh) ) {
			fscanf( fh, "%04x %d", &uiPakId, &uiPakSize );
			g_rPacketLen[ uiPakId ] = uiPakSize;
		}
		fclose( fh );

		g_fRecvHandler = fRecvHandler;
		g_bZoneConnection = false;
	}

	int Connect( int iServerId, char* pcAddr, unsigned short unPort )
	{
		g_bZoneConnection = ( iServerId == 3 );

		HarmEmu::Network::ServerAddress xAddr;
		xAddr.uiPort = unPort;
		strcpy( xAddr.pcAddr, pcAddr );

		for( auto i = HarmEmu::Network::g_ConnectHandlers.begin(); i != HarmEmu::Network::g_ConnectHandlers.end(); ++i ) {
			(*i)( &xAddr );
		}

		// Connect
		g_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		struct sockaddr_in ain;
		hostent* remoteHost = gethostbyname( pcAddr );
		ain.sin_family		= AF_INET;
		ain.sin_addr.s_addr	= *((unsigned long*)remoteHost->h_addr_list[0]);
		ain.sin_port		= htons( unPort );
		
		if( connect( g_Socket, (SOCKADDR*)&ain, sizeof(ain) ) == SOCKET_ERROR )
			return 1;

		for( auto i = HarmEmu::Network::g_ConnectAfterHandlers.begin(); i != HarmEmu::Network::g_ConnectAfterHandlers.end(); ++i ) {
			(*i)( &xAddr );
		}

		g_uiBufRead = 0;
		g_uiBufTotal = 0;

		printf( "==== SOCKET CONNECTED ====\n" );
		return 0;
	}

	void Close( )
	{
		printf( "==== SOCKET CLOSED ====\n" );
		shutdown( g_Socket, SD_BOTH );
		closesocket( g_Socket );
		g_Socket = INVALID_SOCKET;
	}

	bool IsZoneConnection( )
	{
		return g_bZoneConnection;
	}

	void SendPacket( unsigned short unCmd, void* pData, unsigned int uiSize )
	{
		RoPacket xPacket;
		xPacket.unCmd = unCmd;

		unsigned int uiRealSize = _GetPacketSize( unCmd );
		if( uiRealSize == 0 ) {
			uiRealSize = uiSize + 4;
			*((unsigned short*)&xPacket.pcData[0]) = uiRealSize;
			memcpy( &xPacket.pcData[2], pData, uiSize );
		} else {
			memcpy( &xPacket.pcData[0], pData, uiSize );
		}

		_SendPacketData( &xPacket, uiRealSize, true );
	}

	int WriteBytesNoHandler( void* pcBuffer, unsigned int uiSize )
	{
		return send( g_Socket, (char*)pcBuffer, uiSize, 0 );
	}

	int WriteBytes( void* pcBuffer, unsigned int uiSize )
	{
		HarmEmu::Network::g_DataHandler( true, (unsigned char*)pcBuffer, uiSize );
		return WriteBytesNoHandler( pcBuffer, uiSize );
	}

	int RecvBytes( void* pcBuffer, unsigned int uiSize )
	{
		int readval = recv( g_Socket, (char*)pcBuffer, uiSize, 0 );
		if( readval > 0 && readval != INVALID_SOCKET ) {
			HarmEmu::Network::g_DataHandler( false, (unsigned char*)pcBuffer, readval );
		}
		return readval;
	}

	void _HandlePacketData( void* pData, unsigned int uiLen, bool bSizeInData );

	bool Poll( )
	{
		if( g_Socket == INVALID_SOCKET ) return false;

		fd_set fds;
		FD_ZERO( &fds );
		FD_SET( g_Socket, &fds );

		timeval t = { 0, 0 };
		int selval = select( g_Socket, &fds, nullptr, nullptr, &t );

		if( selval < 0 ) return true;
		if( !FD_ISSET(g_Socket,&fds) ) return true;


		unsigned int uiBytesNeeded = 0;
		if( g_uiBufTotal == 0 ) {
			if( g_uiBufRead < 2 ) {
				// Need Command
				uiBytesNeeded = 2 - g_uiBufRead;
			} else if( g_uiBufRead < 4 ) {
				// Need Size
				uiBytesNeeded = 4 - g_uiBufRead;
			}
		} else {
			uiBytesNeeded = g_uiBufTotal - g_uiBufRead;
		}

		char* pcBuffer = (char*)&g_xBuffer;
		int readval = RecvBytes( &pcBuffer[g_uiBufRead], uiBytesNeeded );
		if( readval == 0 || readval == SOCKET_ERROR ) {
			// Socket Closed
			Close( );
			return false;
		}

		g_uiBufRead += readval;

		if( g_uiBufRead < 2 ) {
			// Not enough bytes for command
			return false;
		}

		unsigned int uiPakBytes = _GetPacketSize( g_xBuffer.unCmd );
		if( uiPakBytes == 0 ) {
			if( g_uiBufRead < 4 ) {
				// Not enough bytes for size
				return false;
			}

			g_uiBufTotal = *((unsigned short*)&g_xBuffer.pcData[0]);
		} else {
			g_uiBufTotal = uiPakBytes;
		}

		if( g_uiBufTotal > g_uiBufRead ) {
			// Not enough data bytes yet
			return false;
		}

		// We null-terminate the packet, just in case...
		*((unsigned char*)&g_xBuffer.pcData[ g_uiBufTotal - (uiPakBytes==0?2:0) ]) = 0;

		_HandlePacketData( &g_xBuffer, g_uiBufTotal, uiPakBytes == 0 );

		g_uiBufRead = 0;
		g_uiBufTotal = 0;

		return true;
	}


	void _HandlePacketData( void* pData, unsigned int uiLen, bool bSizeInData )
	{
		RoPacket* pPacket = (RoPacket*)pData;

#ifdef DEBUG_PRINT_PACKETS
		if( bSizeInData ) {
			printf( "--- R>> [%04x] %d\n", pPacket->unCmd, uiLen - 4 );
		} else {
			printf( "--- R>> [%04x] %d\n", pPacket->unCmd, uiLen - 2 );
		}
#endif

		HarmEmu::Network::F_PacketHandler fHandler = HarmEmu::Network::g_rPacketHandlers[ pPacket->unCmd ];		
		if( fHandler ) {
			HarmEmu::Network::g_PacketChainHandler( false, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_PREHANDLER );

			if( fHandler( false, pPacket->unCmd, (unsigned char*)pData, uiLen ) ) {
				HarmEmu::Network::g_PacketChainHandler( false, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_POSTHANDLER );
			} else {
				HarmEmu::Network::g_PacketChainHandler( false, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_POSTHANDLER | HarmEmu::Network::CM_NOSEND );
				return;
			}
		} else {
			HarmEmu::Network::g_PacketChainHandler( false, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_NOHANDLER );
		}

		if( bSizeInData ) {
			g_fRecvHandler( pPacket->unCmd, &pPacket->pcData[2], g_uiBufTotal-4 );
		} else {
			g_fRecvHandler( pPacket->unCmd, &pPacket->pcData[0], g_uiBufTotal-2 );
		}
	}



	void _SetPacketSize( unsigned short unCmd, unsigned short unLen )
	{
		g_rPacketLen[ unCmd ] = unLen;
	}

	unsigned short _GetPacketSize( unsigned short unCmd )
	{
		return g_rPacketLen[ unCmd ];
	}

	void _SendPacketData( void* pData, unsigned int uiLen, bool bUseHandlers )
	{
		RoPacket* pPacket = (RoPacket*)pData;

#ifdef DEBUG_PRINT_PACKETS
		printf( "--- SX<< [%04x] %d\n", pPacket->unCmd, uiLen );
#endif

		if( bUseHandlers ) {
			HarmEmu::Network::F_PacketHandler fHandler = HarmEmu::Network::g_rPacketHandlers[ pPacket->unCmd ];		
			if( fHandler ) {
				HarmEmu::Network::g_PacketChainHandler( true, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_PREHANDLER );

				if( fHandler( true, pPacket->unCmd, (unsigned char*)pData, uiLen ) ) {
					HarmEmu::Network::g_PacketChainHandler( true, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_POSTHANDLER );
				} else {
					HarmEmu::Network::g_PacketChainHandler( true, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_POSTHANDLER | HarmEmu::Network::CM_NOSEND );
					return;
				}
			} else {
				HarmEmu::Network::g_PacketChainHandler( true, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_NOHANDLER );
			}

		} else {
			HarmEmu::Network::g_PacketChainHandler( true, pPacket->unCmd, (unsigned char*)pData, uiLen, HarmEmu::Network::CM_NOPROCHANDLERS );
		}

		WriteBytes( pData, uiLen );
	}

	void _SendPacketDataNoStreamHandler( void* pData, unsigned int uiLen )
	{
		WriteBytesNoHandler( pData, uiLen );
	}

}
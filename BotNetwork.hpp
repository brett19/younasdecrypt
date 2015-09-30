#ifndef BotNetwork_h__
#define BotNetwork_h__

#include <map>

namespace BotNetwork
{
	typedef void (*F_PacketHandler)( unsigned short unCmd, void* pcData, unsigned short unSize );

	void Init( F_PacketHandler fRecvHandler );

	bool IsZoneConnection( );
	int Connect( int iServerId, char* pcAddr, unsigned short unPort );
	void Close( );

	bool Poll( );

	int RecvBytes( void* pcBuffer, unsigned int uiSize );
	int WriteBytes( const void* pcBuffer, unsigned int uiSize );
	void SendPacket( unsigned short unCmd, void* pData, unsigned int uiSize );

	void _SetPacketSize( unsigned short unCmd, unsigned short unLen );
	unsigned short _GetPacketSize( unsigned short unCmd );
	void _SendPacketData( void* pData, unsigned int uiLen, bool bUseHandlers = true );
	void _SendPacketDataNoStreamHandler( void* pData, unsigned int uiLen );
}

#endif // BotNetwork_h__

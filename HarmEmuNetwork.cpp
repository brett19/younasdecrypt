#include "HarmEmuNetwork.hpp"
#include "HarmEmu_Base.hpp"
#include "BotNetwork.hpp"

namespace HarmEmu
{
	namespace Network
	{
		std::unordered_map< unsigned short, F_PacketHandler > g_rPacketHandlers;
		std::vector< F_StreamConnectHandler> g_ConnectHandlers;
		std::vector< F_StreamConnectAfterHandler > g_ConnectAfterHandlers;
		F_StreamDataHandler g_DataHandler;
		F_PacketChainHandler g_PacketChainHandler;

		void __cdecl __Generic( )
		{
			HarmEmu::LogMsg( "-::Network::__Generic( )\n" );
		}

		void __cdecl EnterStealthMode( )
		{
			HarmEmu::LogMsg( "-::Network::EnterStealthMode( )\n" );
			__asm{ int 3 }; // Not Implemented
		}

		void __cdecl RegisterPacketIDTranslator(F_PacketIDTranslator fHandler, int iUnknown )
		{
			HarmEmu::LogMsg( "-::Network::RegisterPacketIDTranslator( %08x, %08x )\n", fHandler, iUnknown );
			__asm{ int 3 }; // Not Implemented
		}

		void __cdecl SetPacketHandler( int iUnknown, void* pvUnknown )
		{
			HarmEmu::LogMsg( "-::Network::SetPacketHandler( %08x, %08x )\n", iUnknown, pvUnknown );
			__asm{ int 3 }; // Not Implemented
		}

		void* __cdecl GetPacketHandler( int iUnknown, bool bUnknown )
		{
			HarmEmu::LogMsg( "-::Network::GetPacketHandler( %08x, %s )\n", iUnknown, bUnknown?"true":"false" );
			__asm{ int 3 }; // Not Implemented
			return nullptr;
		}





		void __cdecl RegisterPacketChainHandler(F_PacketChainHandler fHandler,int iUnknown)
		{
			HarmEmu::LogMsg( "-::Network::RegisterPacketChainHandler( %08x, %08x )\n", fHandler, iUnknown );

			static bool bHandlerSet = false;
			if( bHandlerSet ) {
				throw new std::exception( "There is already a PacketChainHandler set!" );
			}
			bHandlerSet = true;

			g_PacketChainHandler = fHandler;
		}

		void __cdecl RegisterStreamConnectHandler(F_StreamConnectHandler fHandler,int iUnknown)
		{
			HarmEmu::LogMsg( "-::Network::RegisterStreamConnectHandler( %08x, %08x )\n", fHandler, iUnknown );

			g_ConnectHandlers.push_back( fHandler );
		}

		void __cdecl RegisterStreamConnectAfterHandler(F_StreamConnectAfterHandler fHandler, int iUnknown)
		{
			HarmEmu::LogMsg( "-::Network::RegisterStreamConnectAfterHandler( %08x, %08x )\n", fHandler, iUnknown );

			g_ConnectAfterHandlers.push_back( fHandler );
		}

		void __cdecl RegisterStreamDataHandler(F_StreamDataHandler fHandler, int iUnknown)
		{
			HarmEmu::LogMsg( "-::Network::RegisterStreamDataHandler( %08x, %08x )\n", fHandler, iUnknown );

			static bool bHandlerSet = false;
			if( bHandlerSet ) {
				throw new std::exception( "There is already a StreamDataHandler set!" );
			}
			bHandlerSet = true;

			g_DataHandler = fHandler;
		}

		void __cdecl RegisterPacket(unsigned short unCmd, F_PacketHandler fHandler, int iUnknown)
		{
			HarmEmu::LogMsg( "-::Network::RegisterPacket( %04x, %08x, %08x )\n", unCmd, fHandler, iUnknown );

			g_rPacketHandlers.insert( std::pair<unsigned short, F_PacketHandler>(unCmd, fHandler) );
		}

		void __cdecl SendPacketNoStreamHandler( unsigned char *pData, unsigned int uiLen )
		{
			HarmEmu::LogMsg( "-::Network::SendPacketNoStreamHandler( %08x, %08x )\n", pData, uiLen );

			BotNetwork::_SendPacketDataNoStreamHandler( pData, uiLen );
		}

		bool __cdecl IsZoneConnection( )
		{
			bool bZoneConnection = BotNetwork::IsZoneConnection( );

			//HarmEmu::LogMsg( "-::Network::IsZoneConnection( ) = %s\n", bZoneConnection?"true":"false" );

			return bZoneConnection;
		}

		void __cdecl SendPacket( unsigned char* pData, unsigned int uiLen, bool bUnk )
		{
			HarmEmu::LogMsg( "-::Network::SendPacket( %08x, %d, %s )\n", pData, uiLen, bUnk?"true":"false" );

			if( bUnk ) {
				__asm{ int 3 };
			}
			BotNetwork::_SendPacketData( pData, uiLen, false );
		}

		void __cdecl SetPacketSize( int iCmd, int iSize )
		{
			HarmEmu::LogMsg( "-::Network::SetPacketSize( %08x, %d )\n", iCmd, iSize );

			BotNetwork::_SetPacketSize( iCmd, iSize );
		}

		int __cdecl GetPacketSize( int iCmd )
		{
			HarmEmu::LogMsg( "-::Network::GetPacketSize( %08x )\n", iCmd );

			return BotNetwork::_GetPacketSize( iCmd );;
		}
	}
}
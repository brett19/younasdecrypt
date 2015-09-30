#ifndef HarmEmuNetwork_h__
#define HarmEmuNetwork_h__

#include "HarmModule.hpp"

namespace HarmEmu
{
	namespace Network
	{
		struct ServerAddress
		{
			char pcAddr[16];
			unsigned int uiPort;
			union {
				unsigned char ucAddr[4];
				unsigned int uiAddr;
			};
		};

		enum ChainMode
		{
			CM_PREHANDLER = 0x2,
			CM_POSTHANDLER = 0x4,
			CM_NOHANDLER = 0x8,
			CM_NOSEND = 0x10,
			CM_NOPROCHANDLERS = 0x20
		};

		typedef bool (__fastcall *F_StreamConnectHandler)(ServerAddress *);
		typedef bool (__fastcall *F_StreamConnectAfterHandler)(ServerAddress *);
		typedef void (__fastcall *F_StreamDataHandler)(bool bSending,unsigned char *pData,unsigned int uiLen);
		typedef bool (__fastcall *F_PacketHandler)(bool bSending,unsigned short unCmd,unsigned char * pData,unsigned int uiLen);
		typedef bool (__fastcall *F_PacketChainHandler)(bool bSending,unsigned short unCmd,unsigned char * pData,unsigned int uiLen,int iUnk);
		typedef bool (__fastcall *F_PacketIDTranslator)(bool,unsigned short*,bool*);

		extern std::unordered_map< unsigned short, F_PacketHandler > g_rPacketHandlers;
		extern std::vector< F_StreamConnectHandler> g_ConnectHandlers;
		extern std::vector< F_StreamConnectAfterHandler > g_ConnectAfterHandlers;
		extern F_StreamDataHandler g_DataHandler;
		extern F_PacketChainHandler g_PacketChainHandler;

		void __cdecl __Generic( );
		void __cdecl EnterStealthMode( );
		void __cdecl RegisterPacketIDTranslator(F_PacketIDTranslator fHandler, int iUnknown );
		void __cdecl SetPacketHandler( int iUnknown, void* pvUnknown );
		void* __cdecl GetPacketHandler( int iUnknown, bool bUnknown );

		void __cdecl RegisterPacketChainHandler(F_PacketChainHandler fHandler,int iUnknown);
		void __cdecl RegisterStreamConnectHandler(F_StreamConnectHandler fHandler,int iUnknown);
		void __cdecl RegisterStreamConnectAfterHandler(F_StreamConnectAfterHandler fHandler, int iUnknown);
		void __cdecl RegisterStreamDataHandler(F_StreamDataHandler fHandler, int iUnknown);
		void __cdecl RegisterPacket(unsigned short unCmd, F_PacketHandler fHandler, int iUnknown);
		void __cdecl SendPacketNoStreamHandler( unsigned char *pData, unsigned int uiLen );
		bool __cdecl IsZoneConnection( );
		void __cdecl SendPacket( unsigned char* pData, unsigned int uiLen, bool bUnk );
		void __cdecl SetPacketSize( int iCmd, int iSize );
		int __cdecl GetPacketSize( int iCmd );
	}
}

#endif // HarmEmuNetwork_h__
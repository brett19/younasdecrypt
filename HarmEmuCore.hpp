#ifndef HarmEmuCore_h__
#define HarmEmuCore_h__

#include "HarmEmu_Base.hpp"
#include "HarmModule.hpp"
#include "HarmEmuCoreAES.hpp"

namespace HarmEmu
{
	static const bool DEBUG_CORE = false;

	namespace Core
	{
		unsigned int g_uiHarmLookup[ 0x98 ] = { 0 };

		void Init( HarmModuleLinker* pLinker )
		{
			char* pcHarmLookupData[] = {
				"kernel32.dll", "VirtualProtect",
				"kernel32.dll", "LoadLibraryA",
				"user32.dll", "MessageBoxA",
				"kernel32.dll", "VirtualAlloc",
				"kernel32.dll", "GetProcAddress",
				"kernel32.dll", "GetConsoleScreenBufferInfo",
				"kernel32.dll", "SetConsoleScreenBufferSize",
				"kernel32.dll", "SetConsoleActiveScreenBuffer",
				"kernel32.dll", "SetConsoleTextAttribute",
				"kernel32.dll", "WriteFile",
				"kernel32.dll", "CloseHandle",
				"kernel32.dll", "CreateConsoleScreenBuffer",
				"kernel32.dll", "VirtualFree",
				"kernel32.dll", "AllocConsole",
				"kernel32.dll", "QueryPerformanceFrequency",
				"kernel32.dll", "QueryPerformanceCounter",
				"kernel32.dll", "Sleep",
				"user32.dll", "ShowWindow",
				"kernel32.dll", "GetConsoleWindow",
				"kernel32.dll", "SetConsoleTitleA",
				"kernel32.dll", "GetModuleHandleA",
				"ntdll.dll", "RtlInitializeCriticalSection",
				"ntdll.dll", "RtlEnterCriticalSection",
				"ntdll.dll", "RtlLeaveCriticalSection",
				"kernel32.dll", "WaitForSingleObject",
				"kernel32.dll", "CreateThread",
				"", "",
				"kernel32.dll", "ExitProcess",
				"kernel32.dll", "CreateFileA",
				"kernel32.dll", "ReadFile",
				"kernel32.dll", "CreateFileMappingA",
				"kernel32.dll", "MapViewOfFile",
				"kernel32.dll", "GetFileSize",
				"kernel32.dll", "UnmapViewOfFile",
				"", "",
				"", "",
				nullptr, nullptr
			};

			char** pcIter = pcHarmLookupData;
			for( int i = 2; ; ++i ) {
				char* pcModule = *pcIter++;
				char* pcFunction = *pcIter++;
				if( !pcModule || !pcFunction ) break;
				if( !*pcModule || !*pcFunction ) continue;

				HarmModule* pModule = pLinker->GetModule( pcModule );
				g_uiHarmLookup[i] = (unsigned int)pModule->GetProcAddr( pcFunction );
			}

			g_uiHarmLookup[0] = 0x00000003;
			g_uiHarmLookup[1] = 0x0000108D;
			g_uiHarmLookup[36] = 0xDEADBEEF;
			g_uiHarmLookup[37] = 0xDEADBEEF;
		}

		void* __cdecl MemAlloc( unsigned int uiSize )
		{
			void* pData = malloc( uiSize );
			if( DEBUG_CORE ) HarmEmu::LogMsg( "-::Core::MemAlloc( %08x ) = %08x\n", uiSize, pData );
			return pData;
		}

		void __cdecl MemFree( void* pObject )
		{
			if( DEBUG_CORE ) HarmEmu::LogMsg( "-::Core::MemFree( %08x )\n", pObject );
			free( pObject );
		}

		void* __cdecl InitModule( unsigned int uiKey1, unsigned int uiKey2 )
		{
			if( DEBUG_CORE ) HarmEmu::LogMsg( "-::Core::InitModule( %08x, %08x )\n", uiKey1, uiKey2 );
			return g_uiHarmLookup;
		}


		void __cdecl SeedRand( unsigned int uiSeed )
		{
			if( DEBUG_CORE ) HarmEmu::LogMsg( "-::Core::SeedRand( %08x )\n", uiSeed );
			srand( uiSeed );
		}
		unsigned int __cdecl Rand( )
		{
			unsigned int uiRand = rand( );
			if( DEBUG_CORE ) HarmEmu::LogMsg( "-::Core::Rand( ) = %08x\n", uiRand );
			return uiRand;
		}

		unsigned int __cdecl CalcCrc32( unsigned int uiStartVal, unsigned char* pData, unsigned int uiLen )
		{
			unsigned int uiCrc = ::GetCrc32( pData, uiLen, uiStartVal );
			if( DEBUG_CORE ) HarmEmu::LogMsg( "-::Core::CalcCrc32( %08x, [%08x]'%s', %d ) = %08x\n", uiStartVal, pData, pData, uiLen, uiCrc );
			return uiCrc;
		}








		void* __cdecl func_dd722478( )
		{
			HarmEmu::LogMsg( "-::Core::func_dd722478( )\n" );
			return (void*)4;
		}

		void* __cdecl func_4b5d70ec( )
		{
			HarmEmu::LogMsg( "-::Core::func_4b5d70ec( )\n" );
			return (void*)4;
		}

		void* __cdecl func_8ad8e3fe( )
		{
			HarmEmu::LogMsg( "-::Core::func_8ad8e3fe( )\n" );
			return (void*)4;
		}

		void* __cdecl func_d608a7a5( )
		{
			HarmEmu::LogMsg( "-::Core::func_d608a7a5( )\n" );
			return (void*)4;
		}

		void* __cdecl func_a0f5f9a1( )
		{
			HarmEmu::LogMsg( "-::Core::func_a0f5f9a1( )\n" );
			return (void*)4;
		}

		void* __cdecl func_96f27e11( )
		{
			HarmEmu::LogMsg( "-::Core::func_96f27e11( )\n" );
			return (void*)4;
		}

		void* __cdecl func_c910e53d( )
		{
			HarmEmu::LogMsg( "-::Core::func_c910e53d( )\n" );
			return (void*)4;
		}
	}
}

#endif // HarmEmuCore_h__

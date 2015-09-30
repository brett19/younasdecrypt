#ifndef HarmEmuTimer_h__
#define HarmEmuTimer_h__

#include "HarmEmu_Base.hpp"
#include "HarmModule.hpp"

namespace HarmEmu
{
	namespace Timer
	{
		struct TimerData
		{
			unsigned int iUnk[0x100];
		};

		typedef bool (__cdecl *F_TimerEvent)(TimerData *);

		int __cdecl Add( unsigned int uiUnk1, F_TimerEvent fHandler, int iUnk2, int iUnk3, __int64 iUnk4, void* pvParam )
		{
			HarmEmu::LogMsg( "-::Timer::Add( %08x, %08x, %d, %d, %d, %08x )\n", uiUnk1, fHandler, iUnk2, iUnk3, iUnk4, pvParam );
			return 0;
		}

		int __cdecl AddInterval( unsigned int uiUnk6, unsigned int uiUnk1, F_TimerEvent fHandler, int iUnk2, int iUnk3, __int64 iUnk4, void* pvParam )
		{
			HarmEmu::LogMsg( "-::Timer::AddInterval( %08x, %08x, %08x, %d, %d, %d, %08x )\n", uiUnk6, uiUnk1, fHandler, iUnk2, iUnk3, iUnk4, pvParam );

			TimerData xTimer;
			for( int i = 0; i < 4; i++ ) {
				fHandler( &xTimer );
			}

			return 0;
		}
	}

	namespace Tick
	{
		unsigned int __cdecl Get( )
		{
			unsigned int uiTick = GetTickCount( );
			HarmEmu::LogMsg( "-::Tick::Get( ) = %08x\n", uiTick );
			return uiTick;
		}
	}
}

#endif // HarmEmuTimer_h__
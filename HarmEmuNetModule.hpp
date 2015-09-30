#ifndef HarmEmuNetModule_h__
#define HarmEmuNetModule_h__

#include "HarmEmu_Base.hpp"
#include "HarmModule.hpp"

namespace HarmEmu
{
	namespace NetModule
	{
		struct Task
		{
		};

		typedef void (__fastcall *F_TaskCompletionCallback)(Task *);

		void __cdecl __Generic( )
		{
			HarmEmu::LogMsg( "-::NetModule::__Generic( )\n" );
		}

		void __cdecl RegisterTaskCompletionCallback( F_TaskCompletionCallback fHandler, int iUnknown )
		{
			HarmEmu::LogMsg( "-::NetModule::RegisterTaskCompletionCallback( %08x, %d )\n", fHandler, iUnknown );
		}
	}
}

#endif // HarmEmuNetModule_h__
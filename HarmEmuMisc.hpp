#ifndef HarmEmuMisc_h__
#define HarmEmuMisc_h__

#include "HarmEmu_Base.hpp"
#include "HarmModule.hpp"

namespace HarmEmu
{
	namespace CpuId {
		void __cdecl __Generic( )
		{
			HarmEmu::LogMsg( "-::CpuId::__Generic( )\n" );
			__asm{ int 3 }; // Not Implemented
		}
	}

	namespace ToolBlock { void __cdecl __Generic( ) { /*printf( "-::ToolBlock::__Generic( )\n" );*/ } }
	namespace GrfLib { void __cdecl __Generic( ) { printf( "-::GrfLib::__Generic( )\n" ); } }
	namespace License { void __cdecl __Generic( ) { printf( "-::License::__Generic( )\n" ); } }
}

#endif // HarmEmuMisc_h__
#ifndef HarmEmuHarmConfig_h__
#define HarmEmuHarmConfig_h__

#include "HarmEmu_Base.hpp"

namespace HarmEmu
{
	namespace HarmConfig
	{
		bool __cdecl ReadConfigBool( const char* pName, bool bDefault )
		{
			HarmEmu::LogMsg( "-::HarmConfig::ReadConfigBool( [%08x]'%s', %s )\n", pName, pName, bDefault?"true":"false" );
			return bDefault;
		}
	}
}

#endif // HarmEmuHarmConfig_h__
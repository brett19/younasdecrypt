#ifndef HarmModuleLinker_h__
#define HarmModuleLinker_h__

#include <string>
#include "crc32.h"

class HarmModule;
class HarmModuleWin32;
class HarmModuleReal;
class HarmModuleVirt;

class HarmModuleLinker
{
public:
	virtual HarmModuleWin32* LoadWin32Module( const std::string& sModuleName ) = 0;
	virtual HarmModule* GetModule( unsigned int uiKey ) = 0;
	HarmModule* GetModule( const std::string& sName ) { return GetModule( GetCrc32(sName) ); }
};

#endif // HarmModuleLinker_h__

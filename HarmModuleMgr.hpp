#ifndef HarmModuleMgr_h__
#define HarmModuleMgr_h__

#include "HarmModule.hpp"

class HarmModuleMgr : public HarmModuleLinker
{
public:
	virtual HarmModuleWin32* LoadWin32Module( const std::string& sModuleName )
	{
		HarmModuleWin32* pModule = new HarmModuleWin32( this );
		pModule->Init( sModuleName );
		AddModule( pModule );
		return pModule;
	}

	HarmModuleReal* LoadBundleModule( HarmModuleBundle* pBundle, const std::string& sModuleName )
	{
		unsigned int iBundleIdx = 0;
		HarmModuleData* pModuleData = nullptr;
		for( unsigned int i = 0; i < pBundle->GetCount(); i++ ) {
			HarmModuleData* pData = pBundle->Get( i );
			if( ((HARMPE_HEADER*)pData)->uiUid == GetCrc32(sModuleName) ) {
				iBundleIdx = i;
				pModuleData = pData;
				break;
			}
		}
		if( !pModuleData ) {
			throw new std::exception( "Error Getting Module Data" );
		}
		HarmModuleReal* pModule = new HarmModuleReal( this );
		pModule->Load( pModuleData );

		printf( "HarmModuleMgr :: Module[%d] '%s' loaded @ %08x\n", iBundleIdx, sModuleName.c_str(), pModule->GetRvaPtr(0) );

		AddModule( pModule );
		return pModule;
	}

	HarmModuleVirt* CreateVirtModule( const std::string& sModuleName )
	{
		HarmModuleVirt* pModule = new HarmModuleVirt( this, sModuleName );
		AddModule( pModule );
		return pModule;
	}

	virtual HarmModule* GetModule( unsigned int uiKey )
	{
		ModuleList::iterator i = mModules.find( uiKey );
		return i != mModules.end() ? i->second : 0;
	}
	HarmModule* GetModule( const std::string& sModuleName ) { return GetModule( GetCrc32(sModuleName) ); }

	void AddModule( HarmModule* pModule )
	{
		mModules[ pModule->GetUid() ] = pModule;
	}

	int LinkAll( )
	{
		for( ModuleList::iterator i = mModules.begin( ); i != mModules.end( ); ++i ) {
			(*i).second->Link( );
		}
		return 0;
	}

	int EnterAll( )
	{
		for( ModuleList::iterator i = mModules.begin( ); i != mModules.end( ); ++i ) {
			(*i).second->EntryPoint( );
		}
		return 0;
	}

protected:
	typedef std::unordered_map<unsigned int, HarmModule*> ModuleList;
	typedef std::pair<unsigned int, HarmModule*> ModuleEntry;

	ModuleList mModules;

};

#endif // HarmModuleMgr_h__

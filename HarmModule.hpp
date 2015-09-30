#ifndef HarmModule_h__
#define HarmModule_h__

#include <unordered_map>
#include "HarmModuleLinker.hpp"

typedef std::unordered_map< unsigned int, void* > FuncLookupList;
typedef std::pair< unsigned int, void* > FuncLookupEntry;

class HarmModule
{
public:
	HarmModule( HarmModuleLinker* pLinker )
		: m_pLinker(pLinker)
	{
	}

	virtual unsigned int GetUid( ) = 0;
	virtual int Link( ) = 0;
	virtual int EntryPoint( ) = 0;
	virtual void* GetRvaPtr( unsigned int uiAddr ) = 0;
	virtual void* GetProcAddr( unsigned int uiKey ) = 0;

	void* GetProcAddr( const std::string& sProcName ) { return GetProcAddr( GetCrc32(sProcName) ); }

protected:
	HarmModuleLinker* m_pLinker;

};

#endif // HarmModule_h__
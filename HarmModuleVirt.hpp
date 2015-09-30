#ifndef HarmModuleVirt_h__
#define HarmModuleVirt_h__

#include "HarmModule.hpp"

class HarmModuleVirt : public HarmModule
{
public:
	HarmModuleVirt( HarmModuleLinker* pLinker, const std::string& sModuleName )
		: HarmModule( pLinker )
	{
		Init( GetCrc32(sModuleName) );
	}

	int Init( unsigned int uiUid )
	{
		m_uiUid = uiUid;
		return 0;
	}

	template <typename T>
	int AddFunction( unsigned int uiKey, T fFunction )
	{
		mFuncList[ uiKey ] = *(void**)&fFunction;
		return 0;
	}

	template <typename T>
	int AddFunction( const std::string& sName, T fFunction )
	{
		return AddFunction( GetCrc32(sName), *(void**)&fFunction );
	}

	virtual unsigned int GetUid( ) { return m_uiUid; };

	virtual int EntryPoint( ) { return 0; };

	virtual void* GetRvaPtr( unsigned int uiAddr ) { return nullptr; }

	virtual int Link( ) { return 0; };

	virtual void* GetProcAddr( unsigned int uiKey )
	{
		FuncLookupList::iterator i = mFuncList.find( uiKey );
		return i != mFuncList.end() ? i->second : 0;
	};

private:
	unsigned int m_uiUid;
	FuncLookupList mFuncList;

};

#endif // HarmModuleVirt_h__

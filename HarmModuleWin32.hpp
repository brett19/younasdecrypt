#ifndef HarmModuleWin32_h__
#define HarmModuleWin32_h__

#include <winnt.h>
#include "HarmModule.hpp"

class HarmModuleWin32 : public HarmModule
{
public:
	HarmModuleWin32( HarmModuleLinker* pLinker ) : HarmModule( pLinker ) { }

	int Init( const std::string& sModuleName )
	{
		m_sName = sModuleName;
		m_uiUid = GetCrc32(sModuleName);

		// Load Win32 Library
		mModule = LoadLibrary( sModuleName.c_str() );

		if( !mModule ) { return -1; }
		if(((PIMAGE_DOS_HEADER)mModule)->e_magic != IMAGE_DOS_SIGNATURE) {
			return -4;
		}
		PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)( (BYTE*)mModule + ((PIMAGE_DOS_HEADER)mModule)->e_lfanew );
		if(header->Signature != IMAGE_NT_SIGNATURE) { return -2; }
		if(header->OptionalHeader.NumberOfRvaAndSizes <= 0) { return -3; }
		PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY)( (BYTE*)mModule + header->
			OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress );
		PVOID names = (BYTE *)mModule + exports->AddressOfNames;
		for (int i = 0; i < exports->NumberOfNames; i++) {
			char* exportname = (char*)( (BYTE *)mModule + ((DWORD *)names)[i] );
			mFuncList.insert( FuncLookupEntry( GetCrc32(exportname), GetProcAddress(mModule,exportname) ) );
		}

		return 0;
	}

	template <typename T>
	int AddHookFunction( unsigned int uiKey, T fFunction )
	{
		mHookList[ uiKey ] = *(void**)&fFunction;
		return 0;
	}

	template <typename T>
	int AddHookFunction( const std::string& sName, T fFunction )
	{
		return AddHookFunction( GetCrc32(sName), *(void**)&fFunction );
	}

	virtual unsigned int GetUid( ) { return m_uiUid; };

	// EP is handled by Win32
	virtual int EntryPoint( ) { return 0; };

	virtual void* GetRvaPtr( unsigned int uiAddr ) { return nullptr; }

	// Linking is handled by Win32
	virtual int Link( ) { return 0; };

	virtual void* GetProcAddr( unsigned int uiKey )
	{
		void* fHooked = mHookList[ uiKey ];
		return fHooked ? fHooked : mFuncList[ uiKey ];
	};

protected:
	std::string m_sName;
	unsigned int m_uiUid;
	HMODULE mModule;
	FuncLookupList mHookList;
	FuncLookupList mFuncList;

};

#endif // HarmModuleWin32_h__

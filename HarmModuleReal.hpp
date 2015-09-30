#ifndef HarmModuleReal_h__
#define HarmModuleReal_h__

#include <windows.h>
#include "HarmModule.hpp"

#pragma pack(push,1)
struct HARMPE_HEADER
{
	unsigned int uiMagic;
	char cCheckVal[0x114];
	unsigned int uiUid;
	unsigned int uiSectionRva;
	unsigned int uiVirtualSize;
	unsigned int uiNumSections;
	unsigned int uiNumRelocations;
	unsigned int uiNumImports;
	unsigned int uiNumExports;
	unsigned int uiNumModules;
	unsigned int uiEntryPoint;
};

struct HARMPE_SECTION_HEADER
{
	unsigned char ucFlags;
	unsigned int uiVirtAddr;
	unsigned int uiVirtSize;
	unsigned int uiRealSize;
};
#pragma pack(pop)

class HarmModuleReal : public HarmModule
{
public:
	HarmModuleReal( HarmModuleLinker* pLinker ) : HarmModule( pLinker ) { }

	// LoadMode is 0 for Normal, 1 for Resource
	int Load( HarmModuleData* pData )
	{
		HARMPE_HEADER* pHeader = (HARMPE_HEADER*)pData;

		m_uiUid = pHeader->uiUid;

		// Map the module into memory!
		pBaseAddress = (unsigned char*)VirtualAlloc( nullptr, pHeader->uiVirtualSize, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE );
		{
			unsigned char* pSection = (unsigned char*)pData + pHeader->uiSectionRva;
			for( int i = 0; i < pHeader->uiNumSections; ++i ) {
				HARMPE_SECTION_HEADER* pSHeader = (HARMPE_SECTION_HEADER*)pSection;
				unsigned char* pSData = pSection + sizeof(HARMPE_SECTION_HEADER);

				memcpy( pBaseAddress + pSHeader->uiVirtAddr, pSData, pSHeader->uiRealSize );

				pSection += sizeof(HARMPE_SECTION_HEADER) + pSHeader->uiRealSize;
			}
		}

		// Grab the entry point
		m_fEntryPoint = (F_EntryPoint)( pBaseAddress + pHeader->uiEntryPoint );

		unsigned char* pIter = (unsigned char*)pData + sizeof(HARMPE_HEADER);
		// Load Additional Modules
		for( int i = 0; i < pHeader->uiNumModules; ++i ) {
			unsigned uiUid = *((unsigned int*)pIter);
			pIter += 4;
			char* pcModuleName = (char*)pIter;
			pIter += strlen(pcModuleName) + 1;

			m_pLinker->LoadWin32Module( pcModuleName );
		}

		// Process Relocations
		for( int i = 0; i < pHeader->uiNumRelocations; ++i ) {
			unsigned int uiRelAddr = *((unsigned int*)pIter);
			pIter += 4;

			*((unsigned int*)(pBaseAddress+uiRelAddr)) += (unsigned int)pBaseAddress - 0x00A00000;
		}

		// Read Imports for Later
		unsigned int uiModuleUid = 0;
		for( int i = 0; i < pHeader->uiNumImports; ++i ) {
			unsigned int uiUid = *((unsigned int*)pIter);
			pIter += 4;
			unsigned int uiRelAddr = *((unsigned int*)pIter);
			pIter += 4;

			if( uiRelAddr == 0 ) {
				uiModuleUid = uiUid;
			} else {
				ImportEntry entry = { uiModuleUid, uiUid, uiRelAddr };
				m_rImports.push_back( entry );
			}
		}

		// Process Exports
		for( int i = 0; i < pHeader->uiNumExports; ++i ) {
			unsigned int uiUid = *((unsigned int*)pIter);
			pIter += 4;
			unsigned int uiRelAddr = *((unsigned int*)pIter);
			pIter += 4;

			m_rExports.insert( FuncLookupEntry( uiUid, pBaseAddress + uiRelAddr ) );
		}

		return 0;
	}

	void SetBreakpoint( unsigned int uiFuncUid )
	{
		*((unsigned char**)&m_rExports[ uiFuncUid ]) -= 1;
	}

	void SetBreakpoint( const std::string& sFuncName )
	{
		SetBreakpoint( GetCrc32(sFuncName) );
	}

	virtual int EntryPoint( )
	{
		if( m_fEntryPoint ) {
			return m_fEntryPoint( 0, DLL_PROCESS_ATTACH, 0 );
		} else {
			return 0;
		}
	}

	virtual void* GetRvaPtr( unsigned int uiAddr )
	{
		return pBaseAddress + uiAddr;
	}

	virtual unsigned int GetUid( ) { return m_uiUid; };

	virtual int Link( )
	{
		HarmModule* pCurModule = nullptr;
		unsigned int uiCurUid = 0;

		for( ImportList::iterator i = m_rImports.begin( ); i != m_rImports.end( ); ++i )
		{
			void* fFunction = nullptr;

			if( i->uiModuleUid != uiCurUid || pCurModule == nullptr ) {
				pCurModule = m_pLinker->GetModule( i->uiModuleUid );
			}
			if( pCurModule ) {
				fFunction = pCurModule->GetProcAddr( i->uiImportUid );
			} else {
				//throw new std::exception( "There is an import module missing!" );
			}

			if( !fFunction ) {
				printf( "BADFUNCTION:: Unable to Resolve MOD:%08x  FUNC:%08x\n", i->uiModuleUid, i->uiImportUid );
				fFunction = (void*)&V_InvalidFunction;
				//throw new std::exception( "There is an import function missing!" );
			}

			*((unsigned int*)(pBaseAddress + i->uiFuncRva)) = (unsigned int)fFunction;
		}
		m_rImports.clear( );

		return 0;
	};

	template <typename T>
	int AddHookFunction( unsigned int uiKey, T fFunction )
	{
		m_rHookList[ uiKey ] = *(void**)&fFunction;
		return 0;
	}

	template <typename T>
	int AddHookFunction( const std::string& sName, T fFunction )
	{
		return AddHookFunction( GetCrc32(sName), *(void**)&fFunction );
	}

	static void __cdecl V_InvalidFunction( )
	{
		printf( "[INVALID_FUNCTION_CALL!!]\n");
	}

	virtual void* GetProcAddr( unsigned int uiKey )
	{
		void* fHooked = m_rHookList[ uiKey ];
		return fHooked ? fHooked : m_rExports[ uiKey ];
	};

protected:
	typedef int (__stdcall *F_EntryPoint)( unsigned int uiInstance, unsigned int uiReason, void* pReserved );

	unsigned int m_uiUid;

	unsigned char* pBaseAddress;
	F_EntryPoint m_fEntryPoint;

	FuncLookupList m_rHookList;
	FuncLookupList m_rExports;

	struct ImportEntry
	{
		unsigned int uiModuleUid;
		unsigned int uiImportUid;
		unsigned int uiFuncRva;
	};
	typedef std::vector<ImportEntry> ImportList;
	ImportList m_rImports;

};

#endif // HarmModuleReal_h__

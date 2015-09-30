#ifndef HarmEmuRagFile_h__
#define HarmEmuRagFile_h__

#include "HarmEmu_Base.hpp"

namespace HarmEmu
{
	namespace RagFile
	{
		void* __fastcall Constructor( void* pObject )
		{
			HarmEmu::LogMsg( "-::RagFile[%08x]::RagFile( )\n", pObject );
			return pObject;
		}

		void __fastcall Destructor( void* pObject )
		{
			HarmEmu::LogMsg( "-::RagFile[%08x]::~RagFile( )\n", pObject );
		}

		class Impl
		{
		public:
			bool Open( char* pcPath, unsigned int uiUnk, bool bUnk )
			{
				HarmEmu::LogMsg( "-::RagFile[%08x]::Open( '%s', %08x, %s )\n", this, pcPath, uiUnk, bUnk?"true":"false" );
				return false;
			}

			void* ptr( long )
			{
				HarmEmu::LogMsg( "-::RagFile[%08x]::ptr( )\n", this );
				return this;
			}

			unsigned long GetLength( )
			{
				HarmEmu::LogMsg( "-::RagFile[%08x]::GetLength( )\n", this );
				return 1;
			}
		};

	}
}

#endif // HarmEmuRagFile_h__

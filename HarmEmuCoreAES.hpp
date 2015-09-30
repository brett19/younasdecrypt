#ifndef HarmEmuCoreAES_h__
#define HarmEmuCoreAES_h__

#include "aes.h"
#include "HarmEmu_Base.hpp"

namespace HarmEmu
{
	namespace Core
	{
		namespace AES
		{
			void* __fastcall Constructor( void* pObject )
			{
				//HarmEmu::LogMsg( "-::Core::AES[%08x]::AES( )\n", pObject );

				*((::AES**)pObject) = new ::AES( );

				return pObject;
			}

			class Impl
			{
			public:
				void SetParameters( int keylength, int blocklength )
				{
					//HarmEmu::LogMsg( "-::Core::AES[%08x]::SetParameters( %d, %d )\n", this, keylength, blocklength );

					((::AES*)this)->SetParameters( keylength, blocklength );
				}

				void StartEncryption( const unsigned char * key )
				{
					//HarmEmu::LogMsg( "-::Core::AES[%08x]::StartEncryption( %08x )\n", this, key );
					//PrintBinary( key, 32 );

					((::AES*)this)->StartEncryption( key );
				}

				void Encrypt( const unsigned char * datain, unsigned char * dataout, unsigned long numBlocks, int mode )
				{
					//HarmEmu::LogMsg( "-::Core::AES[%08x]::Encrypt( %08x, %08x, %d, %d )\n", this, datain, dataout, numBlocks, mode );

					//memcpy( dataout, datain, numBlocks*16 );
					((::AES*)this)->Encrypt( datain, dataout, numBlocks, (::AES::BlockMode)mode );
				}
			};

		}
	}
}

#endif // HarmEmuCoreAES_h__

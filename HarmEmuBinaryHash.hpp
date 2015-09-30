#ifndef HarmEmuBinaryHash_h__
#define HarmEmuBinaryHash_h__

#include "md5.h"
#include "HarmEmu_Base.hpp"
#include "HarmModule.hpp"

namespace HarmEmu
{
	namespace BinaryHash
	{
		void __cdecl __Generic( )
		{
			HarmEmu::LogMsg( "-::BinaryHash::__Generic( )\n" );
		}

		bool __cdecl RoBinaryHash( unsigned char* pData )
		{
			MD5 client_md;
			client_md.digestFile( "D:\\Apps\\Ragray\\LRO.exe" );
			memcpy( pData, client_md.digestRaw, 16 );

			HarmEmu::LogMsg( "-::BinaryHash::RoBinaryHash( %08x )\n", pData );
			//PrintBinary( client_md.digestRaw, 16 );

			return true;
		}
	}
}

#endif // HarmEmuBinaryHash_h__
#ifndef HarmEmuPacketCrypt_h__
#define HarmEmuPacketCrypt_h__

#include "md5.h"
#include "HarmEmu_Base.hpp"
#include "HarmModule.hpp"

namespace HarmEmu
{
	namespace PacketCrypt
	{
		void __cdecl CalcKey( unsigned char* pcOut, unsigned char* pcSeed )
		{
			unsigned char pcBase[] = {
				0xd9, 0x7c, 0xf1, 0x44, 0x3e, 0xdd, 0xa5, 0xef, 0xfd, 0x29, 0xe9, 0x89, 0x81, 0x33, 0x6a, 0x0c,
				0x47, 0xbd, 0xb8, 0xb9, 0x41, 0x66, 0x2b, 0x88, 0x17, 0xfe, 0x6f, 0x93, 0x8f, 0x7a, 0x53, 0xdc
			};
			for( int i = 0; i < 32; i++ ) {
				pcOut[i] = pcBase[i] ^ pcSeed[i%4] ^ pcSeed[4+i%4];
			}
		};
	}
}

#endif // HarmEmuPacketCrypt_h__
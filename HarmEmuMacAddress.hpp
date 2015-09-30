#ifndef HarmEmuMacAddress_h__
#define HarmEmuMacAddress_h__

#include "HarmEmu_Base.hpp"

namespace HarmEmu
{
	namespace MacAddress
	{
		bool __cdecl Determine( unsigned char* pAddr )
		{
			HarmEmu::LogMsg( "-::MacAddress::Determine( %08x )\n", pAddr );

			unsigned char fake_mac[6] = {
				/* Vendor ID */ 0xBC, 0xAE, 0xC5,	// ASUSTek Computers
				/* Device ID */ 0x7F, 0xF3, 0x43	// Randomness
			};
			memcpy( pAddr, fake_mac, 6 );

			return true;
		}

		bool __cdecl DetermineRemote( unsigned char* pAddr )
		{
			HarmEmu::LogMsg( "-::MacAddress::DetermineRemote( %08x )\n", pAddr );

			unsigned char fake_mac[6] = {
				/* Vendor ID */ 0x00, 0x26, 0xF3,	// SMC Networks
				/* Device ID */ 0x1F, 0xD3, 0x99	// Randomness
			};//00:26:F3
			memcpy( pAddr, fake_mac, 6 );

			return true;
		}

		void __cdecl PrintMacAddress( unsigned char* pAddr )
		{
			HarmEmu::LogMsg( "-::MacAddress::PrintMacAddress( %08x )\n", pAddr );
		}
	}
}

#endif // HarmEmuMacAddress_h__
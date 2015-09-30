#include "crc32.h"

unsigned int crc32Table[256];

void InitCrc32 ()
{
	int i, j;
	unsigned int dwPolynomial;
	unsigned int dwCrc;

	dwPolynomial = 0xEDB88320; // VIDEO4

	// Lookup table
	for (i = 0; i<256; i++){
		dwCrc = i;
		for (j = 8; j>0; j--){
			if (dwCrc & 1){
				dwCrc >>= 1;
				dwCrc ^= dwPolynomial;
			}
			else {
				dwCrc >>= 1;
			}
		}
		crc32Table[i] = dwCrc;
	}
}

unsigned int GetCrc32(unsigned char *buf, unsigned int length, unsigned int uiStartVal ){
	static bool bCrcInited = false;
	if( !bCrcInited ) {
		InitCrc32( );
		bCrcInited = true;
	}

	unsigned int crc32Result = ~uiStartVal;
	unsigned char iLookup;
	unsigned int i;

	for (i=0; i<length; i++){
		iLookup = (crc32Result & 0xFF) ^ buf[i];
		crc32Result >>= 8;
		crc32Result ^= crc32Table[iLookup];
	}

	return ~(crc32Result);
}

unsigned int GetCrc32( const std::string& sText, unsigned int uiStartVal )
{
	return GetCrc32( (unsigned char*)sText.c_str( ), sText.size( ), uiStartVal );
}
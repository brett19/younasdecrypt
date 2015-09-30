#ifndef crc32_h__
#define crc32_h__

#include <string>

unsigned int GetCrc32(unsigned char *buf, unsigned int length, unsigned int uiStartVal = 0 );
unsigned int GetCrc32( const std::string& sText, unsigned int uiStartVal = 0 );

#endif // crc32_h__
#include <iostream>
#include <stdarg.h>
#include "HarmEmu_Base.hpp"

//#define DEBUG_HARMONY

namespace HarmEmu
{
#ifdef DEBUG_HARMONY
	void LogMsg( const char* pcFormat, ... )
	{
		va_list args;
		va_start( args, pcFormat );
		vprintf( pcFormat, args );
		va_end( args );
	}
#else
	void LogMsg( const char* pcFormat, ... ) { }
#endif

	char PrintBinary_FixChar( unsigned char c ) {
		return ( c >= ' ' && c <= '~' ) ? c : '.';
	}

	void PrintBinary( const unsigned char* pData, unsigned int uiLen )
	{
		printf( "-------------------------------------------------------\n" );
		int cur_byte = 0;
		while( cur_byte < uiLen ) {
			int cur_byte_tmp = cur_byte;
			for( int i = 0; i < 0x10; i++, cur_byte++ ) {
				if( cur_byte < uiLen ) {
					printf( "%02x ", pData[cur_byte] );
				} else {
					printf( "   " );
				}
			}
			printf( "   " );
			cur_byte = cur_byte_tmp;
			for( int i = 0; i < 0x10; i++, cur_byte++ ) {
				if( cur_byte < uiLen ) {
					printf( "%c", PrintBinary_FixChar(pData[cur_byte]) );
				} else {
					printf( " " );
				}
			}
			printf( "\n" );
		}
		printf( "-------------------------------------------------------\n" );
	}
};
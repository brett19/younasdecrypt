#ifndef HarmModuleBundle_h__
#define HarmModuleBundle_h__

#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>

typedef unsigned char* HarmModuleData;

class HarmModuleBundle
{
public:
	int Load( const std::string& sBundlePath )
	{
		FILE* fh = fopen( sBundlePath.c_str(), "rb" );

		fseek( fh, 0, SEEK_END );
		unsigned int uiFileLen = ftell(fh);
		fseek( fh, 0, SEEK_SET );

		unsigned int uiSize;
		unsigned int uiRealSize;

		unsigned int iTargetId = 4;
		while( ftell(fh) < uiFileLen )
		{
			fread( &uiSize, 4, 1, fh );
			fread( &uiRealSize, 4, 1, fh );

			uiSize ^= GetXorKey( iTargetId++ );
			uiRealSize ^= 0x1B642C18;

			unsigned char* pDData = new unsigned char[ uiRealSize ];

			char* pData = new char[ uiSize - 4 ];
			fread( pData, 1, uiSize - 4, fh );
			uncompress( (Bytef*)pDData, (uLongf*)&uiRealSize, (const Bytef*)pData, (uLong)(uiSize - 4) );	
			delete[] pData;

			m_rModules.push_back( (HarmModuleData*)pDData );
		}

		fclose( fh );

		return 0;
	}

	size_t GetCount( )
	{
		return m_rModules.size( );
	}

	HarmModuleData* Get( unsigned int uiIdx )
	{
		return m_rModules[ uiIdx ];
	}

protected:
	unsigned int GetXorKey( unsigned int uiSeed )
	{
		unsigned retval = uiSeed;
		for( int i = 0; i < 8; i++ ) {
			retval = ( retval & 1 ) ? ( retval >> 1 ^ 0xE92DEF5C ) : ( retval >> 1 );
		}
		return retval;
	}

	std::vector< HarmModuleData* > m_rModules;

};

#endif // HarmModuleBundle_h__
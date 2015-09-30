#ifndef HarmEmu_Base_h__
#define HarmEmu_Base_h__

namespace HarmEmu
{
	void LogMsg( const char* pcFormat, ... );

	char PrintBinary_FixChar( unsigned char c );
	void PrintBinary( const unsigned char* pData, unsigned int uiLen );
};

#endif //HarmEmu_Base_h__
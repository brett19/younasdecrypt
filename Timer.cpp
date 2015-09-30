#include "Timer.hpp"
#include <windows.h>

namespace Timer
{
	unsigned __int64 g_llFrequency;
	unsigned __int64 g_llCurTime;

	unsigned __int64 GetTicks( )
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&g_llCurTime );
		return g_llCurTime;
	}

	unsigned int GetDiffMs( unsigned __int64 llPrevious )
	{
		if( g_llFrequency == 0 ) {
			QueryPerformanceFrequency( (LARGE_INTEGER*)&g_llFrequency );
		}
		QueryPerformanceCounter( (LARGE_INTEGER*)&g_llCurTime );
		return (unsigned int)((g_llCurTime-llPrevious)/g_llFrequency);
	}
};
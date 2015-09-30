#ifndef Timer_hpp__
#define Timer_hpp__

namespace Timer
{
	unsigned __int64 GetTicks( );
	unsigned int GetDiffMs( unsigned __int64 llPrevious );
};

#endif // Timer_hpp__
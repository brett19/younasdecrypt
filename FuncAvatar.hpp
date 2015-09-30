#ifndef FuncAvatar_hpp__
#define FuncAvatar_hpp__

#include <functional>

typedef void (*AvaMoveCallback)( bool bSuccess );

std::function<void(bool)> g_fAvaMoveCallback;
unsigned int g_uiMoveTimeout = 0;
unsigned int g_uiMoveDoneTime = 0;
int g_iAvaTargetX = 0;
int g_iAvaTargetY = 0;

void AvaMoveTo( int iTargetX, int iTargetY, std::function<void(bool)> fCallback = nullptr )
{
	printf( "--AvaMoveTo %d,%d\n", iTargetX, iTargetY );

	g_uiMoveTimeout = GetTickCount( );
	g_fAvaMoveCallback = fCallback;

	unsigned int uiMoveTime = g_uiMoveSpeed * ( (FAST_ABS(iTargetX-g_iAvaPosX) + FAST_ABS(iTargetY-g_iAvaPosY) > 1) ? 14 : 10 ) / 10;
	g_uiMoveDoneTime = GetTickCount() + uiMoveTime;

	RoPakMoveTo xPak;
	xPak.pos.Set( iTargetX, iTargetY, 0 );
	BotNetwork::SendPacket( 0x00a7, &xPak, sizeof(xPak) );
}

void AvaMoveReply( int iTargetX, int iTargetY )
{
	g_iAvaTargetX = iTargetX;
	g_iAvaTargetY = iTargetY;
	g_uiMoveTimeout = 0;
}

void AvaUpdatePos( )
{
	printf( "--AvaPosition = %d,%d\n", g_iAvaPosX, g_iAvaPosY );
}

void AvaMoveUpdate( )
{
	bool bSuccess = false;

	if( !g_uiMoveTimeout || GetTickCount( ) - g_uiMoveTimeout < 2000 ) {
		if( !g_uiMoveDoneTime || GetTickCount( ) < g_uiMoveDoneTime ) {
			// Still Processing Previous Move
			return;
		}

		// Movement is done
		g_iAvaPosX = g_iAvaTargetX;
		g_iAvaPosY = g_iAvaTargetY;

		AvaUpdatePos( );

		bSuccess = true;
	}

	g_uiMoveTimeout = 0;
	g_uiMoveDoneTime = 0;
	Tasks::Active()->OnMoveComplete( bSuccess );
}

#endif //FuncAvatar_hpp__
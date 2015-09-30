#ifndef FuncPathing_hpp__
#define FuncPathing_hpp__

#include "FuncAvatar.hpp"
#include "Timer.hpp"

MapTree::MapPath g_xPath;
unsigned int g_uiNextPathMove = 0;
unsigned int g_uiPathTargetX = 0;
unsigned int g_uiPathTargetY = 0;

float AvaPathCalc( int iX, int iY );
void AvaPathNext( );

void AvaPathUpdate( )
{
}

void _AvaPathMoveHandler( bool bSuccess )
{
	if( bSuccess ) {
		g_xPath.Pop( );
	}
	AvaPathNext( );
}

void AvaPathNext( )
{
	if( g_xPath.IsEmpty( ) ) return;
	AvaMoveTo( g_xPath.GetBackX(), g_xPath.GetBackY(), &_AvaPathMoveHandler );
}

void AvaPathStart( )
{
	AvaPathNext( );
}

float AvaPathCalc( int iX, int iY )
{
	MapTree::Map* pMap = MapTree::GetMap(g_pcMapName);
	MapTree::MapPather xPather( pMap );

	/*
	std::vector<std::pair<int,int>> rAvoidList;
	rAvoidList.push_back(std::pair<int,int>(135, 108));
	xPather.SetAvoid( &rAvoidList );
	*/

	float fCost = 0.0f;
	xPather.Solve( g_iAvaPosX, g_iAvaPosY, iX, iY, &fCost, &g_xPath );
	return fCost;
}

void AvaMapChanged( )
{
	// Map Changed, Intra-Map pathing no longer valid
	g_xPath.Clear( );
}

#endif
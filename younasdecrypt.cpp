#include <iostream>
#include "micropather.h"
#include "HarmCtrl.hpp"
#include "BotNetwork.hpp"
#include "RoPackets.hpp"
#include "RoInfo.hpp"
#include "Globals.hpp"
#include "MapTree.hpp"
#include "Tasks.hpp"

#include "FuncMisc.hpp"
#include "FuncMessage.hpp"
#include "FuncAvatar.hpp"
#include "FuncPathing.hpp"
#include "FuncNpc.hpp"
#include "TaskMapMove.hpp"
#include "TaskNpcWarper.hpp"
#include "TaskWorldMove.hpp"
#include "TaskDBRoom.hpp"
#include "HndlLogin.hpp"
#include "HndlAvatar.hpp"
#include "HndlUnits.hpp"
#include "HndlZone.hpp"
#include "HndlNpc.hpp"
#include "Hndl_Router.hpp"

unsigned int g_uiLastPing = 0;

void CheckTickTock( )
{
	if( GetTickCount() - g_uiLastPing > 12000 ) {
		SendTick( );
		g_uiLastPing = GetTickCount( );
	}
}

void BotFrameTick( )
{
	// Check for Ping
	CheckTickTock( );

	// Update Avatar Position
	AvaMoveUpdate( );

	// Proc Tasks
	Tasks::Active()->Proc( );
}

int main( int argc, char* argv[] )
{
	
	std::vector<std::string> rTest = VecTok( "Test1:Test2:Test3:Test4", ':' );

	unsigned int uiStartTimeX = GetTickCount( );
	if( !MapTree::InitFromCache( "D:\\RoBotData\\bot_map_cache.dat" ) ) {
		MapTree::LoadMaps( "D:\\RoBotData\\map_cache.dat" );
		MapTree::LoadWarps( "D:\\RoBotData\\warps.txt" );
		MapTree::LoadWarpers( "D:\\RoBotData\\warpers.txt" );
		MapTree::Precalc( );
		MapTree::SaveToCache( "D:\\RoBotData\\bot_map_cache.dat" );
	}
	printf( "Map Data Init took %dms\n", GetTickCount()-uiStartTimeX );

	/*
	unsigned int uiStartTimeY = GetTickCount( );
	float fCost = 0.0f;
	MapTree::Map* pSrcMap = MapTree::GetMap( "c_tower1" );
	MapTree::Map* pDestMap = MapTree::GetMap( "aldebaran" );
	MapTree::WorldPath xPath;
	MapTree::WorldPather xGraph;
	xGraph.Solve( pSrcMap, 196, 163, pDestMap, 128, 117, &fCost, &xPath );

	printf( "Found Route - Cost: %f  Took: %dms\n", fCost, GetTickCount()-uiStartTimeY );
	while( !xPath.IsEmpty() ) {
		MapTree::WpNode* pWarp = xPath.GetBack();
		printf( "%s [%s](%d,%d) -> [%s](%d,%d)\n",
			(pWarp->GetType()==MapTree::WT_WARPER) ? "[WARPER]" : "[PORTAL]",
			pWarp->pSrc->pMap->sName.c_str(), pWarp->pSrc->iX, pWarp->pSrc->iY,
			pWarp->pDest->pMap->sName.c_str(), pWarp->pDest->iX, pWarp->pDest->iY );
		xPath.Pop( );
	}

	system( "PAUSE" );
	return 0;
	//*/

	BotNetwork::Init( &RoutePacket );
	HarmCtrl::LoadHarmony( );
	Tasks::Init( );

	strcpy( pcAccount, "belivious" );
	strcpy( pcPassword, "diablo" );
	strcpy( pcAvatar, "Burtteh" );

	g_uiLastPing = GetTickCount( );
	ConnectLogin( "208.43.76.98", 6984 );

	g_bRunning = true;
	while( g_bRunning )
	{
		BotNetwork::Poll( );
		BotFrameTick( );
		Sleep( 10 );
	}

	system( "PAUSE" );

	return 0;
}
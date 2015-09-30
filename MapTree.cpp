#include "MapTree.hpp"
#include <zlib.h>
#include <unordered_map>
#include <vector>
#include <Windows.h>

namespace MapTree
{
	std::unordered_map<std::string,unsigned int> g_rMapLookup;
	std::vector<Map*> g_rMaps;
	std::vector<WpNode*> g_rWarps;
	std::vector<WpDest*> g_rDests;
	std::vector<WpSrc*> g_rSrcs;

	Map* GetMap( const std::string& sMapName )
	{
		auto iFinder = g_rMapLookup.find( sMapName );
		if( iFinder == g_rMapLookup.end() ) return nullptr;
		return g_rMaps[ iFinder->second ];
	};

	bool SaveToCache( const std::string& sPath )
	{
		char pcBuffer[ 256 ];

		FILE* fh = fopen( sPath.c_str(), "wb" );

		unsigned int uiNumMaps = g_rMaps.size( );
		fwrite( &uiNumMaps, 4, 1, fh );

		std::unordered_map<Map*,unsigned int> rMaps;
		for( int i = 0; i < uiNumMaps; ++i )
		{
			Map* pMap = g_rMaps[i];

			strcpy( pcBuffer, pMap->sName.c_str() );
			fwrite( pcBuffer, 24, 1, fh );
			
			fwrite( &pMap->uiWidth, 4, 1, fh );
			fwrite( &pMap->uiHeight, 4, 1, fh );

			fwrite( pMap->pCells, pMap->uiWidth*pMap->uiHeight, 1, fh );

			rMaps[pMap] = i;
		}

		std::unordered_map<WpNode*, unsigned int> rWaypoints;

		unsigned int* pNumWarps = new unsigned int[ WT_MAX ];
		memset( pNumWarps, 0, WT_MAX*sizeof(unsigned int) );

		unsigned int uiNumWarps = g_rWarps.size( );
		for( int i = 0; i < uiNumWarps; ++i ) {
			WpNode* pNode = g_rWarps[i];
			pNumWarps[ pNode->GetType() ]++;
			rWaypoints[ pNode ] = i;
		}

		unsigned int uiNumDests = g_rDests.size( );
		fwrite( &uiNumDests, 4, 1, fh );

		std::unordered_map<WpDest*,unsigned int> rDests;
		for( int i = 0; i < uiNumDests; ++i )
		{
			WpDest* pDest = g_rDests[i];

			unsigned int uiMapId = rMaps[ pDest->pMap ];
			fwrite( &uiMapId, 4, 1, fh );
			fwrite( &pDest->iX, 4, 1, fh );
			fwrite( &pDest->iY, 4, 1, fh );

			unsigned int uiNumCosts = pDest->rCosts.size();
			fwrite( &uiNumCosts, 4, 1, fh );

			for( int j = 0; j < uiNumCosts; ++j ) {
				WpCost& xCost = pDest->rCosts[j];

				unsigned int iWpIdx = rWaypoints[ xCost.pWaypoint ];
				fwrite( &iWpIdx, 4, 1, fh );
				fwrite( &xCost.fCost, 4, 1, fh );
			}

			rDests[pDest] = i;
		}

		unsigned int uiNumSrcs = g_rSrcs.size( );
		fwrite( &uiNumSrcs, 4, 1, fh );

		std::unordered_map<WpSrc*,unsigned int> rSrcs;
		for( int i = 0; i < uiNumSrcs; ++i )
		{
			WpSrc* pSrc = g_rSrcs[i];

			unsigned int uiMapId = rMaps[ pSrc->pMap ];
			fwrite( &uiMapId, 4, 1, fh );
			fwrite( &pSrc->iX, 4, 1, fh );
			fwrite( &pSrc->iY, 4, 1, fh );

			rSrcs[pSrc] = i;
		}

		for( unsigned int uiType = 0; uiType < WT_MAX; ++uiType )
		{
			fwrite( &pNumWarps[uiType], 4, 1, fh );

			for( int i = 0; i < uiNumWarps; ++i )
			{
				WpNode* pWarp = g_rWarps[ i ];
			
				if( pWarp->GetType() != uiType ) continue;

				unsigned int uiDestId = rDests[ pWarp->pDest ];
				unsigned int uiSrcId = rSrcs[ pWarp->pSrc ];

				fwrite( &uiDestId, 4, 1, fh );
				fwrite( &uiSrcId, 4, 1, fh );
				fwrite( &pWarp->uiUserInfo, 4, 1, fh );
			}
		}

		delete[] pNumWarps;

		fclose( fh );
		return true;
	};

	bool InitFromCache( const std::string& sPath )
	{
		char pcBuffer[ 256 ];

		FILE* fh = fopen( sPath.c_str(), "rb" );
		if( !fh ) return false;

		unsigned int uiNumMaps;
		fread( &uiNumMaps, 4, 1, fh );

		Map* pMaps = new Map[ uiNumMaps ];
		for( int i = 0; i < uiNumMaps; ++i ) {
			Map* pMap = &pMaps[i];

			fread( pcBuffer, 1, 24, fh );
			pMap->sName = pcBuffer;

			fread( &pMap->uiWidth, 4, 1, fh );
			fread( &pMap->uiHeight, 4, 1, fh );

			pMap->pCells = new unsigned char[ pMap->uiWidth*pMap->uiHeight ];
			fread( pMap->pCells, pMap->uiWidth*pMap->uiHeight, 1, fh );

			g_rMaps.push_back( pMap );
			g_rMapLookup.insert(std::pair<std::string,unsigned int>( pMap->sName, i ));
		}

		unsigned int uiNumDests = 0;
		fread( &uiNumDests, 4, 1, fh );

		WpCost xCost;

		WpDest* pDests = new WpDest[ uiNumDests ];
		for( int i = 0; i < uiNumDests; ++i ) {
			WpDest* pDest = &pDests[i];

			unsigned int uiMapId = 0;
			fread( &uiMapId, 4, 1, fh );
			pDest->pMap = &pMaps[ uiMapId ];

			fread( &pDest->iX, 4, 1, fh );
			fread( &pDest->iY, 4, 1, fh );

			unsigned int uiNumCosts = 0;
			fread( &uiNumCosts, 4, 1, fh );

			for( int j = 0; j < uiNumCosts; ++j ) {
				fread( &xCost.pWaypoint, 4, 1, fh );
				fread( &xCost.fCost, 4, 1, fh );
				pDest->rCosts.push_back( xCost );
			}

			pDest->pMap->rWpDests.push_back( pDest );
			g_rDests.push_back( pDest );
		}

		unsigned int uiNumSrcs = 0;
		fread( &uiNumSrcs, 4, 1, fh );

		WpSrc* pSrcs = new WpSrc[ uiNumSrcs ];
		for( int i = 0; i < uiNumSrcs; ++i ) {
			WpSrc* pSrc = &pSrcs[i];

			unsigned int uiMapId = 0;
			fread( &uiMapId, 4, 1, fh );
			pSrc->pMap = &pMaps[ uiMapId ];

			fread( &pSrc->iX, 4, 1, fh );
			fread( &pSrc->iY, 4, 1, fh );

			pSrc->pMap->rWpSrcs.push_back( pSrc );
			g_rSrcs.push_back( pSrc );
		}

		for( unsigned int uiType = 0; uiType < WT_MAX; ++uiType )
		{
			unsigned int uiNumWarps = 0;
			fread( &uiNumWarps, 4, 1, fh );

			for( int i = 0; i < uiNumWarps; ++i )
			{
				WpNode* pNode = nullptr;
				if( uiType == WT_PORTAL ) pNode = new WpPortal( );
				else if( uiType == WT_WARPER ) pNode = new WpWarper( );
				
				unsigned int uiDestId = 0;
				fread( &uiDestId, 4, 1, fh );
				pNode->pDest = &pDests[ uiDestId ];

				unsigned int uiSrcId = 0;
				fread( &uiSrcId, 4, 1, fh );
				pNode->pSrc = &pSrcs[ uiSrcId ];

				fread( &pNode->uiUserInfo, 4, 1, fh );

				pNode->pSrc->pMap->rWaypoints.push_back( pNode );
				g_rWarps.push_back( pNode );
			}
		}

		// We gotta post-process the destinations to hook them to the warps
		for( auto i = g_rDests.begin(); i != g_rDests.end(); ++i ) {
			for( auto j = (*i)->rCosts.begin(); j != (*i)->rCosts.end(); ++j ) {
				(*j).pWaypoint = g_rWarps[ (unsigned int)(*j).pWaypoint ];
			}
		}

		fclose( fh );
		return true;
	};
	
	bool CalculateWarpAdjacent( MapPather& xPather, Map* pMap )
	{
		xPather.SetMap( pMap );

		WpCost xCost;
		for( auto i = pMap->rWpDests.begin(); i != pMap->rWpDests.end(); ++i )
		{
			WpDest* pDest = *i;
			for( auto j = pMap->rWpSrcs.begin(); j != pMap->rWpSrcs.end(); ++j )
			{
				WpSrc* pSrc = *j;

				if( xPather.Solve( pDest->iX, pDest->iY, pSrc->iX, pSrc->iY, &xCost.fCost, nullptr ) ) {
					for( auto k = pMap->rWaypoints.begin(); k != pMap->rWaypoints.end(); ++k )
					{
						if( (*k)->pSrc == pSrc ) {
							xCost.pWaypoint = *k;
							pDest->rCosts.push_back( xCost );
						}
					}
				}
			}
		}

		return true;
	};

	bool Precalc( )
	{
		// Calculate adjacency list
		unsigned int iTotalMaps = g_rMaps.size( );

		unsigned int iMapsCompleted = 0;
		MapPather xPather;

		#pragma omp parallel for private(xPather)
		for( int i = 0; i < iTotalMaps; ++i ) {
			Map* pMap = g_rMaps[i];

			unsigned int uiStartTime = GetTickCount( );
			CalculateWarpAdjacent( xPather, pMap );
			
			#pragma omp critical
			{
				iMapsCompleted++;
				printf( "% 4d/%d :: Map '%s' took %dms to pregenerate\n", iMapsCompleted, iTotalMaps, pMap->sName.c_str(), GetTickCount()-uiStartTime );
			}
		}
		return true;
	};

	bool LoadMaps( const std::string& sPath )
	{
		struct FileHeader {
			unsigned int file_size;
			unsigned short map_count;
		};

		struct FileMapHeader {
			char name[ 12 ];
			unsigned short xs;
			unsigned short ys;
			unsigned int len;
		};

		FILE* fh = fopen( sPath.c_str(), "rb" );

		FileHeader xHeader = { 0 };
		fread( &xHeader, sizeof(FileHeader), 1, fh );

		unsigned int uiBufferLen = 300 * 300;
		unsigned char* pcBuffer = new unsigned char[ uiBufferLen ];

		unsigned int uiSquareTileage = 0;

		FileMapHeader xMapHeader = { 0 };
		for( int i = 0; i < xHeader.map_count; ++i )
		{
			fread( &xMapHeader, sizeof(FileMapHeader), 1, fh );
			if( xMapHeader.len > uiBufferLen ) {
				delete[] pcBuffer;
				uiBufferLen = uiBufferLen * 3 / 2; // (1.5 times the size)
				pcBuffer = new unsigned char[ uiBufferLen ];
			}
			fread( pcBuffer, 1, xMapHeader.len, fh );
			
			Map* pMap = new Map( );
			pMap->sName = xMapHeader.name;
			pMap->uiWidth = xMapHeader.xs;
			pMap->uiHeight = xMapHeader.ys;

			unsigned int uiRealSize = pMap->uiWidth * pMap->uiHeight;
			pMap->pCells = new unsigned char[ uiRealSize ];
			uncompress( (Bytef*)pMap->pCells, (uLongf*)&uiRealSize, (Bytef*)pcBuffer, xMapHeader.len );

			g_rMaps.push_back( pMap );
			g_rMapLookup.insert(std::pair<std::string,unsigned int>( pMap->sName, g_rMaps.size()-1 ));

			uiSquareTileage += uiRealSize;
		}

		delete[] pcBuffer;

		fclose( fh );
		return true;
	}

	WpSrc* GetMapWpSrc( Map* pMap, int iX, int iY )
	{
		WpSrc* pSrc = new WpSrc( );
		pSrc->pMap = pMap;
		pSrc->iX = iX;
		pSrc->iY = iY;

		pMap->rWpSrcs.push_back( pSrc );
		g_rSrcs.push_back( pSrc );

		return pSrc;
	}

	WpDest* GetMapWpDest( Map* pMap, int iX, int iY )
	{
		WpDest* pDest = nullptr;

		for( auto i = pMap->rWpDests.begin(); i != pMap->rWpDests.end(); ++i ) {
			// Don't need to check map since it implicitly is equal (we are searching the maps dest list)
			if( (*i)->iX == iX && (*i)->iY == iY ) {
				return *i;
			}
		}
		
		if( !pDest ) {
			pDest = new WpDest( );
			pDest->pMap = pMap;
			pDest->iX = iX;
			pDest->iY = iY;
			pMap->rWpDests.push_back( pDest );
			g_rDests.push_back( pDest );
		}

		return pDest;
	}

	bool LoadWarps( const std::string& sPath )
	{
		static char sBuffer[ 1024 ];
		FILE* fh = fopen( sPath.c_str(), "rb" );

		char src_map[ 24 ] = "", dest_map[ 24 ] = "";
		unsigned int src_x = 0, src_y = 0, dest_x = 0, dest_y = 0;

		unsigned int uiWarpIdx = 0;
		while( !feof(fh) )
		{
			fscanf( fh, "%s %d %d %s %d %d", src_map, &src_x, &src_y, dest_map, &dest_x, &dest_y );
			fgets( sBuffer, 1024, fh );

			if( src_map[0] == '#' ) continue;

			Map* pSrcMap = GetMap( src_map );
			Map* pDestMap = GetMap( dest_map );
			if( !pSrcMap || !pDestMap ) continue;

			WpSrc* pSrc = GetMapWpSrc( pSrcMap, src_x, src_y );
			WpDest* pDest = GetMapWpDest( pDestMap, dest_x, dest_y );

			WpPortal* pWarp = new WpPortal( );
			pWarp->pSrc = pSrc;
			pWarp->pDest = pDest;
			pWarp->uiUserInfo = 0;

			pSrcMap->rWaypoints.push_back( pWarp );
			g_rWarps.push_back( pWarp );

			uiWarpIdx++;
		}

		fclose( fh );

		return true;
	}

	bool LoadWarpers( const std::string& sPath )
	{
		static char sBuffer[ 1024 ];
		FILE* fh = fopen( sPath.c_str(), "rb" );

		char map[ 24 ] = "";
		unsigned int x = 0, y = 0;

		std::vector<WpSrc*> rSrcs;
		std::vector<WpDest*> rDests;

		while( !feof(fh) )
		{
			fscanf( fh, "%s %d %d", map, &x, &y );
			fgets( sBuffer, 1024, fh );

			if( map[0] == '#' || !map[0] ) continue;

			Map* pMap = GetMap( map );
			if( !pMap ) continue;

			WpSrc* pSrc = GetMapWpSrc( pMap, x, y );
			WpDest* pDest = GetMapWpDest( pMap, x, y );
			rSrcs.push_back( pSrc );
			rDests.push_back( pDest );
		}

		unsigned int uiDestIdx = 0;
		for( auto i = rDests.begin(); i != rDests.end(); i++ ) {
			for( auto j = rSrcs.begin(); j != rSrcs.end(); j++ ) {
				WpWarper* pWarper = new WpWarper( );
				pWarper->pSrc = *j;
				pWarper->pDest = *i;
				pWarper->uiUserInfo = uiDestIdx;

				(*j)->pMap->rWaypoints.push_back( pWarper );
				g_rWarps.push_back( pWarper );
			}
			uiDestIdx++;
		}		

		fclose( fh );

		return true;
	}
}
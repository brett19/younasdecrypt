#ifndef MapTree_hpp__
#define MapTree_hpp__

#include <string>
#include <vector>
#include <unordered_map>
#include "micropather.h"
#include "MapInfo.hpp"
#include "MapPath.hpp"
#include "WorldPath.hpp"

namespace MapTree
{
	Map* GetMap( const std::string& sMapName );
	bool LoadMaps( const std::string& sPath );
	bool LoadWarpers( const std::string& sPath );
	bool LoadWarps( const std::string& sPath );
	bool Precalc( );
	bool SaveToCache( const std::string& sPath );
	bool InitFromCache( const std::string& sPath );
}

#endif
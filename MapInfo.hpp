#ifndef MapInfo_hpp__
#define MapInfo_hpp__

namespace MapTree
{
	/*
	struct WarpPortal;

	struct WarpCost
	{
		WarpPortal* pDest;
		float fCost;
	};

	struct PFNode
	{
		// 0 - WarpPortal, 1 - SENode, 2 - WarpNpc 
		virtual unsigned int GetType( ) = 0;
	};
	
	struct WarpNpc : PFNode
	{
		virtual unsigned int GetType( ) { return 2; }
	};

	struct WarpPortal : PFNode
	{
		virtual unsigned int GetType( ) { return 0; }

		unsigned int iWarpId;
		Map* pSrcMap;
		unsigned int iSrcX;
		unsigned int iSrcY;
		Map* pDestMap;
		unsigned int iDestX;
		unsigned int iDestY;
		std::vector<WarpCost> rCosts;
	};
	*/

	struct Map;
	struct WpNode;

	enum WaypointType
	{
		WT_START = 0,
		WT_END = 1,
		WT_PORTAL = 2,
		WT_WARPER = 3,
		WT_MAX = 4
	};

	struct WpPosition
	{
		Map* pMap;
		unsigned int iX;
		unsigned int iY;
	};

	struct WpSrc : public WpPosition
	{
	};

	struct WpCost
	{
		float fCost;
		WpNode* pWaypoint;
	};

	struct WpDest : public WpPosition
	{
		std::vector<WpCost> rCosts;
	};

	struct WpObject
	{
		virtual int GetType( ) = 0;
	};

	struct WpNode : public WpObject
	{
		unsigned int uiUserInfo;
		WpSrc* pSrc;
		WpDest* pDest;
	};

	struct WpPortal : public WpNode
	{
		virtual int GetType( ) { return WT_PORTAL; };
	};

	struct WpWarper : public WpNode
	{
		virtual int GetType( ) { return WT_WARPER; };
	};

	struct Map
	{
		std::string sName;
		unsigned int uiWidth;
		unsigned int uiHeight;
		unsigned char* pCells;
		//std::vector<WarpPortal*> rWarpIn;		// Warps into this map
		//std::vector<WarpPortal*> rWarpOut;		// Warps off this map
		
		std::vector<WpDest*> rWpDests;
		std::vector<WpSrc*> rWpSrcs;
		std::vector<WpNode*> rWaypoints;
	};
};

#endif // MapInfo_hpp__
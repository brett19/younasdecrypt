#ifndef WorldPath_hpp__
#define WorldPath_hpp__

#include "MapInfo.hpp"
#include "MapPath.hpp"

namespace MapTree
{
	// Holds a world path, NOTE, it also holds start/end nodes so it ignores those
	//  It also uses a counter to allow us to go through the list backwards
	class WorldPath
	{
	public:
		WorldPath() { }

		WpNode* GetBack( ) const { return (WpNode*)m_rPath.back(); }
		bool IsEmpty( ) const { return m_rPath.empty( ); }
		void Pop( ) { m_rPath.pop_back(); }
		void Clear( ) { m_rPath.clear(); }

		std::vector<WpNode*> m_rPath;
	};

	class WorldPather : micropather::Graph
	{
	public:
		struct WpTemporary : public WpObject
		{
			Map* pMap;
			int iX;
			int iY;
		};

		struct WpStart : public WpTemporary
		{
			virtual int GetType( ) { return WT_START; }

			std::unordered_map<WpNode*,float> rCosts;
		};

		struct WpEnd : public WpTemporary
		{
			virtual int GetType( ) { return WT_END; }

			std::unordered_map<WpDest*,float> rCosts;
		};

		WorldPather( ) : m_pPather(nullptr) { m_pPather = new micropather::MicroPather( this ); }

		void GenerateRCosts( WpTemporary* pNode )
		{
			float fCost = 0.0f;
			MapPather xPather( pNode->pMap );

			if( pNode->GetType( ) == WT_START ) {
				WpStart* pStart = (WpStart*)pNode;

				pStart->rCosts.clear();
				for( auto i = pStart->pMap->rWaypoints.begin(); i != pStart->pMap->rWaypoints.end(); ++i ) {
					if( xPather.Solve( pStart->iX, pStart->iY, (*i)->pSrc->iX, (*i)->pSrc->iY, &fCost, nullptr ) ) {
						pStart->rCosts.insert(std::pair<WpNode*,float>(*i, fCost));
					}
				}
			} else {
				WpEnd* pEnd = (WpEnd*)pNode;

				pEnd->rCosts.clear();
				for( auto i = pEnd->pMap->rWpDests.begin(); i != pEnd->pMap->rWpDests.end(); ++i ) {
					if( xPather.Solve( (*i)->iX, (*i)->iY, pEnd->iX, pEnd->iY, &fCost, nullptr ) ) {
						pEnd->rCosts.insert(std::pair<WpDest*,float>(*i, fCost));
					}
				}
			}
		};

		bool Solve( Map* pSMap, unsigned short iSx, unsigned short iSy, Map* pEMap, unsigned short iEx, unsigned short iEy, float* pfCost, WorldPath* pPath )
		{
			if( pSMap == pEMap ) {
				*pfCost = 0.0f;
				pPath->m_rPath.clear( );
				return false;
			}

			m_xStart.pMap = pSMap;
			m_xStart.iX = iSx;
			m_xStart.iY = iSy;
			GenerateRCosts( &m_xStart );

			m_xEnd.pMap = pEMap;
			m_xEnd.iX = iEx;
			m_xEnd.iY = iEy;
			GenerateRCosts( &m_xEnd );

			if( !pPath ) {
				return m_pPather->Solve( &m_xStart, &m_xEnd, nullptr, pfCost ) == 0;
			} else {
				std::vector<void*> rPath;

				bool bRetVal = m_pPather->Solve( &m_xStart, &m_xEnd, &rPath, pfCost ) == 0;
				for( auto i = rPath.rbegin(); i != rPath.rend(); ++i ) {
					WpObject* pObject = (WpObject*)*i;
					if( pObject->GetType() != WT_START && pObject->GetType() != WT_END ) {
						pPath->m_rPath.push_back( (WpNode*)(*i) );
					}
				}
				return bRetVal;
			}
		}

	protected:
		WpStart m_xStart;
		WpEnd m_xEnd;
		micropather::MicroPather* m_pPather;

		virtual float LeastCostEstimate( void* stateStart, void* stateEnd )
		{
			return 1.0f;
		};

		void AdjacentCostStart( WpStart* pNode, std::vector< micropather::StateCost > *adjacent )
		{
			for( auto i = pNode->rCosts.begin(); i != pNode->rCosts.end(); ++i )
			{
				micropather::StateCost stateCost = { i->first, i->second };
				adjacent->push_back( stateCost );
			}
		};

		void AdjacentCostEnd( WpNode* pNode, std::vector< micropather::StateCost > *adjacent )
		{
			// Add the end node if we can
			if( pNode->pDest->pMap == m_xEnd.pMap ) {
				auto iEndLink = m_xEnd.rCosts.find( pNode->pDest );
				if( iEndLink != m_xEnd.rCosts.end() ) {
					micropather::StateCost stateCost = { &m_xEnd, iEndLink->second };
					adjacent->push_back( stateCost );
				}
			}
		}

		void AdjacentCostNode( WpNode* pWarp, std::vector< micropather::StateCost > *adjacent )
		{
			// Add attached Warps
			for( auto i = pWarp->pDest->rCosts.begin(); i != pWarp->pDest->rCosts.end(); ++i )
			{
				if( i->pWaypoint == pWarp ) continue;

				micropather::StateCost stateCost = { i->pWaypoint, i->fCost };
				adjacent->push_back( stateCost );
			}

			AdjacentCostEnd( pWarp, adjacent );
		};

		virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent )
		{
			WpObject* pNode = (WpObject*)state;
			if( pNode->GetType() == WT_START ) {
				AdjacentCostStart( (WpStart*)state, adjacent );
			} else if( pNode->GetType() == WT_END ) {
			} else if( pNode->GetType() == WT_PORTAL ) {
				AdjacentCostNode( (WpPortal*)state, adjacent );
			} else if( pNode->GetType() == WT_WARPER ) {
				AdjacentCostNode( (WpWarper*)state, adjacent );
			}
		};

		virtual void PrintStateInfo( void* state ) { }

	};
};

#endif // WorldPath_hpp__
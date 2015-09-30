#ifndef MapPath_hpp__
#define MapPath_hpp__

#include "MapInfo.hpp"

namespace MapTree
{
	static inline bool CellIsWalkable( unsigned char cCell ) { return ( cCell != 1 && cCell != 5 ) ? true : false; }

	struct MyState
	{
		unsigned short x;
		unsigned short y;
	};
	static_assert( sizeof(MyState)==sizeof(void*), "ERROR WITH STATE DATA" );
	static inline void* MakeState( MyState* pState ) { return (void*)(*((unsigned int*)pState)); }


	// Holds a map path NOTE: It holds the first node as being your current location, so it ignores that.
	class MapPath
	{
	public:
		MapPath() { }
		MapPath( unsigned int uiInitialSize ) { m_rPath.reserve(uiInitialSize); }

		inline MyState* GetBack( ) const { return ((MyState*)&(m_rPath[m_rPath.size()-2])); }
		unsigned short GetBackX( ) const { return GetBack( )->x; }
		unsigned short GetBackY( ) const { return GetBack( )->y; }
		bool IsEmpty( ) const { return m_rPath.size() <= 1; }
		void Pop( ) { m_rPath.pop_back(); m_rPath.pop_back(); m_rPath.push_back(nullptr); }
		void Clear( ) { m_rPath.clear(); }

		std::vector<void*> m_rPath;

	};

	class MapPather : micropather::Graph
	{
	public:
		MapPather( ) : m_pMap(nullptr), m_pPather(nullptr), m_pAvoid(nullptr) { }
		MapPather( Map* pMap ) : m_pMap(nullptr), m_pPather(nullptr), m_pAvoid(nullptr) { SetMap(pMap); }

		void SetMap( Map* pMap )
		{
			m_pMap = pMap;
			if( !m_pPather ) {
				m_pPather = new micropather::MicroPather( this, 8000, 8 );
			}
			m_pPather->Reset( );
		}

		bool Solve( unsigned short iSx, unsigned short iSy, unsigned short iEx, unsigned short iEy, float* pfCost, MapPath* pPath )
		{
			MyState xStartState = { iEx, iEy };
			MyState xEndState = { iSx, iSy };

			if( !pPath ) {
				return m_pPather->Solve( MakeState(&xStartState), MakeState(&xEndState), nullptr, pfCost ) == 0;
			} else {
				return m_pPather->Solve( MakeState(&xStartState), MakeState(&xEndState), &pPath->m_rPath, pfCost ) == 0;
			}
		}

		void SetAvoid( const std::vector<std::pair<int,int>>* pAvoid )
		{
			m_pAvoid = pAvoid;
		}

	protected:
		const std::vector<std::pair<int,int>>* m_pAvoid;
		micropather::MicroPather* m_pPather;

		virtual float LeastCostEstimate( void* stateStart, void* stateEnd )
		{
			MyState* pStartState = (MyState*)&stateStart;
			MyState* pEndState = (MyState*)&stateEnd;

			int iX = abs( pStartState->x - pEndState->x );
			int iY = abs( pStartState->y - pEndState->y );

			// Manhatten Distance
			return (float)( iX + iY );
		};

		inline bool CellIsProtected( int x, int y )
		{
			if( !m_pAvoid ) return false;
			for( auto i = m_pAvoid->begin(); i != m_pAvoid->end(); ++i ) {
				if( i->first == x && i->second == y ) return true;
			}
			return false;
		};

		inline bool AddAdjacent( int x, int y, float weight, std::vector< micropather::StateCost > *adjacent )
		{
			if( x < 0 || y < 0 || x >= m_pMap->uiWidth || y >= m_pMap->uiHeight || 
				!CellIsWalkable(m_pMap->pCells[x+y*m_pMap->uiWidth]) ) return false;

			if( CellIsProtected(x,y) ) {
				weight *= 100;
			}

			MyState xState = { x, y };

			micropather::StateCost stateCost = { MakeState(&xState), weight };
			adjacent->push_back( stateCost );
			return true;
		};

		virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent )
		{
			MyState* pState = (MyState*)&state;
			unsigned short& iX = pState->x;
			unsigned short& iY = pState->y;

			bool bRight = AddAdjacent( iX + 1,	iY,			1.0f, adjacent );
			bool bLeft =  AddAdjacent( iX - 1,	iY,			1.0f, adjacent );
			bool bDown =  AddAdjacent( iX,		iY - 1,		1.0f, adjacent );
			bool bUp =	  AddAdjacent( iX,		iY + 1,		1.0f, adjacent );

			if( bRight && bUp )
				AddAdjacent( iX + 1,	iY + 1,		1.4f, adjacent );
			if( bRight && bDown )
				AddAdjacent( iX + 1,	iY - 1,		1.4f, adjacent );
			if( bLeft && bUp )
				AddAdjacent( iX - 1,	iY + 1,		1.4f, adjacent );
			if( bLeft && bDown )
				AddAdjacent( iX - 1,	iY - 1,		1.4f, adjacent );
		};

		virtual void PrintStateInfo( void* state ) { }

		Map* m_pMap;

	};
};

#endif // MapPath_hpp__
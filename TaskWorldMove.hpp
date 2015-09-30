#ifndef TaskWorldMove_hpp__
#define TaskWorldMove_hpp__

class TaskWorldMove : public Tasks::Task
{
public:
	TaskWorldMove( const std::string& sTargetMap, int iTargetX, int iTargetY )
		: m_sTargetMap(sTargetMap), m_iTargetX(iTargetX), m_iTargetY(iTargetY)
	{
		m_pTargetMap = MapTree::GetMap( m_sTargetMap );
	}

	virtual std::string GetDesc( ) const {
		static char pcBuffer[ 512 ];
		sprintf( pcBuffer, "TaskWorldMove : '%s' %d,%d", m_sTargetMap.c_str(), m_iTargetX, m_iTargetY );
		return pcBuffer;
	}

	virtual void Proc( )
	{
	}

	void MoveNext( )
	{
		MapTree::Map* pCurMap = MapTree::GetMap( g_pcMapName );

		if( pCurMap == m_pTargetMap || m_xPath.IsEmpty( ) ) {
			if( g_iAvaPosX != m_iTargetX || g_iAvaPosY != m_iTargetY ) {
				Tasks::Push( new TaskMapMove( m_iTargetX, m_iTargetY ) );
			} else {
				Tasks::Pop( );
			}
			return;
		}
		
		MapTree::WpNode* pWarp = m_xPath.GetBack();
		
		if( pCurMap == pWarp->pSrc->pMap && pWarp->GetType() == MapTree::WT_WARPER ) {
			// Warp Npc
			Tasks::PushPaused( new TaskNpcWarper( pWarp->pDest->pMap->sName ) );
			Tasks::Push( new TaskMapMove( pWarp->pSrc->iX, pWarp->pSrc->iY ) );
			return;
		}

		if( pCurMap == pWarp->pDest->pMap ) {
			// Arrived already
			m_xPath.Pop( );
			MoveNext( );
			return;
		}

		if( pCurMap != pWarp->pSrc->pMap ) {
			// Odd...
			__asm{ int 3 };
			CalculatePath();
			MoveNext();
			return;
		}

		Tasks::Push( new TaskMapMove( pWarp->pSrc->iX, pWarp->pSrc->iY ) );
	}

	void CalculatePath( )
	{
		MapTree::Map* pCurMap = MapTree::GetMap( g_pcMapName );

		if( pCurMap == m_pTargetMap ) {
			m_xPath.Clear( );
			return;
		}

		MapTree::WorldPather xGraph;
		xGraph.Solve( pCurMap, g_iAvaPosX, g_iAvaPosY, m_pTargetMap, m_iTargetX, m_iTargetY, &m_fCost, &m_xPath );

		MapTree::WorldPath xPath = m_xPath;

		printf( "--- CALCULATED WORLD MOVE PATH ---\n" );
		printf( "  ======== '%s' -> '%s'\n", pCurMap->sName.c_str(), m_pTargetMap->sName.c_str() );
		while( !xPath.IsEmpty() ) {
			MapTree::WpNode* pWarp = xPath.GetBack();

			printf( "%s [%s](%d,%d) -> [%s](%d,%d)\n",
				(pWarp->GetType()==MapTree::WT_WARPER) ? "[WARPER]" : "[PORTAL]",
				pWarp->pSrc->pMap->sName.c_str(), pWarp->pSrc->iX, pWarp->pSrc->iY,
				pWarp->pDest->pMap->sName.c_str(), pWarp->pDest->iX, pWarp->pDest->iY );

			xPath.Pop( );
		}
		printf( "----------------------------------\n" );
	}

	virtual void Resume( )
	{
		CalculatePath( );
		MoveNext( );
	}

	virtual void OnMapChange( )
	{
		MoveNext( );
	}

protected:
	MapTree::WorldPath m_xPath;
	float m_fCost;
	std::string m_sTargetMap;
	int m_iTargetX;
	int m_iTargetY;
	MapTree::Map* m_pTargetMap;

};

#endif // TaskMapMove_hpp__
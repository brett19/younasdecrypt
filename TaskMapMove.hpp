#ifndef TaskMapMove_hpp__
#define TaskMapMove_hpp__

class TaskMapMove : public Tasks::Task
{
public:
	TaskMapMove( int iTargetX, int iTargetY )
		: m_iTargetX(iTargetX), m_iTargetY(iTargetY)
	{
	}

	virtual std::string GetDesc( ) const {
		static char pcBuffer[ 512 ];
		sprintf( pcBuffer, "TaskMapMove : %d,%d", m_iTargetX, m_iTargetY );
		return pcBuffer;
	}

	virtual void Proc( )
	{
	}

	virtual void OnMoveComplete( bool bSuccess )
	{
		if( bSuccess ) {
			m_xPath.Pop( );
		} else {
			__asm{ int 3 };
		}
		MoveNext( );
	}

	void MoveNext( )
	{
		if( m_xPath.IsEmpty( ) ) {
			Tasks::Pop( );
			return;
		}
		AvaMoveTo( m_xPath.GetBackX(), m_xPath.GetBackY(), nullptr );
	}

	virtual void Resume( )
	{
		// Calculate Path

		MapTree::Map* pMap = MapTree::GetMap(g_pcMapName);
		MapTree::MapPather xPather( pMap );

		/*
		std::vector<std::pair<int,int>> rAvoidList;
		rAvoidList.push_back(std::pair<int,int>(135, 108));
		xPather.SetAvoid( &rAvoidList );
		*/

		if( !xPather.Solve( g_iAvaPosX, g_iAvaPosY, m_iTargetX, m_iTargetY, &m_fCost, &m_xPath ) ) {
			__asm{ int 3 };
		}
		MoveNext( );
	}

	virtual void OnMapChanged( )
	{
		// If the map changes, this task is no longer valid
		Tasks::Pop( );

		// Propagate since we pop on this event
		//Tasks::Active( )->OnMapChanged( );
	}

protected:
	MapTree::MapPath m_xPath;
	float m_fCost;
	int m_iTargetX;
	int m_iTargetY;

};

#endif // TaskMapMove_hpp__
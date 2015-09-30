#ifndef TaskNpcWarper_hpp__
#define TaskNpcWarper_hpp__

class TaskNpcWarper : public Tasks::Task
{
public:
	TaskNpcWarper( const std::string& sDestMap )
		: m_sDestMap(sDestMap), m_pUnit(nullptr)
	{
		if( m_sDestMap == "aldebaran" ) {
			m_sMapType = "Town";
			m_sMapWord = "Aldebaran";
		} else if( m_sDestMap == "gaia" ) {
			m_sMapType = "Town";
			m_sMapWord = "Gaia";
		} else if( m_sDestMap == "prontera" ) {
			m_sMapType = "Town";
			m_sMapWord = "Prontera";
		} else if( m_sDestMap == "xmas" ) {
			m_sMapType = "Town";
			m_sMapWord = "Lutie";
		}
	}

	virtual std::string GetDesc( ) const {
		static char pcBuffer[ 512 ];
		sprintf( pcBuffer, "TaskNpcWarper : ." );
		return pcBuffer;
	}

	virtual void Proc( )
	{
	}

	bool CheckNpc( GameUnit* pUnit )
	{
		if( pUnit == nullptr ) { printf( "--------------------------------- MISSING NPC\n" ); return false; }

		if( pUnit->iType != UT_NPC || pUnit->sName != "Warper" ) {
			return false;
		}

		if( !m_pUnit ) {
			printf( "TaskNpcWarper :: Clicked NPC\n" );
			NpcClick( pUnit->iId );
			m_pUnit = pUnit;
		}
		return true;
	}

	virtual void OnNpcNext( GameUnit* pUnit )
	{
		if( pUnit == nullptr ) { printf( "--------------------------------- MISSING NPC\n" ); return; }

		NpcNext( m_pUnit->iId );
	}

	virtual void OnNpcMenu( GameUnit* pUnit, const std::vector<std::string>& rOptions )
	{
		if( pUnit == nullptr ) { printf( "--------------------------------- MISSING NPC\n" ); return; }

		for( int i = 0; i < rOptions.size(); ++i ) {
			const std::string& sText = rOptions[i];
			if( sText.find( m_sMapType ) != std::string::npos || sText.find( m_sMapWord ) != std::string::npos ) {
				NpcSelOption( pUnit->iId, i );
				break;
			}
		}
	}

	virtual void Resume( )
	{
		for( auto i = g_rUnits.begin(); i != g_rUnits.end(); ++i ) {
			if( CheckNpc( i->second ) ) {
				break;
			}
		}
	}

	virtual void OnUnitSpawn( GameUnit* pUnit )
	{
		CheckNpc( pUnit );
	}

	virtual void OnMapChanged( )
	{
		// Success!
		Tasks::Pop( );
	}

protected:
	std::string m_sMapType;
	std::string m_sMapWord;
	GameUnit* m_pUnit;
	std::string m_sDestMap;

};

#endif // TaskNpcWarper_hpp__
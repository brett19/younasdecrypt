#ifndef HndlUnits_hpp__
#define HndlUnits_hpp__

void Recv_UnitSpawnIdle( void* pcData, unsigned short unSize )
{
	RoPakUnitSpawnIdle* pPacket = (RoPakUnitSpawnIdle*)pcData;

	printf( "UnitSpawn(Idle) [%08x] {%s} '%s' @ %d,%d\n", 
		pPacket->id, pcUnitTypes[pPacket->type], pPacket->name, 
		pPacket->pos.GetX(), pPacket->pos.GetY() );

	if( pPacket->class_id != 111 ) {
		GameUnit* pUnit = new GameUnit( );
		pUnit->iType = (UNIT_TYPE)pPacket->type;
		pUnit->iId = pPacket->id;
		pUnit->sName = pPacket->name;
		pUnit->uiMoveSpeed = pPacket->speed;
		pUnit->iX = pPacket->pos.GetX( );
		pUnit->iY = pPacket->pos.GetY( );
		g_rUnits.insert(std::pair<unsigned int,GameUnit*>(pUnit->iId,pUnit));

		Tasks::Active()->OnUnitSpawn( pUnit );
	}
}

void Recv_UnitSpawnMove( void* pcData, unsigned short unSize )
{
	RoPakUnitSpawnMove* pPacket = (RoPakUnitSpawnMove*)pcData;

	printf( "UnitSpawn(Move) [%08x] {%s} '%s' @ %d,%d -> %d,%d\n",
		pPacket->id, pcUnitTypes[pPacket->type], pPacket->name,
		pPacket->pos.GetX1(), pPacket->pos.GetY1(), pPacket->pos.GetX2(), pPacket->pos.GetY2() );

	if( pPacket->class_id != 111 ) {
		GameUnit* pUnit = new GameUnit( );
		pUnit->iType = (UNIT_TYPE)pPacket->type;
		pUnit->iId = pPacket->id;
		pUnit->sName = pPacket->name;
		pUnit->uiMoveSpeed = pPacket->speed;
		pUnit->iX = pPacket->pos.GetX2( );
		pUnit->iY = pPacket->pos.GetY2( );
		g_rUnits.insert(std::pair<unsigned int,GameUnit*>(pUnit->iId,pUnit));

		Tasks::Active()->OnUnitSpawn( pUnit );
	}
}

void Recv_UnitMove( void* pcData, unsigned short unSize )
{
	RoPakUnitMove* pPacket = (RoPakUnitMove*)pcData;

	printf( "UnitMove [%08x] %d,%d -> %d,%d\n",
		pPacket->id,
		pPacket->pos.GetX1( ), pPacket->pos.GetY1( ),
		pPacket->pos.GetX2( ), pPacket->pos.GetY2( )
	);

	GameUnit* pUnit = g_rUnits[ pPacket->id ];
	if( pUnit ) {
		pUnit->iX = pPacket->pos.GetX2( );
		pUnit->iY = pPacket->pos.GetY2( );
	}

	Tasks::Active()->OnUnitMove( pUnit );
}

void Recv_UnitClear( void* pcData, unsigned short unSize )
{
	RoPakUnitClear* pPacket = (RoPakUnitClear*)pcData;

	if( pPacket->id == g_uiUnitId ) {
		pPacket->id = 0;
	}

	printf( "UnitClear [%08x] %s\n",
		pPacket->id,
		pcClearTypes[pPacket->type]
	);

	if( pPacket->id == 0 ) return;

	GameUnit* pUnit = g_rUnits[ pPacket->id ];

	Tasks::Active()->OnUnitClear( pUnit );
	
	if( pUnit ) {
		delete pUnit;
		g_rUnits[ pPacket->id ] = nullptr;
	}
}

#endif //Hndl_Units_hpp__
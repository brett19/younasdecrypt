#ifndef HndlZone_hpp__
#define HndlZone_hpp__

void Recv_ZoneReady( void* pcData, unsigned short unSize )
{
	RoPakMapReady* pPacket = (RoPakMapReady*)pcData;

	g_iAvaPosX = pPacket->pos.GetX( );
	g_iAvaPosY = pPacket->pos.GetY( );

	BotNetwork::SendPacket( 0x007d, nullptr, 0 );

	g_rUnits.clear( );
	Tasks::Active()->OnMapChanged( );

	//Tasks::Push( new TaskNpcWarper( "aldebaran" ) );
	//Tasks::PushPaused( new TaskWorldMove( "xmas", 158, 133 ) );
	Tasks::Push( new TaskWorldMove( "aldebaran", 139, 118 ) );
	/*
	
	Tasks::PushPaused( new TaskWorldMove( "aldebaran", 139, 121 ) );
	Tasks::PushPaused( new TaskWorldMove( "c_tower1", 199, 169 ) );
	Tasks::PushPaused( new TaskWorldMove( "aldebaran", 139, 121 ) );
	Tasks::PushPaused( new TaskWorldMove( "c_tower1", 199, 169 ) );
	Tasks::PushPaused( new TaskWorldMove( "aldebaran", 139, 121 ) );
	Tasks::PushPaused( new TaskWorldMove( "c_tower1", 199, 169 ) );
	Tasks::PushPaused( new TaskWorldMove( "aldebaran", 139, 121 ) );
	Tasks::PushPaused( new TaskWorldMove( "c_tower1", 199, 169 ) );
	Tasks::PushPaused( new TaskWorldMove( "aldebaran", 139, 121 ) );
	Tasks::Push( new TaskWorldMove( "c_tower1", 199, 169 ) );
	//*/
}

void Recv_ZoneMapChange( void* pcData, unsigned short unSize )
{
	RoPakMapChange* pPacket = (RoPakMapChange*)pcData;

	strcpy( g_pcMapName, pPacket->name );
	g_pcMapName[ strlen(g_pcMapName)-4 ] = 0;
	g_iAvaPosX = pPacket->x;
	g_iAvaPosY = pPacket->y;

	BotNetwork::SendPacket( 0x007d, nullptr, 0 );

	g_rUnits.clear( );
	Tasks::Active()->OnMapChanged( );
}

#endif //HndlZone_hpp__
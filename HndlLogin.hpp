#ifndef HndlLogin_hpp__
#define HndlLogin_hpp__

unsigned int g_uiAccountId;
unsigned int g_uiCharId;
unsigned int g_uiSessionId[2];

void ConnectLogin( char* pcAddr, unsigned short unPort )
{
	BotNetwork::Connect( 1, pcAddr, unPort );

	RoPakLogin xPakOut;
	xPakOut.server_ver = 0x0132B4E4;
	xPakOut.server_type = 0x0E;
	strcpy( xPakOut.username, pcAccount );
	strcpy( xPakOut.password, pcPassword );
	BotNetwork::SendPacket( 0x64, &xPakOut, sizeof(xPakOut) );
}

void ConnectChar( char* pcAddr, unsigned short unPort )
{
	BotNetwork::Connect( 2, pcAddr, unPort );

	RoPakAvatarConnect xPakOut;
	xPakOut.account_id = g_uiAccountId;
	xPakOut.session_id = g_uiSessionId[0];
	xPakOut.session_id2 = g_uiSessionId[1];
	xPakOut.gender = g_ucGender;
	BotNetwork::SendPacket( 0x65, &xPakOut, sizeof(xPakOut) );

	unsigned int uiAccountId;
	BotNetwork::RecvBytes( &uiAccountId, 4 );
}

void ConnectZone( char* pcAddr, unsigned short unPort )
{
	BotNetwork::Connect( 3, pcAddr, unPort );

	RoPakZoneConnect xPakOut;
	xPakOut.account_id = g_uiAccountId;
	xPakOut.character_id = g_uiCharId;
	xPakOut.session_id = g_uiSessionId[0];
	xPakOut.tick = GetTickCount( );
	xPakOut.gender = g_ucGender;
	BotNetwork::SendPacket( 0x436, &xPakOut, sizeof(xPakOut) );
}

void Recv_ChannelList( void* pcData, unsigned short unSize )
{
	RoPakServersHead* pHead = (RoPakServersHead*)pcData;

	g_uiAccountId = pHead->account_id;
	g_uiSessionId[0] = pHead->session_id;
	g_uiSessionId[1] = pHead->session_id2;
	g_ucGender = pHead->gender;

	printf( "-- Got Server List!\n" );
	RoPakServersEntry* pSelServer = nullptr;
	RoPakServersEntry* pServer = (RoPakServersEntry*)( pHead + 1 );
	while( (unsigned char*)pServer < (unsigned char*)pcData+unSize ) {
		if( strcmpi(pServer->name,"MidRate") == 0 ) {
			pSelServer = pServer;
		}
		
		printf( "  %s[%d] (%d.%d.%d.%d:%d) %s\n", pServer->name, pServer->usercount,
			pServer->ip[0], pServer->ip[1], pServer->ip[2], pServer->ip[3], pServer->port,
			pServer==pSelServer?" SELECTED":"" );

		pServer++;
	}

	BotNetwork::Close( );

	if( pSelServer == nullptr ) {
		printf( "FAILED TO FIND PROPER SERVER\n" );
		g_bRunning = false;
		return;
	}

	char server_ip[16];
	sprintf( server_ip, "%d.%d.%d.%d", pSelServer->ip[0], pSelServer->ip[1], pSelServer->ip[2], pSelServer->ip[3] );
	ConnectChar( server_ip, pSelServer->port );
}

void Recv_AvatarList( void* pcData, unsigned short unSize )
{
	printf( "-- Got Avatar List!\n" );
	RoPakAvatarEntry* pSelAvatar = nullptr;
	RoPakAvatarEntry* pAvatar = (RoPakAvatarEntry*)((unsigned char*)pcData + 20);
	while( (unsigned char*)pAvatar < (unsigned char*)pcData+unSize ) {
		if( strcmpi(pAvatar->name,pcAvatar) == 0 ) {
			pSelAvatar = pAvatar;
		}

		printf( "  [%d] %08x (%s) %s\n",
			pAvatar->slot_id, pAvatar->character_id, pAvatar->name,
			pAvatar==pSelAvatar?" SELECTED":"" );
		pAvatar++;
	}

	if( pSelAvatar == nullptr ) {
		printf( "FAILED TO FIND PROPER CHARACTER\n" );
		g_bRunning = false;
		return;
	}

	g_uiMoveSpeed = pSelAvatar->walk_speed;

	unsigned char ucSelSlot = (unsigned char)pSelAvatar->slot_id;
	BotNetwork::SendPacket( 0x66, &ucSelSlot, 1 );
}

void Recv_ZoneConnectInfo( void* pcData, unsigned short unSize )
{
	RoPakZoneConnectInfo* pZoneInfo = (RoPakZoneConnectInfo*)pcData;

	g_uiCharId = pZoneInfo->char_id;

	BotNetwork::Close( );

	strcpy( g_pcMapName, pZoneInfo->mapname );
	g_pcMapName[ strlen(g_pcMapName)-4 ] = 0;

	char server_ip[16];
	sprintf( server_ip, "%d.%d.%d.%d", pZoneInfo->ip[0], pZoneInfo->ip[1], pZoneInfo->ip[2], pZoneInfo->ip[3] );
	ConnectZone( server_ip, pZoneInfo->port );
}


#endif //HndlLogin_hpp__
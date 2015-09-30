#ifndef HndlNpc_hpp__
#define HndlNpc_hpp__

std::vector<std::string> VecTok( char* pcString, char cDelim = ' ' )
{
	std::vector<std::string> rStrings;
	char* pcLastString = pcString;
	char* pcIter = pcString;

	for( ; ; ) {
		if( *pcIter != cDelim && *pcIter != 0 ) {
			pcIter++;
			continue;
		}
		std::string sTemp( pcLastString, pcIter-pcLastString );
		if( pcLastString != pcString || sTemp != "" ) {
			rStrings.push_back(sTemp);
		}

		if( *pcIter == 0 ) break;

		pcIter++;
		pcLastString = pcIter;
	}

	return rStrings;
};

void Recv_NpcMes( void* pcData, unsigned short unSize )
{
	RoPakNpcDlgMes* pPacket = (RoPakNpcDlgMes*)pcData;

	printf( "-- Recv Npc Mes :: %s\n", pPacket->msg );
}

void Recv_NpcClose( void* pcData, unsigned short unSize )
{
	RoPakNpcDlgClose* pPacket = (RoPakNpcDlgClose*)pcData;

	printf( "-- Recv Npc Close\n" );

	GameUnit* pUnit = g_rUnits[ pPacket->id ];
	Tasks::Active( )->OnNpcClose( pUnit );
}

void Recv_NpcNext( void* pcData, unsigned short unSize )
{
	RoPakNpcDlgNext* pPacket = (RoPakNpcDlgNext*)pcData;

	printf( "-- Recv Npc Next\n" );

	GameUnit* pUnit = g_rUnits[ pPacket->id ];
	Tasks::Active( )->OnNpcNext( pUnit );
}

void Recv_NpcMenu( void* pcData, unsigned short unSize )
{
	RoPakNpcDlgMenu* pPacket = (RoPakNpcDlgMenu*)pcData;

	printf( "-- Recv Npc Menu :: %s\n", pPacket->msg );

	GameUnit* pUnit = g_rUnits[ pPacket->id ];
	std::vector<std::string> rOptions = VecTok( pPacket->msg, ':' );
	Tasks::Active( )->OnNpcMenu( pUnit, rOptions );
}

#endif // HndlNpc_hpp__
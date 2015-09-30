#ifndef FuncNpc_hpp__
#define FuncNpc_hpp__

void NpcClick( unsigned int iId )
{
	RoPakClickNpc xPak;
	xPak.id = iId;
	BotNetwork::SendPacket( 0x0090, &xPak, sizeof(xPak) );
};

void NpcNext( unsigned int iId )
{
	RoPakNpcNext xPak;
	xPak.id = iId;
	BotNetwork::SendPacket( 0x00b9, &xPak, sizeof(xPak) );
};

void NpcSelOption( unsigned int iId, unsigned int iSelNum )
{
	RoPakNpcOption xPak;
	xPak.id = iId;
	xPak.selval = 1 + iSelNum; // 0 is a reserved number
	BotNetwork::SendPacket( 0x00b8, &xPak, sizeof(xPak) );
}

#endif // FuncNpc_hpp__
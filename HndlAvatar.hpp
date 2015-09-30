#ifndef HndlAvatar_hpp__
#define HndlAvatar_hpp__

void Recv_AvaMoveOk( void* pcData, unsigned short unSize )
{
	RoPakMoveOk* pPacket = (RoPakMoveOk*)pcData;

	// Set Current Position to wherever
	g_iAvaPosX = pPacket->pos.GetX1( );
	g_iAvaPosY = pPacket->pos.GetY1( );

	AvaMoveReply( pPacket->pos.GetX2(), pPacket->pos.GetY2() );
}

void Recv_UnitId( void* pcData, unsigned short unSize )
{
	g_uiUnitId = *(unsigned int*)pcData;
}

void Recv_UpdateStatusLong( void* pcData, unsigned short unSize )
{
	RoPakUpdateStatusL* pPacket = (RoPakUpdateStatusL*)pcData;

	switch( pPacket->type )
	{
		case SC_MOVESPEED: g_uiMoveSpeed = pPacket->value; break;
	}
}

#endif //Hndl_Units_hpp__
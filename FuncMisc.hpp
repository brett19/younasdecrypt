#ifndef FuncMisc_hpp__
#define FuncMisc_hpp__

void SendTick( )
{
	RoPakTick xPak = { 0, GetTickCount() };
	BotNetwork::SendPacket( 0x0089, &xPak, sizeof(xPak) );
}

#endif //FuncMisc_hpp__
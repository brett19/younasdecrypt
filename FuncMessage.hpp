#ifndef FuncMessage_hpp__
#define FuncMessage_hpp__

void SendGlobalMessage( const char* pcFormat, ... )
{
	static char pcMessage[ 256 ];
	static char pcSendData[ 24 + 4 + 256 ];

	va_list args;
	va_start( args, pcFormat );
	vsprintf( pcMessage, pcFormat, args );
	va_end( args );

	sprintf( pcSendData, "%s : %s", pcAvatar, pcMessage );

	BotNetwork::SendPacket( 0x00f3, pcSendData, strlen(pcSendData) + 1 );
}

#endif //FuncMessage_hpp__
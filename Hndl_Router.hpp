#ifndef Hndl_Router_hpp__
#define Hndl_Router_hpp__

void RoutePacket( unsigned short unCmd, void* pcData, unsigned short unSize )
{
	switch( unCmd ){
		case 0x0069: // Channel Listing
			Recv_ChannelList(pcData,unSize); break;
		case 0x006b: // Avatar Listing
			Recv_AvatarList(pcData,unSize); break;
		case 0x0071: // Zone Server Info
			Recv_ZoneConnectInfo(pcData,unSize); break;
		case 0x0073: // AuthOK
			Recv_ZoneReady(pcData,unSize); break;
		case 0x007f: // PingReply
			break;
		case 0x0080: // Clear Unit
			Recv_UnitClear(pcData,unSize); break;
		case 0x0086: // Move Unit
			Recv_UnitMove(pcData,unSize); break;
			break;
		case 0x0087: // Move OK
			Recv_AvaMoveOk(pcData,unSize); break;
			break;
		case 0x008d: // Global Message
			break;
		case 0x008e: // Display Message
			break;
		case 0x0091: // Change Map
			Recv_ZoneMapChange(pcData,unSize); break;
			break;
		case 0x009a: // Broadcast Message
			break;
		case 0x009c: // Change Direction
			break;
		case 0x00b1:
		case 0x00b0: // UpdateStatus_Long
			Recv_UpdateStatusLong(pcData,unSize); break;
		case 0x00b4: // NpcMes
			Recv_NpcMes(pcData,unSize); break;
		case 0x00b5: // NpcNext
			Recv_NpcNext(pcData,unSize); break;
		case 0x00b6: // NpcClose
			Recv_NpcClose(pcData,unSize); break;
		case 0x00b7: // NpcMenu
			Recv_NpcMenu(pcData,unSize); break;
		case 0x00bd: // Initial Status
			break;
		case 0x00be: // Update Status Point Requirement
			break;
		case 0x00d7: // Chat Room
			break;
		case 0x010f: // Skill Info Block??
			break;
		case 0x011a: // Skill (No Damage)
			break;
		case 0x011f: // Spawn Skill Unit (Bragi, Dissonance, etc..)
			break;
		case 0x013a: // Update Attack Range
			break;
		case 0x0141: // UpdateStat
			break;
		case 0x017f: // Guild Message
			break;
		case 0x01c9: // Graffiti
			break;
		case 0x01d7: // UpdateLook
			break;
		case 0x01e1: // Spiritball Single
			break;
		case 0x01f3: // Special Effect
			break;
		case 0x0283: // My ID
			Recv_UnitId(pcData,unSize); break;
		case 0x02e8: // Update Inventory (Stackable)
			break;
		case 0x02d0: // Update Inventory (Equipable)
			break;
		case 0x043f: // Status Change
			break;
		case 0x07d9: // Hotkeys
			break;
		case 0x07f7: // Spawn Unit Moving
			Recv_UnitSpawnMove(pcData,unSize); break;
		case 0x07f9: // Spawn Unit Idle
			Recv_UnitSpawnIdle(pcData,unSize); break;
		default:
			printf( "PAK -- [%04x] %d\n", unCmd, unSize );
			break;
	}
}

#endif //Hndl_Router_hpp__
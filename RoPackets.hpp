#ifndef RoPackets_h__
#define RoPackets_h__
#pragma pack(push,1)

struct RoPacket
{
	unsigned short unCmd;
	unsigned char pcData[ 0x400 ];
};
struct RoMovePos
{
	unsigned int GetX1( ) { return ( ( data[0] << 2 ) | ( data[1] >> 6 ) ) & 0x3FF; };
	unsigned int GetY1( ) { return ( ( data[2] >> 4 ) | ( data[1] << 4 ) ) & 0x3FF; };
	unsigned int GetX2( ) { return ( ( data[2] << 6 ) | ( data[3] >> 2 ) ) & 0x3FF; };
	unsigned int GetY2( ) { return ( ( data[3] << 8 ) | ( data[4] & 0xFF ) ) & 0x3FF; };
	unsigned int GetSX( ) { return ( data[5] >> 4 ) & 0xF; };
	unsigned int GetSY( ) { return ( data[5] ) & 0xF; };

	unsigned char data[6];
};
struct RoPos
{
	unsigned int GetX( ) { return ( ( data[0] << 2 ) | ( data[1] >> 6 ) ) & 0x3FF; };
	unsigned int GetY( ) { return ( ( data[2] >> 4 ) | ( data[1] << 4 ) ) & 0x3FF; };
	unsigned int GetDir( ) { return data[2] & 0xF; };

	void Set( int x, int y, int dir )
	{
		data[0] = x >> 2;
		data[1] = ( x << 6 ) | ( ( y >> 4 ) & 0x3F );
		data[2] = ( y << 4 ) | ( dir & 0xF );
	};

	unsigned char data[3];
};




struct RoPakLogin
{
	int server_ver;
	char username[0x18];
	char password[0x18];
	char server_type;
};
struct RoPakServersHead
{
	unsigned int session_id;
	unsigned int account_id;
	unsigned int session_id2;
	char __unk1[30];
	char gender;
};
struct RoPakServersEntry
{
	unsigned char ip[4];
	short port;
	char name[20];
	int usercount;
	short __unk1;
};



struct RoPakAvatarConnect
{
	unsigned int account_id;
	unsigned int session_id;
	unsigned int session_id2;
	short __unk1;
	char gender;
};
struct RoPakAvatarEntry
{
	unsigned int character_id;
	unsigned int experience_base;
	unsigned int zenny;
	unsigned int experience_job;
	unsigned int level_job;
	unsigned int opt1;
	unsigned int opt2;
	unsigned int option;
	unsigned int karma;
	unsigned int manner;
	unsigned short status_points;
	unsigned int hp_current;
	unsigned int hp_max;
	unsigned short sp_current;
	unsigned short sp_max;
	unsigned short walk_speed;
	unsigned short job_id;
	unsigned short hair_style;
	unsigned short __unk1;
	unsigned short level_base;
	unsigned short skill_point;
	unsigned short equip_headgear_low;
	unsigned short equip_shield;
	unsigned short equip_headgear_top;
	unsigned short equip_headgear_mid;
	unsigned short hair_color;
	unsigned short clothes_color;
	char name[24];
	unsigned char stat_str;
	unsigned char stat_agi;
	unsigned char stat_vit;
	unsigned char stat_int;
	unsigned char stat_dex;
	unsigned char stat_luk;
	unsigned short slot_id;
	unsigned short rename;
};
struct RoPakZoneConnectInfo
{
	unsigned int char_id;
	char mapname[16];
	unsigned char ip[4];
	unsigned short port;
};

struct RoPakZoneConnect
{
	unsigned int account_id;
	unsigned int character_id;
	unsigned int session_id;
	unsigned int tick;
	unsigned char gender;
	unsigned int __unk1;
};

struct RoPakMapReady
{
	unsigned int tick;
	RoPos pos;
	unsigned short __unk1;
};

struct RoPakUnitMove
{
	unsigned int id;
	RoMovePos pos;
	unsigned int tick;
};
struct RoPakUnitClear
{
	unsigned int id;
	unsigned char type;
};
struct RoPakTick
{
	unsigned short __unk1;
	unsigned int tick;
};


struct RoPakUnitSpawnIdle
{
	unsigned char type;
	unsigned int id;
	unsigned short speed;
	unsigned short opt1;
	unsigned short opt2;
	unsigned int option;
	unsigned short class_id;
	unsigned short hair_style;
	unsigned short weapon;
	unsigned short shield;
	unsigned short head_bottom;
	unsigned short head_top;
	unsigned short head_mid;
	unsigned short hair_color;
	unsigned short cloth_color;
	unsigned short head_dir;
	unsigned int guild_id;
	unsigned short emblem_id;
	unsigned short manner;
	unsigned int opt3;
	unsigned char karma;
	unsigned char gender;
	RoPos pos;
	unsigned char __unk1;
	unsigned char __unk2;
	unsigned char dead_sit;
	unsigned short level;
	unsigned short user_font;
	char name[24];
};
struct RoPakUnitSpawnMove
{
	unsigned char type;
	unsigned int id;
	unsigned short speed;
	unsigned short opt1;
	unsigned short opt2;
	unsigned int option;
	unsigned short class_id;
	unsigned short hair_style;
	unsigned short weapon;
	unsigned short shield;
	unsigned short head_bottom;
	unsigned int tick;
	unsigned short head_top;
	unsigned short head_mid;
	unsigned short hair_color;
	unsigned short cloth_color;
	unsigned short head_dir;
	unsigned int guild_id;
	unsigned short emblem_id;
	unsigned short manner;
	unsigned int opt3;
	unsigned char karma;
	unsigned char gender;
	RoMovePos pos;
	unsigned char __unk1;
	unsigned char __unk2;
	unsigned short level;
	unsigned short user_font;
	char name[24];
};

struct RoPakUpdateStatusL
{
	unsigned short type;
	unsigned int value;
};

struct RoPakMoveTo
{
	unsigned int __unk1;
	RoPos pos;
};

struct RoPakMapChange
{
	char name[ 16 ];
	unsigned short x;
	unsigned short y;
};

struct RoPakMoveOk
{
	unsigned int tick;
	RoMovePos pos;
};

struct RoPakClickNpc
{
	unsigned int id;
	unsigned char __unk1;
};

struct RoPakNpcDlgMes
{
	unsigned int id;
	char msg[ 100 ];
};

struct RoPakNpcDlgNext
{
	unsigned int id;
};

struct RoPakNpcDlgClose
{
	unsigned int id;
};

struct RoPakNpcNext
{
	unsigned int id;
};

struct RoPakNpcDlgMenu
{
	unsigned int id;
	char msg[ 100 ];
};

struct RoPakNpcOption
{
	unsigned int id;
	unsigned char selval;
};

struct RoGlobalMessage
{
	unsigned int id;
	char msg[ 100 ];
};

#pragma pack(pop)
#endif // RoPackets_h__

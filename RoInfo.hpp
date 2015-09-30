#ifndef RoInfo_hpp__
#define RoInfo_hpp__

#include <string>

enum STATUS_CODE
{
	SC_MOVESPEED = 0
};

enum UNIT_TYPE
{
	// eAthena
	UT_PLAYER = 0,
	UT_NOTHING,
	UT_ITEM,
	UT_SKILL,
	UT_UNKNOWN,
	UT_MOB,
	UT_NPC,
	UT_PET,
	UT_HOM,
	UT_MERC,

	// Internal
	UT_FAKENPC = 0x1000
};
extern const char* pcUnitTypes[];

enum CLEAR_TYPE
{
	CT_OUTSIGHT = 0,
	CT_DIED,
	CT_RESPAWNED,
	CT_TPLOGOUT
};
extern const char* pcClearTypes[];




struct GameUnit
{
	virtual ~GameUnit( ) { };

	UNIT_TYPE iType;
	unsigned int iId;
	std::string sName;

	unsigned int uiMoveSpeed;

	int iX;
	int iY;
};





#endif // RoInfo_hpp__
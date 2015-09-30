#include "Globals.hpp"

bool g_bRunning;

char pcAccount[ 24 ];
char pcPassword[ 24 ];
char pcAvatar[ 24 ];

unsigned int g_uiUnitId = 0;
unsigned char g_ucGender = 0;
unsigned int g_uiMoveSpeed = 0;
int g_iAvaPosX = 0;
int g_iAvaPosY = 0;

char g_pcMapName[ 24 ];

std::unordered_map<unsigned int, GameUnit*> g_rUnits;
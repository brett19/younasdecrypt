#ifndef Globals_hpp__
#define Globals_hpp__

#include <unordered_map>
#include "RoInfo.hpp"

static inline int FAST_ABS(int x) { return x < 0 ? -x : x; }

namespace MapData { struct MapCells; }

extern bool g_bRunning;

extern char pcAccount[ 24 ];
extern char pcPassword[ 24 ];
extern char pcAvatar[ 24 ];

extern unsigned int g_uiUnitId;
extern unsigned char g_ucGender;
extern unsigned int g_uiMoveSpeed;
extern int g_iAvaPosX;
extern int g_iAvaPosY;

extern char g_pcMapName[ 24 ];

extern std::unordered_map<unsigned int, GameUnit*> g_rUnits;

#endif //Globals_hpp__
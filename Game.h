#ifndef _GAME_H
#define _GAME_H

#ifndef _SYSTEM_H
#include "System.h"
#endif

// Game Data Structure
struct GameData : public SystemData
{};

class Game : public System
{
public:
	Game(const GameData& data);
	virtual ~Game();
};

#endif	// _GAME_H

#ifndef _ENGINE_H
#define _ENGINE_H

// C Runtime Header Files
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifndef _MAP_
#include <map>
#endif

// Additional Include Files
#ifndef _CONTEXT_H
#include "context.h"
#endif

// Forward Declarations
enum SystemType;
class System;
class Game;

// State Enumeration
enum EngineState
{
	Invalid, 
	Constructing, 
	Initializing, 
	Running, 
	ShuttingDown,  
	Destroying
};

class Engine
{
public:
	Engine();
	~Engine();

	// Methods
	// Game Engine Loop
	int RunLoop();

	// Memebers
	static EngineState GetEngineState() { return m_EngineState; }

private:
	// Methods
	// Implement Basic Functionalities
	int Initialize();
	int Update(Context& context);
	int Draw(Context& context);
	int ShutDown();

	int AddSystem(System* psys);	// add core system to the engine
	template<typename T>			// retrieve core system from the engine
	T* GetSystem(SystemType systype)
	{
		T* psys = static_cast<T*>(m_mapSystems[systype]);
		if (!psys)
		{
			//Logger::Warning("System is not valid!");
			return nullptr;
		}
		else
		{
			return psys;
		}
	}
	Game* CreateGame();				// create game instance

	// Members
	static EngineState m_EngineState;
	std::map<SystemType, System*> m_mapSystems;
};

#endif	// _ENGINE_H

#ifndef _ENGINE_H
#define _ENGINE_H

// C Runtime Header Files
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifndef _MAP_
#include <map>
#endif

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

	//void* operator new(size_t size);
	//void operator delete(void* pDelete);

	// Game Engine Loop
	int RunLoop();

	static EngineState GetEngineState() { return m_EngineState; }

private:
	static EngineState m_EngineState;
	std::map<SystemType, System*> m_mapSystem;

	// Implement Basic Functionalities
	int Initialize();
	int Update(const Context& context);
	int Draw(const Context& context);
	int ShutDown();

	// Add Core System To The Engine
	int AddSystem(System* psys);
	// Retrieve Core System From The Engine
	template<typename T>
	T* GetSystem(SystemType systype)
	{
		T* psys = static_cast<T*>(m_mapSystem[systype]);
		if (!psys)
		{
			//Logger::Warning("System is not valid!");
			return nullptr;
		}
		else { return psys; }
	}
	// Create Game Instance
	Game* CreateGame();
};

#endif	// _ENGINE_H
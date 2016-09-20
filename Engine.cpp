#include "Engine.h"

// Additional Include Files
#ifndef _STRING_H
#include "string.h"
#endif
#ifndef _DELETEMACROS_H
#include "deletemacros.h"
#endif
#ifndef _SYSTEM_H
#include "System.h"
#endif
#ifndef _GAME_H
#include "Game.h"
#endif
#ifndef _WINDOW_H
#include "Window.h"
#endif


// Engine State Variable
EngineState Engine::m_EngineState = EngineState::Invalid;


// Constructor & destructor
Engine::Engine()
{
	m_EngineState = EngineState::Constructing;
}
Engine::~Engine()
{
	m_EngineState = EngineState::Destroying;
}

// Public Methods
int Engine::RunLoop()
{
	if (!this->Initialize())
		return false;

	Context context;

	srand(GetTickCount());

	MSG msg = {};

	m_EngineState = EngineState::Running;
	
	while (msg.message != WM_QUIT && m_EngineState == EngineState::Running)
	{
		// Check Resize

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		this->Update(context);
		this->Draw(context);
	}

	//Logger::Info("Ending the program");
	//Logger::WriteLogFile();

	if (!this->ShutDown())
		return false;
	
	return msg.wParam;
}

// Private Methods
int Engine::Initialize()
{
	m_EngineState = EngineState::Initializing;

	Game* game = CreateGame();
	
	if (!game)
		return false;
	
	// Add Systems
	if (!AddSystem(new Window(WindowData(640, 480))))
		return false;

	// Initialize Systems
	if (!m_mapSystems[SystemType::Sys_Window]->Initialize())
		return false;

	return true;
}
int Engine::Update(Context& context)
{
	for (std::pair<SystemType, System*> pSys : m_mapSystems)
	{
		if (!pSys.second)
			continue;

		pSys.second->Update(context);
	}

	return true;
}
int Engine::Draw(Context& context)
{
	return true;
}
int Engine::ShutDown()
{
	m_EngineState = EngineState::ShuttingDown;

	for (std::pair<SystemType, System*> psys : m_mapSystems)
	{
		/*if (!psys.second->ShutDown())
		{
			//Logger::Warning("Failed to shutdown system: " + psys->GetSystemType());
			continue;
		}*/

		SafeDelete(psys.second);
	}

	return true;
}

int Engine::AddSystem(System* psys)
{
	auto element = m_mapSystems.insert(std::make_pair(psys->GetType(), psys));
	
	if (!element.second)
		return false;
	
	return true;
}
Game* Engine::CreateGame()
{
	if (!AddSystem(new Game(GameData())))
		return nullptr;

	Game* game = GetSystem<Game>(SystemType::Sys_Game);

	if (!game)
		return nullptr;

	//if (!game->Initialize())
	//	return nullptr;
	
	return game;
}

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


EngineState Engine::m_EngineState = EngineState::Invalid;


Engine::Engine()
{
	m_EngineState = EngineState::Constructing;
}
Engine::~Engine()
{
	m_EngineState = EngineState::Destroying;
}


// PUBLIC METHODS
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


// PRIVATE METHODS
int Engine::Initialize()
{
	m_EngineState = EngineState::Initializing;

	Game* game = CreateGame();
	
	if (!game)
		return false;
	
	// Add Systems HERE

	return true;
}

int Engine::Update(const Context& context)
{
	return true;
}

int Engine::Draw(const Context& context)
{
	return true;
}

int Engine::ShutDown()
{
	m_EngineState = EngineState::ShuttingDown;

	for (std::pair<SystemType, System*> psys : m_mapSystem)
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
	auto element = m_mapSystem.insert(std::make_pair(psys->GetType(), psys));
	
	if (element.second)
		return true;
	
	return false;
}

Game* Engine::CreateGame()
{
	if (!AddSystem(new Game(GameData())))
		return nullptr;

	Game* game = GetSystem<Game>(SystemType::Sys_Game);

	if (!game)
		return nullptr;

	/*if (!game->Initialize())
		return nullptr;*/
	
	return game;
}

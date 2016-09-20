#ifndef _SYSTEM_H
#define _SYSTEM_H

#ifndef _UBOJECT_H
#include "UObject.h"
#endif

// Addtional Include Files
#ifndef _CONTEXT_H
#include "context.h"
#endif

// System Type Enumeration
enum SystemType
{
	Sys_Invalid, 
	Sys_Window, 
	Sys_Game, 
	Sys_Input,
	Sys_Graphics,
	Sys_GameTimer, 
	Sys_MAX
};
// System Data Structure
struct SystemData
{
	SystemData();
	SystemData(const SystemType& type);
	SystemType systemType;
};

class System : public UObject
{
	friend class Engine;

public:
	SystemType GetType() { return m_SystemType; }

protected:
	System(const SystemData& data);
	virtual ~System();

	SystemType m_SystemType;

	virtual bool Initialize() { return UObject::Initialize(); }
	virtual bool Update(Context& context) { return UObject::Update(context); }
	virtual bool ShutDown() { return UObject::ShutDown(); }
};

#endif	// _SYSTEM_H

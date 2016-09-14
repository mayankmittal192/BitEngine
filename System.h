#ifndef _SYSTEM_H
#define _SYSTEM_H

#ifndef _UBOJECT_H
#include "UObject.h"
#endif

// System Type Enumeration
enum SystemType
{
	Sys_Window, 
	Sys_Game, 
	Sys_Input,
	Sys_Graphics,
	Sys_GameTimer
};

// System Data Structure
struct SystemData
{};

class System : public UObject
{
public:
	System();
	virtual ~System();

	SystemType GetType() { return m_SystemType; }

protected:
	SystemType m_SystemType;
};

#endif

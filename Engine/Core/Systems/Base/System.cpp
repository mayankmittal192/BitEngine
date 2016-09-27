#include "System.h"

// System Data Structure
SystemData::SystemData()
	: systemType(SystemType::Sys_Invalid)
{}
SystemData::SystemData(const SystemType& type)
	: systemType(type)
{}

// Constructor & Destructor
System::System(const SystemData& data)
	: UObject(), m_SystemType(data.systemType)
{}
System::~System()
{}

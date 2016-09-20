#include "UObject.h"


// Object Quantity Tracking Variable
int UObject::m_objectAmount = 0;


// Constructor & Destructor
UObject::UObject()
	: m_Name(_T("")), m_ID(m_objectAmount),
	m_bIsInitialized(false), m_bIsPostInitialized(false),
	m_bIsContentLoaded(false), m_bIsPostContentLoaded(false),
	m_bIsDestroyed(false), m_bCanDraw(true), m_bCanTick(true)
{
	m_Name = _T("Object_") + TOSTRING(m_ID);
	m_objectAmount += 1;
}
UObject::UObject(const std::tstring& name)
	: m_Name(name), m_ID(m_objectAmount), 
	m_bIsInitialized(false), m_bIsPostInitialized(false), 
	m_bIsContentLoaded(false), m_bIsPostContentLoaded(false), 
	m_bIsDestroyed(false), m_bCanDraw(true), m_bCanTick(true)
{
	m_objectAmount += 1;
}
UObject::~UObject()
{
	m_objectAmount -= 1;
}

// Public Methods
void UObject::Reset()
{}

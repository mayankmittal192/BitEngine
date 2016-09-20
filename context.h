#ifndef _CONTEXT_H
#define _CONTEXT_H

// Forward Declarations
class Window;

// Defines
#define WINDOW context.pWnd

// Context Structure
struct Context
{
	float dTime;

	Window* pWnd;
};

#endif	// _CONTEXT_H

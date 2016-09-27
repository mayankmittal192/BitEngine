#ifndef _DELETEMACROS_H
#define _DELETEMACROS_H

// Defines
#define SafeRelease(x) { if(x) { x->Release(); x = nullptr; } }		// Macro For Releasing COM Objects
#define SafeDelete(x) { if(x) { delete x; x = nullptr; } }			// Macro For Deleting Objects

#endif	// _DELETEMACROS_H

#ifndef _WINDOW_H
#define _WINDOW_H

#ifndef _SYSTEM_H
#include "System.h"
#endif

// C Runtime Header Files
#ifndef _WINDOWS_
#include <Windows.h>
#endif

// Additional Include Files
#ifndef _CONTEXT_H
#include "context.h"
#endif
#ifndef _STRING_H
#include "string.h"
#endif

// Window Data Structure
struct WindowData : public SystemData
{
	WindowData();
	WindowData(int w, int h, const std::tstring& title = _T("BitEngine v:1.0"), int b = 32, bool fs = false);

	int width;
	int height;
	int bits;
	const std::tstring& windowTitle;
	bool fullscreen;
};
// Resize Data Structure
struct ResizeData
{
	ResizeData();
	ResizeData(bool resize, int nw, int nh);

	bool mustResize;
	int newWidth;
	int newHeight;
};

class Window : public System
{
	friend class Engine;

public:
	int GetWidth();
	int GetHeight();

	HWND GetWindowHandle();
	HDC GetDeviceContext();
	HINSTANCE GetInstance();
	
	ResizeData& GetResizeData() { return m_ResizeData; }
	
	LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	// Methods
	// Constructor & Destructor
	Window(const WindowData& data);
	virtual ~Window(void);

	// Overridable Methods
	virtual bool Initialize();
	virtual bool Update(Context& context);
	virtual bool ShutDown();

private:
	// Methods
	bool centerWindow();

	// Members
	HWND m_hWindow;
	HDC m_hDC;
	HINSTANCE m_hInst;

	int m_Width;
	int m_Height;
	int m_Bits;
	
	std::tstring m_Title;
	bool m_bFullScreen;
	
	ResizeData m_ResizeData;
};

#endif	// _WINDOW_H

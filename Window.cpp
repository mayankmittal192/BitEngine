#include "Window.h"

// Additional Include Files
//#include "MemoryManager.h"
//#include "Logger.h"


// Window Data Structure
WindowData::WindowData()
	: SystemData(SystemType::Sys_Invalid),
	width(-1), height(-1), bits(-1),
	windowTitle(_T("")), fullscreen(false)
{}
WindowData::WindowData(int w, int h, const std::tstring& title, int b, bool fs)
	: SystemData(SystemType::Sys_Window),
	width(w), height(h), bits(b),
	windowTitle(title), fullscreen(fs)
{}

// Resize Data Structure
ResizeData::ResizeData()
	: mustResize(false), newWidth(-1), newHeight(-1)
{}
ResizeData::ResizeData(bool resize, int nw, int nh)
	: mustResize(resize), newWidth(nw), newHeight(nh)
{}

// Constructor & destructor
Window::Window(const WindowData& data)
	: System(data), m_hWindow(NULL), m_hDC(NULL),
	m_hInst(NULL), m_Width(data.width), m_Height(data.height), m_Bits(data.bits),
	m_bFullScreen(data.fullscreen), m_Title(data.windowTitle), m_ResizeData()
{}
Window::~Window()
{}

#pragma region New & Delete Operator
//void* Window::operator new(size_t size)
//{
//	//return MEMORYMANAGER->Alloc(size);
//}
//void Window::operator delete(void* pdelete)
//{
//	//MEMORYMANAGER->Free(pdelete);
//}
#pragma endregion

// Class Window
// Declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		// if the message is WM_CREATE, then lParam contains a pointer to a CREATESTRUCT
		// the CREATESTRUCT contains the "this" pointer from the CreateWindow method
		// the "this" pointer from our app is stored in the createstruct pcs->lpCreateParams
		CREATESTRUCT *pCS = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pCS->lpCreateParams);
	}
	else
	{
		// retrieve the stored "this" pointer
		Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (pWindow)
			return pWindow->HandleEvent(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE: 
		{
			if (!HIWORD(wParam))
				this->Activate();
			else
				this->DeActivate();
			break;
		}

		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			m_ResizeData.mustResize = true;
			m_ResizeData.newWidth = width;
			m_ResizeData.newHeight = height;
			break;
		}

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hWnd, NULL, false);
			break;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	return 0;
}

// Private Methods
bool Window::Initialize()
{
	System::Initialize();
	
	unsigned int pixelFormat;			// holds the result after searching for a match

	DWORD dwExStyle;					// window extended style
	DWORD dwStyle;						// window style

	RECT wndRect;						// grabs rectangle upper left / lower right values
	wndRect.top = long(0);				// set top value to 0
	wndRect.left = long(0);				// set left value to 0 
	wndRect.right = long(m_Width);		// set right value to requested width
	wndRect.bottom = long(m_Height);	// set bottom value to requested height

	WNDCLASS wndClass;					// windows class structure
	std::tstring className = m_Title;	// windows title

	wndClass.style = CS_DBLCLKS | CS_DROPSHADOW | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = NULL;
	wndClass.cbWndExtra = NULL;
	wndClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className.c_str();

	// Calculate the window size and position based upon the game size
	// int iwindowWidth = m_Width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	// int iWindowHeight = m_Height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	// if (wndClass.lpszMenuName != NULL) iWindowHeight += GetSystemMetrics(SM_CYMENU);
	// int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth / 2;
	// int iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight / 2;

	if (!RegisterClass(&wndClass))
	{
		// Logger::Log(_T("Failed To Register Window"), LOGTYPE_ERROR, true);
		return false;
	}

	if (m_bFullScreen)
	{
		DEVMODE dmScreenSettings;									// device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));		// make sure memory is clear
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// size of the devmode structure
		dmScreenSettings.dmPelsWidth = m_Width;						// selected screen width
		dmScreenSettings.dmPelsHeight = m_Height;					// selected screen width
		dmScreenSettings.dmBitsPerPel = m_Bits;						// selected bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results
		// NOTE: CDS_FULLSCREEN Gets Rid Of Start bar
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Use Windowed Mode
			//Logger::Log(_T("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card"), LOGTYPE_WARNING, true);

			dwExStyle = WS_EX_APPWINDOW;	// window extended style
			dwStyle = WS_POPUP;				// window style
			//ShowCursor(false);			// hide mouse pointer (optional)
		}
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// window extended style
		dwStyle = WS_OVERLAPPEDWINDOW;							// window style
	}

	AdjustWindowRectEx(&wndRect, dwStyle, FALSE, dwExStyle);	// adjust window to true requested size

	if (!(m_hWindow = CreateWindowEx(
		dwExStyle, className.c_str(), className.c_str(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0,
		wndRect.right - wndRect.left,	// calculate window width
		wndRect.bottom - wndRect.top,	// calculate window height
		NULL, NULL, m_hInst, this)))
	{
		//Logger::Log(_T("Failed To Create Window Handle"), LOGTYPE_ERROR, true);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd =				// pfd tells window how we want things to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// size of this pixel format descriptor
		1,											// version number
		PFD_DRAW_TO_WINDOW |						// format must support window
		PFD_SUPPORT_OPENGL |						// format must support openGL
		PFD_DOUBLEBUFFER,							// format must support double buffering
		PFD_TYPE_RGBA,								// request an RGBA format
		m_Bits,										// select our color depth
		0, 0, 0, 0, 0, 0,							// color bits ignored
		0,											// no alpha buffer
		0,											// shift bit ignored
		0,											// no accumulation buffer
		0, 0, 0, 0,									// accumulation bits ignored
		16,											// 16 bit z-buffer (depth buffer)
		0,											// no stencil buffer
		0,											// no auxilliary buffer
		PFD_MAIN_PLANE,								// main drawing layer
		0,											// reserved
		0, 0, 0										// layer masks ignored
	};

	if (!(m_hDC = GetDC(m_hWindow)))
	{
		//Logger::Log(_T("Can Not Create Device Context"), LOGTYPE_ERROR, true);
		return false;
	}
	if (!(pixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
	{
		//Logger::Log(_T("Can Not Find A Suitable Pixel Format"), LOGTYPE_ERROR, true);
		return false;
	}
	if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
	{
		//Logger::Log(_T("Can Not Set The Pixel Format"), LOGTYPE_ERROR, true);
		return false;
	}

	ShowWindow(m_hWindow, SW_SHOW);
	SetForegroundWindow(m_hWindow);
	SetFocus(m_hWindow);

	m_ResizeData.mustResize = true;
	m_ResizeData.newWidth = m_Width;
	m_ResizeData.newHeight = m_Height;

	//if (!m_bFullScreen && !this->centerWindow())
	//{
	//	//Logger::Log(_T("Failed To Center Window"), LOGTYPE_ERROR, true);
	//	return false;
	//}

	// Disable Closing Button Debug Window
	HWND hConsoleWnd = GetConsoleWindow();
	if (hConsoleWnd != NULL)
	{
		HMENU hConsoleMenu = GetSystemMenu(hConsoleWnd, FALSE);
		if (hConsoleMenu != 0)
		{
			BOOL bRect = RemoveMenu(hConsoleMenu, SC_CLOSE, MF_BYCOMMAND);
			if (!bRect)
			{
				//Logger::Log(_T("Failed To Remove Close Button"), LOGTYPE_ERROR, true);
				return false;
			}
		}
	}

	//Logger::Log(_T("Window Is Initialized"), LOGTYPE_INFO, false);
	
	// Success
	return true;
}
bool Window::Update(Context& context)
{
	//System::Update(context);

	if (WINDOW != this)
		WINDOW = this;

	return true;
}
bool Window::ShutDown()
{
	//System::ShutDown();

	if (m_bFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}

	// Are We Able To Release DC
	// Set DC To NULL
	if (m_hDC && !ReleaseDC(m_hWindow, m_hDC))
	{
		//Logger::Log(_T("Release DC Failed"));
		m_hDC = NULL;
		return false;
	}
	// Are We Able To Destroy Window
	// Set Window To NULL
	if (m_hWindow && !DestroyWindow(m_hWindow))
	{
		//Logger::Log(_T("Could Not Destroy The Window"));
		m_hWindow = NULL;
		return false;
	}
	// Are We Able To Unregister Class
	// Set Instance To NULL
	if (!UnregisterClass(m_Title.c_str(), m_hInst))
	{
		//Logger::Log(_T("Could Not Unregister Class"));
		m_hInst = NULL;
		return false;
	}

	return true;
}


#include <windows.h>

#include "Gut.h"
#include "GUtWin32.h"

static HWND g_hWnd = NULL;
static HINSTANCE g_hInstance = NULL;
static HANDLE g_ThreadHandle = NULL;
static unsigned long g_ThreadID = 0;

static int g_iWindowPosX = 100;
static int g_iWindowPosY = 100;
static int g_iWindowWidth = 512;
static int g_iWindowHeight = 512;

void GutSetWindowHandleWin32(HWND hWnd)
{
	g_hWnd = hWnd;
}

HWND GutGetWindowHandleWin32(void)
{
	return g_hWnd;
}

HINSTANCE GutGetWindowInstanceWin32(void)
{
	return g_hInstance;
}

void GutGetWindowSize(int &w, int &h)
{
	w = g_iWindowWidth;
	h = g_iWindowHeight;
}

void GutGetMouseState(int &x, int &y, int button[3])
{
	POINT p;

	GetCursorPos(&p);
	ScreenToClient(g_hWnd, &p);
	
	x = p.x;
	y = p.y;
	
	button[0] = GetKeyState(VK_LBUTTON) & 0x80 ? 1 : 0;
	button[1] = GetKeyState(VK_MBUTTON) & 0x80 ? 1 : 0;
	button[2] = GetKeyState(VK_RBUTTON) & 0x80 ? 1 : 0;
}

// `�����T���B�z�禡`
static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// `�ˬd�T������`
	switch (message)
	{
		case WM_DESTROY:
		{
			// `�p�G�����Q�����F, �N�D�ʵo�X�{���ӵ������T��.`
			PostQuitMessage(0);
			break;
		}

		case WM_SIZE: // `�����j�p�Q����`
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			g_iWindowWidth = w;
			g_iWindowHeight = h;
			if ( g_GutCallBack.OnSize && GutGetGraphicsDeviceType()!=GUT_UNKNOWN ) 
			{
				// `���]�w�����j�p���ܪ��B�z�禡����, �N�h�I�s��.`
				g_GutCallBack.OnSize(w, h);
			}
			break;
		}

		case WM_PAINT: // `�����ݭn���e`
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			if ( g_GutCallBack.OnPaint )
			{
				// `���]�w��ø�T���B�z�禡����, �N�h�I�s��.`
				g_GutCallBack.OnPaint();
			}
			break;
		}

		default: // `�䥦�T���������浹���ت��T���B�z�禡�h��`
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}

	return 0;
} // WndProc

void GutCloseWindow(void)
{
	PostQuitMessage(0);
}

// `�ˬdWindows�@�~�t�ζǨӪ��T��`
bool GutProcessMessage(void)
{
	MSG	msg;
	// `�ݬݦ��S������T��`
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		// `���T������, �N�⥦�e���T���B�z�禡.`
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if ( msg.message==WM_QUIT )
		{
			// `���������������T���|�Ǧ^false�ӵ����j��.`
			return false;
		}
	}

	return true;
}

bool GutFullScreen(int width, int height)
{
	char *title = "fullscreen";

	DEVMODE dmScreenSettings;								// Device Mode
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
	dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
	dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
	{
		// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
		return false;
	}


	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));

	// `�]�w�������A`
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	// `�]�w�T���B�z�禡��WndProc`
	window_class.lpfnWndProc = WndProc;
	// `���o�ثe���檺�{��`
	window_class.hInstance = GetModuleHandle(NULL);
	// `�ϥΤ��w���ƹ����`
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	// `�I���]���¦�, ��꦳�S���]���S�t.`
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// `���w�@�ӵ��U�����ɨϥΪ��W�r`
	window_class.lpszClassName = title;
	// `��@�~�t�ε��U�@�ӷs������`
	if (RegisterClass(&window_class) == 0)
		return 0;
	
	DWORD dwExStyle=WS_EX_APPWINDOW;	// Window Extended Style
	DWORD dwStyle=WS_POPUP;				// Windows Style
	ShowCursor(FALSE);					// Hide Mouse Pointer

	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	g_iWindowPosX = g_iWindowPosY = 0;
	g_iWindowWidth = width;
	g_iWindowHeight = height;

	HWND window_handle = CreateWindowEx(	
		dwExStyle,
		"fullscreen",
		title,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right-WindowRect.left,
		WindowRect.bottom-WindowRect.top,
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;

	// `�O���U�ΨӥN�������ثe����{��������, �H��i�H�A�Ψ�.`
	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;
	// `��ܵ���`
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);
	SetForegroundWindow(window_handle);	
	SetFocus(window_handle);
	
	return true;
}


// `�}�ҵ���`
// `x, y = �������W������m`
// `width, height = �����j�p`
// `title = �������D�����ܪ���r`
bool GutCreateWindow(int x, int y, int width, int height, const char *title)
{
	static bool registered = false;

	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));

	// `�]�w�������A`
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	// `�]�w�T���B�z�禡��WndProc`
	window_class.lpfnWndProc = WndProc;
	// `���o�ثe���檺�{��`
	window_class.hInstance = GetModuleHandle(NULL);
	// `�ϥΤ��w���ƹ����`
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	// `�I���]���¦�, ��꦳�S���]���S�t.`
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// `���w�@�ӵ��U�����ɨϥΪ��W�r`
	window_class.lpszClassName = title;
	// `��@�~�t�ε��U�@�ӷs������`
	if ( !registered )
	{
		if (RegisterClass(&window_class) == 0)
			return false;
		registered = true;
	}

	DWORD window_style;

	if ( g_GutCallBack.OnSize )
	{
		// `�p�G���B�z�����Y�񪺰T��, �~�e�\�ƹ����ܵ����j�p.`
		window_style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		// `�p�G���S���B�z�����Y�񪺰T��, ���e�\�ƹ����ܵ����j�p.`
		window_style = WS_BORDER | WS_SYSMENU;
	}

	if ( x<0 || y<0 )
	{
		// �M�Τ��w��
		x = g_iWindowPosX;
		y = g_iWindowPosY;
	}

	if ( width<0 || height < 0 )
	{
		// �M�Τ��w��
		width = g_iWindowWidth;
		height = g_iWindowHeight;
	}

	// `�p���������u�|�������I��, ���n�ɭn�W�[�����j�p.`
	RECT window_rect;
	SetRect(&window_rect, x, y, x+width, y+height);
	AdjustWindowRect(&window_rect, window_style, FALSE);
	
	g_iWindowPosX = x;
	g_iWindowPosY = y;
	g_iWindowWidth = width;
	g_iWindowHeight = height;

	// `�ǳƤu�@����, �}�Ҥ@�ӵ���.`
	HWND window_handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		title,
		title,
		window_style,
		window_rect.left,	// x
		window_rect.top,	// y
		window_rect.right - window_rect.left,	// width
		window_rect.bottom - window_rect.top,	// height
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;

	// `�O���U�ΨӥN�������ثe����{��������, �H��i�H�A�Ψ�.`
	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;
	// `��ܵ���`
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);
	
	return true;
}

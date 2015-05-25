#include "GutWin32.h"
#include "GutOpenGL.h"

static HWND            g_hWnd      = NULL;
static HINSTANCE       g_hInstance = NULL;
static unsigned long   g_ThreadID  = 0;

static int g_iWindowPosX   = 100;
static int g_iWindowPosY   = 100;
static int g_iWindowWidth  = 512;
static int g_iWindowHeight = 512;


HWND GutGetWindowHandleWin32(void)
{
	return g_hWnd;
}
void GutSetWindowHandleWin32(HWND hWnd)
{
	g_hWnd = hWnd;
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

#ifdef _WIN32

static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			// 如果視窗被關掉了, 就主動發出程式該結束的訊息
			PostQuitMessage(0);
			break;
		}

//		case WM_SIZE: // 視窗大小被改變
//		{
//			int w = LOWORD(lParam);
//			int h = HIWORD(lParam);
//			g_iWindowWidth = w;
//			g_iWindowHeight = h;
//			if ( g_GutCallBack.OnSize && GutGetGraphicsDeviceType()!=GUT_UNKNOWN )
//			{
//				// 有設定視窗大小改變的處理函式的話, 就去呼叫它
//				g_GutCallBack.OnSize(w, h);
//			}
//			break;
//		}

//		case WM_PAINT: // 視窗需要重畫
//		{
//			PAINTSTRUCT ps;
//			BeginPaint(hwnd, &ps);
//			EndPaint(hwnd, &ps);
//			if ( g_GutCallBack.OnPaint )
//			{
//				// `有設定重繪訊息處理函式的話, 就去呼叫它.`
//				g_GutCallBack.OnPaint();
//			}
//			break;
//		}

		default: // 其它訊息都直接交給內建的訊息處理函式去做
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}

	return 0;
}
bool GutCreateWindow(int x, int y, int width, int height, std::string title)
{
	static bool registered = false;

	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));


	window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;   // 設定視窗型態
	window_class.lpfnWndProc   = WndProc;                              // 設定訊息處理函式為WndProc
	window_class.hInstance     = GetModuleHandle(NULL);                // 取得目前執行的程式
	window_class.hCursor       = LoadCursor(NULL, IDC_ARROW);          // 使用內定的滑鼠游標
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  // 背景設為黑色, 其實有沒有設都沒差
	window_class.lpszClassName = "SimpleWindow";                       // 給定一個註冊視窗時使用的名字


	// 跟作業系統註冊一個新的視窗
	if ( !registered )
	{
		if (RegisterClass(&window_class) == 0)
			return false;
		registered = true;
	}

	DWORD window_style;

//	if ( g_GutCallBack.OnSize )
	if ( 1 )
	{
		// 如果有處理視窗縮放的訊息, 才容許滑鼠改變視窗大小
		window_style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		// 如果有沒有處理視窗縮放的訊息, 不容許滑鼠改變視窗大小
		window_style = WS_BORDER | WS_SYSMENU;
	}

	if ( x<0 || y<0 )
	{
		// 套用內定值
		x = g_iWindowPosX;
		y = g_iWindowPosY;
	}

	if ( width<0 || height < 0 )
	{
		// 套用內定值
		width = g_iWindowWidth;
		height = g_iWindowHeight;
	}

	// 計算視窗的邊線會佔掉的點數, 必要時要增加視窗大小
	RECT window_rect;
	SetRect(&window_rect, x, y, x+width, y+height);
	AdjustWindowRect(&window_rect, window_style, FALSE);

	g_iWindowPosX = x;
	g_iWindowPosY = y;
	g_iWindowWidth = width;
	g_iWindowHeight = height;

	// 準備工作完成, 開啟一個視窗
	HWND window_handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		"SimpleWindow",
		title.c_str(),
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

	// 記錄下用來代表視窗跟目前執行程式的物件, 以後可以再用到
	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;


	GutInitGraphicsDeviceOpenGL();


	// 顯示視窗
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);

	return true;
}
bool GutProcessMessage()
{
	MSG	msg;

	// 看看有沒有任何訊息
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		// 有訊息的話, 就把它送給訊息處理函式
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if ( msg.message==WM_QUIT )
		{
			// 收到關閉視窗的訊息會傳回false來結束迴圈
			return false;
		}
	}

	return true;
}
void GutDestroyWindow(void)
{
	GutReleaseGraphicsDeviceOpenGL();
	DestroyWindow( g_hWnd );
	UnregisterClass( "SimpleWindow", g_hInstance );
}
#else
enum
{
	_NET_WM_STATE_REMOVE=0,
	_NET_WM_STATE_ADD,
	_NET_WM_STATE_TOGGLE,
};
struct AtomData
{
	Atom        protocols;
	Atom        delete_window;
};
AtomData        mAtom;
bool GutProcessMessage()
{
	XEvent			msg;

	XNextEvent(g_hInstance,&msg);

	switch(msg.type)
	{
		case Expose:
			glXSwapBuffers(g_hInstance,g_hWnd.window);
			break;

		case ClientMessage:
			if( mAtom.protocols     ==      msg.xclient.message_type &&
			    mAtom.delete_window ==(Atom)msg.xclient.data.l[0] )
			{
				return false;
			}
			break;

		default:
			break;
	}

	return true;
}
void GutDestroyWindow(void)
{
	glXMakeCurrent(g_hInstance,None,NULL);
	glXDestroyContext(g_hInstance,g_hWnd.context);
	XDestroyWindow(g_hInstance,g_hWnd.window);
	XCloseDisplay(g_hInstance);
}
static void SetTitle(std::string str)
{
	char   *_str=const_cast<char*>(str.c_str());

	XTextProperty		title;

	XLockDisplay(g_hInstance);

	int     result=Xutf8TextListToTextProperty( g_hInstance,
	                                            &_str,
	                                            1,
	                                            XUTF8StringStyle,
	                                            &title
	                                            );

	XUnlockDisplay(g_hInstance);

	if(result == Success)
	{
		XSetWMName(g_hInstance, g_hWnd.window, &title);
		XSetWMIconName(g_hInstance,g_hWnd.window,&title);
		XFree(title.value);
	}
	else
	{
		XStoreName(g_hInstance,g_hWnd.window,"utf-8 format title could't print");
	}
}
//xy還沒起作用，沒有一個範例需要鎖定視窗位置，所以留著以後實作沒關係
bool GutCreateWindow(int x, int y, int width, int height, std::string title)
{
	g_iWindowWidth =width;
	g_iWindowHeight=height;

	Window                  root;
	XVisualInfo*            graphInfo;
	XSetWindowAttributes    attributes;

	g_hInstance=XOpenDisplay(0);

	root=DefaultRootWindow(g_hInstance);

	GLint	att[]={GLX_RGBA,GLX_DEPTH_SIZE,24,GLX_DOUBLEBUFFER,None};

	graphInfo=glXChooseVisual(g_hInstance,0,att);

	attributes.colormap=XCreateColormap(g_hInstance,root,graphInfo->visual,AllocNone);
	attributes.override_redirect=0;

	g_hWnd.window=XCreateWindow( g_hInstance,root,0,0,
	                             g_iWindowWidth,
	                             g_iWindowHeight,
	                             0,graphInfo->depth,InputOutput,graphInfo->visual,
	                             CWColormap|CWEventMask|CWOverrideRedirect,
	                             &attributes
	                             );

	XMapWindow(g_hInstance,g_hWnd.window);
	SetTitle(title.c_str());

	g_hWnd.context=glXCreateContext(g_hInstance,graphInfo,0,GL_TRUE);
	glXMakeCurrent(g_hInstance,g_hWnd.window,g_hWnd.context);

	mAtom.protocols     = XInternAtom(g_hInstance,"WM_PROTOCOLS"    ,False);
	mAtom.delete_window = XInternAtom(g_hInstance,"WM_DELETE_WINDOW",False);
	XSetWMProtocols(g_hInstance,g_hWnd.window,&mAtom.delete_window,1);

	GutInitGraphicsDeviceOpenGL();

	XFree(graphInfo);

	return (bool)1;
}
#endif

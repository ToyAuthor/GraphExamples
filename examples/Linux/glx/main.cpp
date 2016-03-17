/*
 * 非常簡單的 OpenGL 範例
 * Linux 下需要 glx 執行 XWindow 跟 OpenGL 之間的銜接工作
 */


#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <GL/glx.h>    // 已包含<X11/Xlib.h>跟<GL/gl.h>了
#include <GL/glu.h>


#define ERROR_HERE() cout<<"Error here:"<<__FILE__<<__LINE__<<endl

using std::cout;
using std::endl;


class CWindow
{
	public:

		enum Result
		{
			SUCCESS=0,
			FAILURE,
			QUIT,
			KEEP_RUN,
		};

		CWindow();
		~CWindow()
		{
			Destroy();
		}

		// 建立視窗
		enum Result Create( const char* title,  // 視窗標題
					        int         width,  // 視窗寬
					        int         height  // 視窗高
					        );

		void        Destroy();                  // 銷毀視窗
		enum Result PeekEvent();                // 讀取並處理系統送來的 event
		void        Refresh();                  // 更新畫面

	private:

		Display*                mDevice;        // 視窗畫面的handle
		Window                  mWindow;        // 視窗代號
		XVisualInfo*            mConfigGL;      // GLX的設置選項
		GLXContext              mDeviceGL;      // OpenGL畫面的handle
		XSetWindowAttributes    mWinConfig;     // 建立視窗時需要填的資料之一
};
int main()
{
	CWindow      win;

	if(CWindow::FAILURE == win.Create("OpenGL Window",800,600))
	{
		ERROR_HERE();
		return EXIT_FAILURE;
	}

	while(CWindow::KEEP_RUN == win.PeekEvent())
	{
		win.Refresh();
	}

	return EXIT_SUCCESS;
}
// 描繪一個彩色的長方形
static void DrawRect()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	glBegin(GL_QUADS);
	glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
	glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
	glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
	glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
	glEnd();
}
CWindow::CWindow():mDevice(0)
{
	memset(&mWinConfig,0,sizeof(XSetWindowAttributes));
	mWinConfig.event_mask = ExposureMask    |
	                        KeyPressMask    |
	                        KeyReleaseMask  |
	                        ButtonPressMask |
	                        ButtonReleaseMask;
}
void CWindow::Destroy()
{
	if(mDevice)
	{
		glXMakeCurrent(mDevice, None, NULL);
		glXDestroyContext(mDevice, mDeviceGL);
		XDestroyWindow(mDevice, mWindow);
		XCloseDisplay(mDevice);
		mDevice=0;
	}
}
enum CWindow::Result
CWindow::Create(const char* title,int width,int height)
{
	mDevice = XOpenDisplay(0);

	if(!mDevice)
	{
		ERROR_HERE();
		return FAILURE;
	}

	Window      root = DefaultRootWindow(mDevice);      //取得根視窗(桌面？)的代號

	GLint       att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	//這行可能會令程式送出很多警告跟你說有哪些OpenGL的API在mesa找不到
	mConfigGL = glXChooseVisual(mDevice, 0, att);

	if(!mConfigGL)
	{
		ERROR_HERE();
		return FAILURE;
	}

	mWinConfig.colormap = XCreateColormap(mDevice, root, mConfigGL->visual, AllocNone);
	mWinConfig.override_redirect = 0;

	mWindow = XCreateWindow( mDevice,
	                         root,
	                         0,
	                         0,
	                         width,
	                         height,
	                         0,
	                         mConfigGL->depth,
	                         InputOutput,
	                         mConfigGL->visual,
	                         CWColormap | CWEventMask | CWOverrideRedirect,
	                         &mWinConfig);


	XMapWindow(mDevice, mWindow);           // 顯示視窗
	XStoreName(mDevice, mWindow, title);    // 設定視窗標題

	mDeviceGL = glXCreateContext(mDevice, mConfigGL, 0, GL_TRUE);

	glXMakeCurrent(mDevice, mWindow, mDeviceGL);    // 將上面產生的最終成品給集結在一起了

	XFree(mConfigGL);

	return SUCCESS;
}
enum CWindow::Result
CWindow::PeekEvent()
{
	XEvent          msg;

	XNextEvent(mDevice, &msg);

	switch(msg.type)
	{
		case Expose:
			DrawRect();
			return KEEP_RUN;
		case KeyPress:
			cout<<"Keycode:"<<msg.xkey.keycode<<endl;   // 順便顯示一下被壓下的是key code是什麼
			return QUIT;
		default:
			return KEEP_RUN;
	}
}
void CWindow::Refresh()
{
	glXSwapBuffers(mDevice, mWindow);
}

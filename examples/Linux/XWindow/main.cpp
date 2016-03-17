/*
 * 一個極簡約的 XWindow 範例
 * 需要引入的lib也很少
 */


#include <stdlib.h>
#include <string>
#include <X11/Xlib.h>

using std::string;

int main(void)
{
	Display*    dis;      // 視窗畫面的handle
	Window      win;      // 視窗代號
	int         scr;      // 螢幕代號


	dis = XOpenDisplay(NULL);

	if(dis == NULL)
	{
		return EXIT_FAILURE;
	}

	scr = DefaultScreen(dis);

	win = XCreateSimpleWindow( dis,
	                           RootWindow(dis, scr),
	                           10, 10, 100, 100, 1,
	                           BlackPixel(dis, scr),
	                           WhitePixel(dis, scr));

	XSelectInput(dis, win, ExposureMask | KeyPressMask);

	XMapWindow(dis, win);   // 在螢幕上顯示視窗

	XEvent      msg;

	for(;;)
	{
		XNextEvent(dis, &msg);   // 取得系統送來的訊息

		switch(msg.type)
		{
			case Expose:
			{
				string      str("Hello World!");

				XFillRectangle( dis, win, DefaultGC(dis, scr), 20, 20, 10, 10);
				XDrawString(    dis, win, DefaultGC(dis, scr), 10, 50, str.c_str(), str.size());
				break;
			}
			case KeyPress:   // 這裡設置成按下任意鍵都可以令程式結束
			{
				XCloseDisplay(dis);
				return EXIT_SUCCESS;
			}
		}
	}

	return EXIT_FAILURE;
}

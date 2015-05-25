#ifndef _GUTILWIN32_
#define _GUTILWIN32_

#include "Gut.h"

HWND GutGetWindowHandleWin32();
void GutSetWindowHandleWin32(HWND hWnd);
HINSTANCE GutGetWindowInstanceWin32();
void GutGetWindowSize(int &w, int &h);
void GutDestroyWindow();                      // 跟 GutCreateWindow()是成對的
bool GutFullScreen(int width, int height);
bool GutProcessMessage();                     // 查看事件信箱有沒有新訊息，做相對應的處理

// 建立一個視窗
bool GutCreateWindow( int x, int y,           // 視窗位置
                      int width, int height,  // 視窗大小
                      std::string title       // 視窗標題
                      );

#endif//_GUTILWIN32_

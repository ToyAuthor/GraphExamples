#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "Gut.h"
#include "GutInput.h"
#include "GutTimer.h"

//#include "render_dx9.h"
//#include "render_dx10.h"
#include "render_opengl.h"
#include "render_data.h"

GutTimer g_Timer;
float g_fFrame_Time = 0.0f;

void GetUserInput(void)
{
	g_fFrame_Time = g_Timer.Stop();
	g_Timer.Restart();
	GutReadKeyboard();
	g_Control.Update(g_fFrame_Time, CGutUserControl::CONTROLLER_ROTATEOBJECT);
	//g_Control.Update(g_fFrame_Time, CGutUserControl::CONTROLLER_FPSCAMERA);
}

void KeyPressed_ADD(void)
{
	g_fZBias += 0.01f * g_fFrame_Time;
	printf("ZBias = %5.3f\r", g_fZBias);
}

void KeyPressed_MINUS(void)
{
	g_fZBias -= 0.01f * g_fFrame_Time;
	printf("ZBias = %5.3f\r", g_fZBias);
}

int main()
{
	// 使用OpenGL來繪圖
	char  device[16] = "opengl";

	void (*render)(void) = RenderFrameOpenGL;
	bool (*init_resource)(void) = InitResourceOpenGL;
	bool (*release_resource)(void) = ReleaseResourceOpenGL;
	void (*resize_func)(int width, int height) = ResizeWindowOpenGL;


	GutResizeFunc( resize_func );

	// 在(100,100)的位置開啟一個大小為(512x512)的視窗
	GutCreateWindow(100, 100, 512, 512, device);

	// 做OpenGL初始化
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	GutInputInit();
	GutRegisterKeyPressed(GUTKEY_ADD, KeyPressed_ADD);
	GutRegisterKeyPressed(GUTKEY_SUBTRACT, KeyPressed_MINUS);

	GutRegisterKeyPressed(GUTKEY_UP, KeyPressed_ADD);
	GutRegisterKeyPressed(GUTKEY_DOWN, KeyPressed_MINUS);

	g_Control.SetCamera(Vector4(5.0f, 0.0f, 5.0f), Vector4(0.0f), Vector4(0.0f, 1.0f, 0.0f) );

	g_Model.Load_ASCII("..\\..\\models\\teapot_set.gma");

	// 載入shader
	if ( !init_resource() )
	{
		release_resource();
		printf("Failed to load resources\n");
		exit(0);
	}

	// 主迴圈
	while( GutProcessMessage() )
	{
		GetUserInput();
		render();
	}

	// 卸載shader
	release_resource();

	// 關閉 OpenGL 繪圖裝置
	GutReleaseGraphicsDevice();

	return 0;
}

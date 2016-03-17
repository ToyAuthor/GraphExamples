#include <stdio.h>
//#include <conio.h>

#include "Gut.h"
#include "GutWin32.h"
//#include "GutInput.h"
#include "GutTimer.h"

#include "render_opengl.h"
#include "render_data.h"

GutTimer g_Timer;
float g_fFrame_Time = 0.0f;

void GetUserInput(void)
{
	g_fFrame_Time = g_Timer.Stop();
	g_Timer.Restart();
//	GutReadKeyboard();
//	g_Control.Update(g_fFrame_Time, CGutUserControl::CONTROLLER_ROTATEOBJECT);
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
//	ResizeWindowOpenGL;
//	GutResizeFunc( resize_func );


	// 在(100,100)的位置開啟一個大小為(512x512)的視窗
	GutCreateWindow(100, 100, 512, 512, "Shadow");


//	GutInputInit();
//	GutRegisterKeyPressed(GUTKEY_ADD, KeyPressed_ADD);
//	GutRegisterKeyPressed(GUTKEY_SUBTRACT, KeyPressed_MINUS);

//	GutRegisterKeyPressed(GUTKEY_UP, KeyPressed_ADD);
//	GutRegisterKeyPressed(GUTKEY_DOWN, KeyPressed_MINUS);

//	g_Control.SetCamera(Vector4(5.0f, 0.0f, 5.0f), Vector4(0.0f), Vector4(0.0f, 1.0f, 0.0f) );

	g_Model.Load_ASCII("..\\..\\models\\teapot_set.gma");

	// 載入shader
	if ( !InitResourceOpenGL() )
	{
		ReleaseResourceOpenGL();
		printf("Failed to load resources\n");
		return 1;
	}
	// 主迴圈
	while( GutProcessMessage() )
	{
//		GetUserInput();
		RenderFrameOpenGL();
	}

	ReleaseResourceOpenGL();

	GutDestroyWindow();

	return 0;
}

#include <stdlib.h>
#include "GutWin32.h"
#include "render_opengl.h"
#include "render_data.h"

/*
 * g_weight_table 的資料還無法使用
 * 必須經過下面這道計算才能用
 */
static void CountWeight()
{
	float       sum = 0.0f;
	const int   num_samples = KERNELSIZE;

	for(int i=0; i<num_samples; i++ )
	{
		sum += g_weight_table[i];
	}

	for(int i=0; i<num_samples; i++ )
	{
		g_weight_table[i] /= sum;
	}
}

int main()
{
	GutCreateWindow(100, 100, 512, 512, "HelloOpenGL");

	if ( !InitResourceOpenGL() )
	{
		ReleaseResourceOpenGL();
		printf("Failed to load resources\n");
		exit(0);
	}

	CountWeight();

	while( GutProcessMessage() )
	{
		RenderFrameOpenGL();
	}

	ReleaseResourceOpenGL();

	GutDestroyWindow();

	return 0;
}

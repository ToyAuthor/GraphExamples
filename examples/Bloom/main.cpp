#include <stdlib.h>
#include "GutWin32.h"
#include "render_opengl.h"
#include "render_data.h"


int main()
{
	glViewport(0, 0, 512, 512);
	GutCreateWindow(100, 100, 512, 512, "HelloOpenGL");


	if ( !InitResourceOpenGL() )
	{
		ReleaseResourceOpenGL();
		printf("Failed to load resources\n");
		exit(0);
	}

	int i;
	const int num_samples = KERNELSIZE;

	float sum = 0.0f;
	for ( i=0; i<num_samples; i++ )
	{
		sum += g_weight_table[i];
	}

	for ( i=0; i<num_samples; i++ )
	{
		g_weight_table[i] /= sum;
	}

	while( GutProcessMessage() )
	{
		RenderFrameOpenGL();
	}

	ReleaseResourceOpenGL();

	GutDestroyWindow();

	return 0;
}

#include <stdlib.h>
#include "GutWin32.h"
#include "render_opengl.h"
#include "render_data.h"


int main()
{
	GutCreateWindow(100, 100, 512, 512, "HelloOpenGL");


	if ( !InitResourceOpenGL() )
	{
		ReleaseResourceOpenGL();
		printf("Failed to load resources\n");
		exit(0);
	}

	while( GutProcessMessage() )
	{
		RenderFrameOpenGL();
	}

	ReleaseResourceOpenGL();

	GutDestroyWindow();

	return 0;
}

#include "GutWin32.h"

int main()
{
	GutCreateWindow(100, 100, 800, 600, "HelloOpenGL");


	while( GutProcessMessage() )
	{
		;
	}

	GutDestroyWindow();

	return 0;
}

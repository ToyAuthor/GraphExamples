#include <stdio.h>
#include "Gut.h"

void RenderFrameDX9(void);
void RenderFrameOpenGL(void);

void main(int argc, char *argv[])
{
	// ���w�ϥ�DirectX 9��ø��
	char *device = "dx9";

	if ( argc > 1 )
	{
		// �p�G�R�O�C�Ѽƫ��w��OpenGL, �N���OpenGL.
		if ( stricmp(argv[1], "opengl")==0 )
		{
			device = "opengl";
		}
	}

	// �b(100,100)����m�}�Ҥ@�Ӥj�p��(512x512)������
	GutCreateWindow(100, 100, 512, 512, device);

	// ��OpenGL��DirectX��l��
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	// �D�j��
	while( GutProcessMessage() )
	{
		if ( !strcmp(device, "dx9") )
		{
			RenderFrameDX9();
		}
		else
		{
			RenderFrameOpenGL();
		}
	}

	// ����OpenGL/DirectXø�ϸ˸m
	GutReleaseGraphicsDevice();
}

// �@��3D�y���I(x,y,z)����m
float vertex[] = {0.0f, 0.0f, 0.0f};

// �ϥ�DirectX 9���禡
void RenderFrameDX9(void)
{
	// ���oD3D�˸m.
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// �����������p��
    device->SetRenderState( D3DRS_LIGHTING, FALSE );
	// ��e���M���¦�
    device->Clear( 
		0, NULL,�@// �M����ӵe�� 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // �M���C���Z buffer 
		D3DCOLOR_ARGB(0, 0, 0, 0),  // �]�w�n���C��M���¦�
		1.0f, // �]�w�n��Z�ȲM��1, �]�N�O�����Y�̻�
		0 // �]�w�n��Stencil buffer�M��0, �b�o�S�t.
	);

	// �}�l�Uø�ϫ��O
	device->BeginScene(); 
	// �]�w��Ʈ榡
	device->SetFVF(D3DFVF_XYZ); 
	// �e�X�@���I
	device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vertex, 12); 
	// �ŧi�Ҧ���ø�ϫ��O���U���F
	device->EndScene();		
	
	// ��I��backbuffer���e���e�{�X��
    device->Present( NULL, NULL, NULL, NULL );
}

// �ϥ�OpenGL���禡
void RenderFrameOpenGL(void)
{
	// �M���e��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �e�@���I
	glBegin(GL_POINTS);
	glVertex3fv(vertex);
	glEnd();

	// ��I��backbuffer���e���e�{�X��
	GutSwapBuffersOpenGL();
}

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <string>
#include <stack>

#include "GutWin32.h"
#include "Gut.h"
//#include "GutDX9.h"
//#include "GutDX10.h"
#include "GutOpenGL.h"

GutCallBack g_GutCallBack;
static char g_szDeviceName[8];
GutEnum g_eDeviceType = GUT_UNKNOWN;

GutCallBack::GutCallBack(void)
{
	memset(this, 0, sizeof(GutCallBack));
}

void GutResizeFunc( void (*onsize)(int x, int y) )
{
	g_GutCallBack.OnSize = onsize;
}

void GutDisplayFunc( void (*onpaint)(void) )
{
	g_GutCallBack.OnPaint = onpaint;
}

void GutIdleFunc( void (*onidle)(void) )
{
	g_GutCallBack.OnIdle = onidle;
}

void GutOnCloseFunc( void (*onclose)(void) )
{
	g_GutCallBack.OnClose = onclose;
}

bool GutInitGraphicsDevice(const char *device)
{
	strcpy(g_szDeviceName, device);
	g_eDeviceType = GutGetGraphicsDeviceType(device);

//	switch(g_eDeviceType)
//	{
//	case GUT_OPENGL:
//		return GutInitGraphicsDeviceOpenGL();
//		break;
//	case GUT_DX9:
//		return GutInitGraphicsDeviceDX9();
//		break;
//	case GUT_DX10:
//	#ifdef _ENABLE_DX10_
//		return GutInitGraphicsDeviceDX10();
//	#endif // _ENABLE_DX10_	
//		break;
//	}

	if(g_eDeviceType==GUT_OPENGL)
	{
		return GutInitGraphicsDeviceOpenGL();
	}

	g_eDeviceType = GUT_UNKNOWN;
	g_szDeviceName[0] = '\0';

	return false;
}

bool GutInitGraphicsDevice(GutDeviceSpec &spec)
{
	strcpy(g_szDeviceName, spec.m_szDevice);
	g_eDeviceType = GutGetGraphicsDeviceType(spec.m_szDevice);

//	switch(g_eDeviceType)
//	{
//	case GUT_OPENGL:
//		return GutInitGraphicsDeviceOpenGL(&spec);
//		break;
//	case GUT_DX9:
//		return GutInitGraphicsDeviceDX9(&spec);
//		break;
//	case GUT_DX10:
//	#ifdef _ENABLE_DX10_
//		return GutInitGraphicsDeviceDX10(&spec);
//	#endif // _ENABLE_DX10_	
//		break;
//	}

	if(g_eDeviceType==GUT_OPENGL)
	{
		return GutInitGraphicsDeviceOpenGL(&spec);
	}

	g_eDeviceType = GUT_UNKNOWN;
	g_szDeviceName[0] = '\0';

	return false;
}

bool GutReleaseGraphicsDevice(void)
{
	if ( !stricmp(g_szDeviceName, "opengl") )
	{
		return GutReleaseGraphicsDeviceOpenGL();
	}
//	else if ( !stricmp(g_szDeviceName, "dx9") )
//	{
//		return GutReleaseGraphicsDeviceDX9();
//	}
//	else if ( !stricmp(g_szDeviceName, "dx10") )
//	{
//	#ifdef _ENABLE_DX10_
//		return GutReleaseGraphicsDeviceDX10();
//	#endif
//	}

	return false;
}

GutEnum GutGetGraphicsDeviceType(const char *device)
{
	if ( device )
	{
		if ( !stricmp(device, "opengl") )
		{
			return GUT_OPENGL;
		}
	//	else if ( !stricmp(device, "dx9") )
	//	{
	//		return GUT_DX9;
	//	}
	//	else if ( !stricmp(device, "dx10") )
	//	{
	//		return GUT_DX10;
	//	}
		else
		{
			return GUT_UNKNOWN;
		}
	}
	else
	{
		return g_eDeviceType;
	}
}

// left hand coord system
Matrix4x4 GutMatrixLookAtLH(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (lookat - eye); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	Matrix4x4 matrix; 
	matrix.Identity();

	matrix.SetColumn(0, xaxis);
	matrix.SetColumn(1, yaxis);
	matrix.SetColumn(2, zaxis);
	matrix[3][0] = -Vector3Dot(xaxis, eye)[0];
	matrix[3][1] = -Vector3Dot(yaxis, eye)[0];
	matrix[3][2] = -Vector3Dot(zaxis, eye)[0];

	return matrix;
}

// right hand coord system
// eye = `���Y��m`
// lookat = `���Y��Ǫ���m`
// up = `���Y���W�誺��V`
Matrix4x4 GutMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (eye - lookat); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	Matrix4x4 matrix; 
	matrix.Identity();

	matrix.SetColumn(0, xaxis);
	matrix.SetColumn(1, yaxis);
	matrix.SetColumn(2, zaxis);
	matrix[3][0] = -Vector3Dot(xaxis, eye)[0];
	matrix[3][1] = -Vector3Dot(yaxis, eye)[0];
	matrix[3][2] = -Vector3Dot(zaxis, eye)[0];

	return matrix;
}

Matrix4x4 GutMatrixOrthoRH(float w, float h, float z_near, float z_far)
{
	return GutMatrixOrthoRH_OpenGL(w, h, z_near, z_far);

//	switch(GutGetGraphicsDeviceType())
//	{
//	case GUT_OPENGL:
//		return GutMatrixOrthoRH_OpenGL(w, h, z_near, z_far);
//		break;
//	case GUT_DX9:
//	case GUT_DX10:
//	default:
//		return GutMatrixOrthoRH_DirectX(w, h, z_near, z_far);
//		break;
//	}
}

Matrix4x4 GutMatrixPerspectiveRH(float fovy, float aspect, float z_near, float z_far)
{
	return GutMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);

//	switch(GutGetGraphicsDeviceType())
//	{
//	case GUT_OPENGL:
//		return GutMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);
//		break;
//	case GUT_DX9:
//	case GUT_DX10:
//	default:
//		return GutMatrixPerspectiveRH_DirectX(fovy, aspect, z_near, z_far);
//		break;
//	}
}

// Direct3D native left hand system
// fovy = ������V������
// aspect = ������V�����ﭫ����V���������
// z_hear = ���Y�i�H�ݨ쪺�̪�Z��
// z_far = ���Y�i�H�ݨ쪺�̻��Z��
Matrix4x4 GutMatrixPerspective_DirectX(float fovy, float aspect, float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale = FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	matrix[0][0] = xscale;
	matrix[1][1] = yscale;
	matrix[2][2] = z_far / (z_far - z_near);
	matrix[2][3] = 1.0f;
	matrix[3][2] = -(z_near * z_far) /(z_far - z_near);
	matrix[3][3] = 0.0f;

	return matrix;
}

// w = `���Y������V�i�H�ݨ쪺�d��`
// h = `���Y������V�i�H�ݨ쪺�d��`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far  = `���Y�i�H�ݨ쪺�̻��Z��`
Matrix4x4 GutMatrixOrthoRH_DirectX(float w, float h, float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();
	
	matrix[0][0] = 2.0f/w;
	matrix[1][1] = 2.0f/h;
	matrix[2][2] = 1.0f/(z_near - z_far);
	matrix[3][2] = z_near / (z_near - z_far);

	return matrix;
}

// fovy = ������V������
// aspect = `������V�����ﭫ����V���������`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far = `���Y�i�H�ݨ쪺�̻��Z��`
Matrix4x4 GutMatrixPerspectiveRH_DirectX(float fovy, float aspect, 
										 float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale = FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	matrix[0][0] = xscale;
	matrix[1][1] = yscale;
	matrix[2][2] = z_far / (z_near - z_far);
	matrix[2][3] = -1.0f;
	matrix[3][2] = (z_near * z_far) /(z_near - z_far);
	matrix[3][3] = 0.0f;

	return matrix;
}

// OpenGL native right hand system
// fovy = `������V������`
// aspect = `������V�����ﭫ����V���������`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far = `���Y�i�H�ݨ쪺�̻��Z��`
Matrix4x4 GutMatrixOrthoRH_OpenGL(float w, float h, float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();
	
	matrix[0][0] = 2.0f/w;
	matrix[1][1] = 2.0f/h;
	matrix[2][2] = 2.0f/(z_near - z_far);
	matrix[3][2] = (z_far + z_near)/(z_near - z_far);
	matrix[3][3] = 1.0f;

	return matrix;
}

Matrix4x4 GutMatrixOrtho_OpenGL(float w, float h, float z_near, float z_far)
{
	return GutMatrixOrthoRH_OpenGL(w, h, z_near, z_far);
}

// OpenGL native right hand system
// fovy = `������V������`
// aspect = `������V�����ﭫ����V���������`
// z_hear = `���Y�i�H�ݨ쪺�̪�Z��`
// z_far = `���Y�i�H�ݨ쪺�̻��Z��`
Matrix4x4 GutMatrixPerspectiveRH_OpenGL(float fovy, float aspect, 
										float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale =  FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	matrix[0][0] = xscale;
	matrix[1][1] = yscale;
	matrix[2][2] = (z_far + z_near)/(z_near - z_far);
	matrix[2][3] = -1.0f;
	matrix[3][2] = 2.0f * z_far * z_near / (z_near - z_far);
	matrix[3][3] = 0.0f;

	return matrix;
}

Matrix4x4 GutMatrixPerspective_OpenGL(float fovy, float aspect, float z_near, float z_far)
{
	return GutMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);
}

// ����x_grids * y_grids�Ӯ�l���ѽL��ҫ�
bool GutCreateGrids(int x_grids, int y_grids, 
					  Vertex_V3N3 **ppVertices, int &num_vertices, 
					  unsigned short **ppIndices, int &num_indices, int &num_triangles)
{
	const int triangles_per_row = x_grids * 2;
	const int indices_per_row = triangles_per_row + 2;
	
	num_vertices = (x_grids + 1) * (y_grids + 1);
	Vertex_V3N3 *pVertices = (Vertex_V3N3 *) GutAllocate16BytesAlignedMemory(sizeof(Vertex_V3N3)*num_vertices);
	*ppVertices = pVertices;
	if ( pVertices==NULL )
		return false;

	num_indices = indices_per_row * y_grids;
	unsigned short *pIndices = new unsigned short[num_indices];
	*ppIndices = pIndices;
	if ( pIndices==NULL )
	{
		GutRelease16BytesAlignedMemory(pVertices);
		return false;
	}
	// �ϥ�triangle strip��, �T���μƥإû�������ޭȼƥش�h2
	num_triangles = num_indices-2;

	Vector4 vCorner(-0.5f, 0.5f, 0.0f, 1.0f);
	Vector4 vStep(1.0f/float(x_grids), -1.0f/float(y_grids), 0.0f, 0.0f);
	Vector4 vPosition = vCorner;
	Vector4 vNormal(0.0f, 0.0f, 1.0f);

	int x,y;
	int vertex_index = 0;

	for ( y=0; y<=y_grids; y++)
	{
		vPosition[0] = vCorner[0];
		for ( x=0; x<=x_grids; x++, vertex_index++)
		{
			vPosition.StoreXYZ(pVertices[vertex_index].m_Position);
			vNormal.StoreXYZ(pVertices[vertex_index].m_Normal);
			vPosition[0] += vStep[0];
		}
		vPosition[1] += vStep[1];
	}

	const int vertices_per_row = x_grids + 1;
	bool from_left_to_right = true;
	int  index_index = 0;

	for ( y=0; y<y_grids; y++ )
	{
		if ( from_left_to_right )
		{
			// �b�_�ƦC���ɭ�, �T���αq���ƨ�k
			pIndices[index_index++] = y * vertices_per_row;
			pIndices[index_index++] = y * vertices_per_row + vertices_per_row;

			for ( x=0; x<x_grids; x++ )
			{
				vertex_index = y * vertices_per_row + x;
				pIndices[index_index++] = vertex_index + 1;
				pIndices[index_index++] = vertex_index + 1 + vertices_per_row;
			}
		}
		else
		{
			// �b���ƦC���ɭ�, �T���αq�k�ƨ쥪
			pIndices[index_index++] = y * vertices_per_row + x_grids;
			pIndices[index_index++] = (y+1) * vertices_per_row + x_grids;

			for ( x=x_grids; x>0; x-- )
			{
				vertex_index = y * vertices_per_row + x;
				pIndices[index_index++] = vertex_index - 1;
				pIndices[index_index++] = vertex_index - 1 + vertices_per_row;
			}
		}
		from_left_to_right = !from_left_to_right;
	}

	return true;
}

bool GutReleaseGrids(Vertex_V3N3 **ppVertices, unsigned short **ppIndices)
{
	if ( *ppVertices )
	{
		GutRelease16BytesAlignedMemory(*ppVertices);
		*ppVertices = NULL;
	}

	if ( *ppIndices )
	{
		delete [] *ppIndices;
		*ppIndices = NULL;
	}

	return true;
}

void GutConvertColor(Vector4 &vColor, unsigned char rgba[4])
{
	Vector4 c = vColor * 255.0f;
	
	switch( GutGetGraphicsDeviceType() )
	{
	case GUT_DX9:
		rgba[0] = (unsigned char ) c[2];
		rgba[1] = (unsigned char ) c[1];
		rgba[2] = (unsigned char ) c[0];
		rgba[3] = (unsigned char ) c[3];
		break;
	default:
		rgba[0] = (unsigned char ) c[0];
		rgba[1] = (unsigned char ) c[1];
		rgba[2] = (unsigned char ) c[2];
		rgba[3] = (unsigned char ) c[3];
		break;
	}
}

static char g_szShaderPath[128] = "";

void GutSetShaderPath(const char *path)
{
	strcpy(g_szShaderPath, path);
}

const char *GutGetShaderPath(void)
{
	return g_szShaderPath;
}

static std::stack<std::string> g_MessagePool;

void GutPushMessage(const char *msg)
{
	if ( msg && msg[0] )
	{
		std::string str = msg;
		g_MessagePool.push(str);
	}
}

std::string GutPopMessage(void)
{
	std::string str;

	if ( g_MessagePool.size() )
	{
		str = g_MessagePool.top();
		g_MessagePool.pop();
	}

	return str;
}

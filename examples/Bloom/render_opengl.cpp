#include "Gut.h"
#include "render_opengl.h"
#include "GutOpenGL.h"
#include "GutWin32.h"
#include "GutTexture_OpenGL.h"

#include "render_data.h"


//-------圖片-------
static GLuint g_texture = 0;                // 記錄本範例唯一一張圖片的材質貼圖編號
static sImageInfo g_ImageInfo;              // 用來記錄圖片資訊

//-------shader-------
static GLuint g_BlurVS = 0;                 // posteffect_blur.glvs
static GLuint g_BlurPS = 0;                 // posteffect_blur.glfs
static GLuint g_BlurProgram = 0;            // 上面兩個的結合

//-------shader-------
static GLuint g_BrightnessPS = 0;           // posteffect_brightness.glfs
static GLuint g_BrightnessProgram = 0;      // posteffect_blur.glvs跟上面的結合

//-------FBO-------
static GLuint g_framebuffer[2] = {0, 0};    // 記本範例唯二的FBO
static GLuint g_frametexture[2] = {0, 0};   // FBO的材質貼圖編號



// One time Init
bool InitResourceOpenGL(void)
{
	if ( NULL==glCreateProgram )
	{
		printf("Driver does not support GLSL\n");
		return false;
	}
	if ( NULL==glBindFramebufferEXT )
	{
		printf("Driver does not support framebuffer object\n");
		return false;
	}

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);

	g_texture = GutLoadTexture_OpenGL("../../textures/space.tga", &g_ImageInfo);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if ( g_texture==0 )
		return false;


	//--------------------建立g_BlurProgram跟g_BrightnessProgram--------------------start

	// 載入Vertex Shader
	g_BlurVS = GutLoadVertexShaderOpenGL_GLSL("../../shaders/Posteffect_blur.glvs");
	// 載入Pixel Shader
	g_BlurPS = GutLoadFragmentShaderOpenGL_GLSL("../../shaders/Posteffect_blur.glfs");
	if ( 0==g_BlurVS || 0==g_BlurPS )
		return false;
	// 建立Shader Program物件
	g_BlurProgram = glCreateProgram();
	if ( 0==g_BlurProgram )
		return false;
	// 把Vertex Shader套入g_BlurProgram裡
	glAttachShader(g_BlurProgram, g_BlurVS);
	// 把Pixel Shader套入g_BlurProgram裡
	glAttachShader(g_BlurProgram, g_BlurPS);
	// 把g_BlurProgram中的Vertex Shader跟Pixel Shader連結起來
	glLinkProgram(g_BlurProgram);

	g_BrightnessPS = GutLoadFragmentShaderOpenGL_GLSL("../../shaders/Posteffect_brightness.glfs");
	if ( 0==g_BrightnessPS )
		return false;

	g_BrightnessProgram = GutCreateProgram(g_BlurVS, g_BrightnessPS);
	if ( 0==g_BrightnessProgram )
		return false;

	//--------------------建立g_BlurProgram跟g_BrightnessProgram--------------------end


	// 因為接下來要生的Frame buffer面積只有圖片的四分之一
	int w = g_ImageInfo.m_iWidth/4;
	int h = g_ImageInfo.m_iHeight/4;

	// 算是在初始配置FBO
	if ( !GutCreateRenderTargetOpenGL(w, h, GL_RGBA8, &g_framebuffer[0], &g_frametexture[0]) )
		return false;

	if ( !GutCreateRenderTargetOpenGL(w, h, GL_RGBA8, &g_framebuffer[1], &g_frametexture[1]) )
		return false;

	// 把緩衝區設定弄回到預設值
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}
// 釋放資源，沒什麼特別需要解釋的
bool ReleaseResourceOpenGL(void)
{
	if ( g_texture )
	{
		glDeleteTextures(1, &g_texture);
		g_texture = 0;
	}

	glDeleteShader(g_BlurVS);
	glDeleteShader(g_BlurPS);
	glDeleteProgram(g_BlurProgram);

	glDeleteShader(g_BrightnessPS);
	glDeleteShader(g_BrightnessProgram);

	for ( int i=0; i<2; i++ )
	{
		glDeleteFramebuffersEXT(1, &g_framebuffer[i]);
		glDeleteTextures(1, &g_frametexture[i]);
	}

	return true;
}
/*
// callback function. 視窗大小改變時會被呼叫, 並傳入新的視窗大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的視窗大小做為新的繪圖解析度
	glViewport(0, 0, width, height);
}
*/
// 畫一張光圖，尚未進行模糊化
static GLuint Brightness(GLuint texture, sImageInfo *pInfo)
{
	int w = pInfo->m_iWidth/4;
	int h = pInfo->m_iHeight/4;

	// 對動態貼圖來繪製畫面
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer[1]);
	glViewport(0, 0, w, h);

	glUseProgram(g_BrightnessProgram);      // 使用對亮度做變化的Shader(會在外面呼叫glUseProgram(0)來統一關閉)
	glBindTexture(GL_TEXTURE_2D, texture);  // 套用貼圖

	//--------設定亮度變化參數--------
	GLint reg_offset = glGetUniformLocation(g_BrightnessProgram, "IntensityOffset");
	GLint reg_scale = glGetUniformLocation(g_BrightnessProgram, "IntensityScale");
	glUniform4fv(reg_offset, 1, (float *)&g_vBrightnessOffset);
	glUniform4fv(reg_scale, 1, (float *)&g_vBrightnessScale);


	//--------設定資料格式--------
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuad[0].m_Position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuadInv[0].m_Texcoord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);// 回到預設狀態

	return g_frametexture[1];
}
// 在FBO上畫出模糊的圖
static GLuint BlurImage(GLuint texture, sImageInfo *pInfo)
{
	int w = pInfo->m_iWidth/4;
	int h = pInfo->m_iHeight/4;

	float fTexelW = 1.0f/(float)w;
	float fTexelH = 1.0f/(float)h;

	const int num_samples = KERNELSIZE;

	Vector4 vTexOffsetX[num_samples];
	Vector4 vTexOffsetY[num_samples];

	// 填寫畫素位置的偏移植，會傳給shaders使用，藉此製造模糊效果
	for ( int i=0; i<num_samples; i++ )
	{
		vTexOffsetX[i].Set(g_uv_offset_table[i] * fTexelW, 0.0f, 0.0f, g_weight_table[i]);
		vTexOffsetY[i].Set(0.0f, g_uv_offset_table[i] * fTexelH, 0.0f, g_weight_table[i]);
	}

	glUseProgram(g_BlurProgram);

	GLint reg = glGetUniformLocation(g_BlurProgram, "vTexOffset");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuad[0].m_Position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuadInv[0].m_Texcoord);

	// 水平方向模糊的畫
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer[0]);
		glViewport(0, 0, w, h);
		glBindTexture(GL_TEXTURE_2D, texture);
		if ( reg>=0 )
			glUniform4fv(reg, num_samples, (float *)vTexOffsetX);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	// 垂直方向模糊的畫
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer[1]);
		glBindTexture(GL_TEXTURE_2D, g_frametexture[0]);
		if ( reg>=0 )
			glUniform4fv(reg, num_samples, (float *)vTexOffsetY);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	return g_frametexture[1];
}

// 使用OpenGL來繪圖
void RenderFrameOpenGL(void)
{
	int w, h;
	GutGetWindowSize(w, h);
	glViewport(0, 0, w, h);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(0);// 先設為無Shader狀態



	// 設定材質, 套用貼圖，先將原始圖片畫出來
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuad[0].m_Position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuad[0].m_Texcoord);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	// 這裡則是畫出糊掉的改圖，跟上面那張疊合可產生光暈效果
	if ( g_bPosteffect )
	{
		GLuint texture = Brightness(g_texture, &g_ImageInfo);      // 取出圖片中偏亮的部份
		texture = BlurImage(texture, &g_ImageInfo);                // 對圖片做模糊化，回傳的是模糊光暈了
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);               // 切回主畫面
		glViewport(0, 0, w, h);

		glUseProgram(0);                                           // 不使用Shader

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, texture);                     // 使用前面做出來的光暈圖
		glEnable(GL_BLEND);                                        // 啟動混色功能
		glBlendFunc(GL_ONE, GL_ONE);


		// 開始描繪
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuad[0].m_Position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), g_FullScreenQuad[0].m_Texcoord);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisable(GL_BLEND);
	}

	GutSwapBuffersOpenGL();
}

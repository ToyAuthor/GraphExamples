#include <stdio.h>
#include <stdlib.h>
#include "Gut.h"
#include "GutBMP.h"
#include "GutTGA.h"

#ifdef __linux__

#include <string.h>
#include <string>

//static void _split_whole_name(const char *whole_name, char *fname, char *ext)
//{
//	char *p_ext;
//
//	p_ext = const_cast<char*>(rindex(whole_name, '.'));
//	if (NULL != p_ext)
//	{
//		strcpy(ext, p_ext);
//		snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
//	}
//	else
//	{
//		ext[0] = '\0';
//		strcpy(fname, whole_name);
//	}
//}
//// 這個有很大的問題，需要重寫
//void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
//{
//	char *p_whole_name;
//
////	drive[0] = '\0';
//	(char*)drive;
//
//	if (NULL == path)
//	{
//		dir[0] = '\0';
//		fname[0] = '\0';
//		ext[0] = '\0';
//		return;
//	}
//
//	if ('/' == path[strlen(path)])
//	{
//		strcpy(dir, path);
//		fname[0] = '\0';
//		ext[0] = '\0';
//		return;
//	}
//
//	p_whole_name = const_cast<char*>(rindex(path, '/'));
//	if (NULL != p_whole_name)
//	{
//		p_whole_name++;
//		_split_whole_name(p_whole_name, fname, ext);
//
//		snprintf(dir, p_whole_name - path, "%s", path);
//	}
//	else
//	{
//		_split_whole_name(path, fname, ext);
//		dir[0] = '\0';
//	}
//}

// 唯一的作用是將副檔名分析出來，暫時亂寫過關，必須好好處理重寫這個函式
void _splitpath(std::string path, char *drive, char *dir, char *fname, char *ext)
{
	(char*)drive;
	(char*)dir;
	(char*)fname;

	int i=0;
	for(;;i++)
	{
		if(path[i]=='.')
		{
			if(path[i+1]=='.')
			{
				i++;
			}
			else
			{
				break;
			}
		}
	}

	int j=0;
	for(;i<path.size();i++,j++)
	{
		ext[j]=path[i];
	}

	ext[j]='\0';
}

inline char* strlwr( char* str )
{
	char* orig = str;
	// process the string
	for ( ; *str != '\0'; str++ )
		*str = tolower(*str);
	return orig;
}

#endif

GLuint GutLoadNoCompressedTexture_OpenGL(const char *filename, sImageInfo *pInfo, bool mipmap_enabled)
{
	GLuint textureID;
	// 產生一個貼圖物件
	glGenTextures( 1, &textureID );
	// 使用g_TextureID貼圖物件
	glBindTexture( GL_TEXTURE_2D, textureID );

	char file_ext[16];
	_splitpath(filename, NULL, NULL, NULL, file_ext);
	strlwr(file_ext);

	GutImage *pLoader = NULL;

	if ( !strcmp(file_ext, ".tga") )
	{
		pLoader = new TGAImg;
	}
	else if ( !strcmp(file_ext, ".bmp") )
	{
		pLoader = new BMPImg;
	}
	else
	{
		glDeleteTextures(1, &textureID);
		return 0;
	}

	if ( !pLoader->Load(filename) )
	{
		return 0;
	}

	int w = pLoader->GetWidth();
	int h = pLoader->GetHeight();
	int bpp = pLoader->GetBPP();

	if ( pInfo )
	{
		pInfo->m_iWidth = w;
		pInfo->m_iHeight = h;
	}

	GLuint image_format = GL_RGBA;
	int elements = 4;

	switch(bpp)
	{
	case 8:
		image_format = GL_LUMINANCE;
		elements = 1;
		break;
	case 24:
		image_format = GL_RGB;
		elements = 3;
		break;
	case 32:
		image_format = GL_RGBA;
		elements = 4;
		break;
	}

	//gluBuild2DMipmaps(GL_TEXTURE_2D, elements, w, h, image_format, GL_UNSIGNED_BYTE, pLoader->GetImg());

	int p = w > h ? w : h;
	int mipmap_levels = 1;

	if ( mipmap_enabled )
	{
		int mask = 0x01;
		for ( int i=0; i<16; i++, mipmap_levels++, mask<<=1 )
		{
			if ( p & mask )
				break;
		}
	}

	if ( mipmap_levels > 1 )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	for ( int i=0; i<mipmap_levels; i++)
	{
		glTexImage2D( GL_TEXTURE_2D, i, elements,
			pLoader->GetWidth(), pLoader->GetHeight(), 0,
			image_format, GL_UNSIGNED_BYTE, pLoader->GetImg() );
		if ( !pLoader->DownSampling() )
			break;
	}

	delete pLoader;

	return textureID;
}

GLuint GutLoadNoCompressedCubemapTexture_OpenGL(const char **filename_array, bool mipmap_enabled)
{
	GLuint textureID;

	// `產生一個貼圖物件`
	glGenTextures( 1, &textureID );
	// `使用g_TextureID貼圖物件`
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID );

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLuint table[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for ( int f=0; f<6; f++ )
	{
		const char *filename= filename_array[f];

		char file_ext[16];
		_splitpath(filename, NULL, NULL, NULL, file_ext);
		strlwr(file_ext);

		GutImage *pLoader = NULL;

		if ( !strcmp(file_ext, ".tga") )
		{
			pLoader = new TGAImg;
		}
		else if ( !strcmp(file_ext, ".bmp") )
		{
			pLoader = new BMPImg;
		}
		else
		{
			glDeleteTextures(1, &textureID);
			return 0;
		}

		if ( !pLoader->Load(filename) )
		{
			delete pLoader;
			return 0;
		}

		int w = pLoader->GetWidth();
		int h = pLoader->GetHeight();
		int bpp = pLoader->GetBPP();
		unsigned char *pImageBuffer = pLoader->GetImg();

		GLuint image_format = GL_RGBA;
		int elements = 4;

		switch(bpp)
		{
		case 8:
			image_format = GL_LUMINANCE;
			elements = 1;
			break;
		case 24:
			image_format = GL_RGB;
			elements = 3;
			break;
		case 32:
			image_format = GL_RGBA;
			elements = 4;
			break;
		}

		gluBuild2DMipmaps(table[f], elements, w, h, image_format, GL_UNSIGNED_BYTE, pImageBuffer);

		glTexParameteri(table[f], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(table[f], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	return textureID;
}

GLuint GutLoadTexture_OpenGL(const char *filename, sImageInfo *pInfo, bool mipmap_enabled = true)
{
	char file_ext[16];
	_splitpath(filename, NULL, NULL, NULL, file_ext);
	strlwr(file_ext);

	if ( !strcmp(file_ext, ".tga") )
	{
		return GutLoadNoCompressedTexture_OpenGL(filename, pInfo, mipmap_enabled);
	}
	else if ( !strcmp(file_ext, ".bmp") )
	{
		return GutLoadNoCompressedTexture_OpenGL(filename, pInfo, mipmap_enabled);
	}
	else
	{
		return 0;
	}
}

GLuint GutLoadCubemapTexture_OpenGL(const char **filename_array, bool mipmap_enabled = true)
{
	const char *filename = filename_array[0];
	char file_ext[16];
	_splitpath(filename, NULL, NULL, NULL, file_ext);
	strlwr(file_ext);

	if ( !strcmp(file_ext, ".tga") )
	{
		return GutLoadNoCompressedCubemapTexture_OpenGL(filename_array, mipmap_enabled);
	}
	else if ( !strcmp(file_ext, ".bmp") )
	{
		return GutLoadNoCompressedCubemapTexture_OpenGL(filename_array, mipmap_enabled);
	}
	else
	{
		return 0;
	}
}

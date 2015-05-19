#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "Gut.h"
#include "GutFont.h"

CGutFont::CGutFont(void)
{
	m_iNumCharacters = 0;
	m_iNumCharactersSet = 0;

	m_pContent = NULL;
	m_pVertexArray = NULL;
	m_pIndexArray = NULL;

	SetFontTextureLayout(16, 8, 0); 
	SetFontSize(32, 32);
	SetFontAdvance(32, 32);
	SetConsoleResolution(512, 512);
	SetConsoleSize(16, 16);
}

CGutFont::~CGutFont()
{
	Release();
}

void CGutFont::Release(void)
{
	if ( m_pContent )
	{
		delete [] m_pContent;
		m_pContent = NULL;
	}

	if ( m_pVertexArray )
	{
		delete [] m_pVertexArray;
		m_pVertexArray = NULL;
	}

	if ( m_pIndexArray )
	{
		delete [] m_pIndexArray;
		m_pIndexArray = NULL;
	}

	m_iNumLines = 0;
}

void CGutFont::SetConsoleSize(int w, int h)
{
	Release();
	
	m_iNumRows = h;
	m_iNumColumns = w;
	m_pContent = new WCHAR[w*h];
	m_pVertexArray = new _FontVertex[w*h*4];
	m_pIndexArray = new short[w*h*6];

	memset(m_pContent, 0, sizeof(WCHAR)*w*h);

	for ( int i=0; i<w*h; i++ )
	{
		int vbase = i*4;
		int ibase = i*6;

		m_pIndexArray[ibase  ] = vbase;
		m_pIndexArray[ibase+1] = vbase+1;
		m_pIndexArray[ibase+2] = vbase+2;

		m_pIndexArray[ibase+3] = vbase;
		m_pIndexArray[ibase+4] = vbase+2;
		m_pIndexArray[ibase+5] = vbase+3;
	}
}

WCHAR *CGutFont::GetLine(int line)
{
	if ( line >=0 && line < m_iNumRows )
	{
		return m_pContent + line * m_iNumColumns;
	}

	return NULL;
}

void CGutFont::SetFontTextureLayout(int w, int h, int ascii_begin)
{
	m_iLayoutW = w;
	m_iLayoutH = h;
	m_iNumCharactersSet = w * h;
	m_iAsciiBegin = ascii_begin;
}

bool CGutFont::AccessTexture(WCHAR c, int &x, int &y)
{
	// m_iAsciiBegin���r���ɤ��Ĥ@�Ӧr���ҥN��ASCII�X
	int offset = c - m_iAsciiBegin;

	if ( offset < m_iNumCharactersSet )
	{
		// �p��X��r�b�K�Ϥ�����m
		// layoutW���b�����譱�ƦC�F�X�Ӧr��
		y = offset / m_iLayoutW;
		x = offset % m_iLayoutW;
		return true;
	}
	else
	{
		// ���b�r���ɸ�
		x = y = 0;
		return false;
	}
}

void CGutFont::BuildMesh(void)
{
	m_iNumCharacters = 0;
	int row,column;
	
	float fTexW = 1.0f/(float)m_iLayoutW;
	float fTexH = 1.0f/(float)m_iLayoutH;
	float fX = 0;
	float fY = m_fHeight;
	float fZ = 0.0f;

	_FontVertex_Temp v[4];

	for ( int y=0; y<m_iNumLines; y++ )
	{
		// `���X�o�@��`
		WCHAR *line = GetLine(y);
		// `���X��m���^�ù��̥k��`
		fX = 0;
		for ( int x=0; x<m_iNumColumns; x++ )
		{
			WCHAR c = line[x];
			if ( c==0 )
				continue;
			// `��X��r�b�K�ϸ̪���m`
			AccessTexture(c, column, row);
			// `�إ߳o�Ӥ�r���x�μҫ�`
			v[0].m_Position.Set(fX, fY, fZ);
			v[1].m_Position.Set(fX, fY-m_fFontHeight, fZ);
			v[2].m_Position.Set(fX+m_fFontWidth, fY-m_fFontHeight, fZ);
			v[3].m_Position.Set(fX+m_fFontWidth, fY, fZ);
			// `�ھڤ�r�b�K�ϸ̪���m�ӳ]�w�K�Ϯy��`
			float rX = (float)column/(float)m_iLayoutW;
			float rY = (float)row/(float)m_iLayoutH;
			v[0].m_Texcoord.Set(rX, rY, 0.0f);
			v[1].m_Texcoord.Set(rX, rY+fTexH, 0.0f);
			v[2].m_Texcoord.Set(rX+fTexW, rY+fTexH, 0.0f);
			v[3].m_Texcoord.Set(rX+fTexW, rY, 0.0f);
			// `������, Vector4 ������O�Ŷ��A�⥦�����u�ϥ�3����m��2���K�Ϯy�Ъ����A���C`
			int base = m_iNumCharacters*4;
			for ( int i=0; i<4; i++ )
			{
				v[i].m_Position.StoreXYZ(m_pVertexArray[base+i].m_Position);
				v[i].m_Texcoord.StoreXY(m_pVertexArray[base+i].m_Texcoord);
			}
			//
			m_iNumCharacters++;
			// `���X��m�V�k���ʤ@��`
			fX += m_fAdvanceX;
		}
		// `���X��m�V�U���ʤ@��`
		fY -= m_fAdvanceY;
	}
}

void CGutFont::Puts(int row, const char *string, bool bBuildMesh)
{
	if ( row >= m_iNumLines )
		return;

	int len = strlen(string);
	WCHAR *temp = new WCHAR[len+1];
	
	for ( int i=0; i<len; i++ )
	{
		temp[i] = string[i];
	}
	temp[len] = 0;

	Puts(temp, bBuildMesh);

	delete [] temp;
}

void CGutFont::Puts(const char *string, bool bBuildMesh)
{
	int len = strlen(string);
	WCHAR *temp = new WCHAR[len+1];
	
	for ( int i=0; i<len; i++ )
	{
		temp[i] = string[i];
	}
	temp[len] = 0;

	Puts(temp, bBuildMesh);

	delete [] temp;
}

void CGutFont::Puts(const WCHAR *string, bool bBuildMesh)
{
	if ( m_iNumLines >= m_iNumRows )
	{
		// ��ӵe���Q�񺡤F, ���r�V�W����
		for ( int i=0; i<m_iNumLines-1; i++ )
		{
			WCHAR *line = GetLine(i);
			WCHAR *line_next = GetLine(i+1);
			memcpy(line, line_next, m_iNumColumns*sizeof(WCHAR));
		}
	}
	else
	{
		// �s�W�@��
		m_iNumLines++;
	}

	WCHAR *line = GetLine(m_iNumLines-1);
	wcsncpy(line, string, m_iNumColumns);

	if ( bBuildMesh )
	{
		// �ئ�ҫ�	
		BuildMesh();
	}
}

void CGutFont::Puts(int row, const WCHAR *string, bool bBuildMesh)
{
	if ( row >= m_iNumLines )
		return;

	WCHAR *line = GetLine(row);
	wcsncpy(line, string, m_iNumColumns);

	if ( bBuildMesh )
	{
		// �ئ�ҫ�	
		BuildMesh();
	}
}


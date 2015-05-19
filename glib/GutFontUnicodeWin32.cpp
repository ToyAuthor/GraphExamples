#include <windows.h>
#include <gdiplus.h>

#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "Gut.h"
#include "GutFontUnicodeWin32.h"
#include "GutWin32.h"

CGutFontUniCode::CGutFontUniCode(void)
{
	m_frameCount = 0;
	m_iTextureW = m_iTextureH = 0;

	m_MemDC = NULL;
	m_Bitmap = NULL;
	m_Font = NULL;

	m_pMap = NULL;
}

CGutFontUniCode::~CGutFontUniCode()
{
	Release();

	DeleteDC(m_MemDC);
	DeleteObject(m_Bitmap);
	DeleteObject(m_Font);
}

void CGutFontUniCode::Release(void)
{
	CGutFont::Release();

	m_Map.clear();

	if ( m_pMap )
	{
		delete [] m_pMap;
		m_pMap = NULL;
	}
}

bool CGutFontUniCode::Initialize(void)
{
	HWND hWnd = GutGetWindowHandleWin32();
	HDC hDC = GetDC(hWnd);

	m_MemDC = CreateCompatibleDC(hDC);
	m_Bitmap = CreateCompatibleBitmap(m_MemDC, m_fFontWidth, m_fFontHeight);
	m_Font = CreateFont(
					m_fFontHeight, m_fFontWidth*0.5f, 
					0, 0, 
					FW_NORMAL, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					ANTIALIASED_QUALITY,
					FIXED_PITCH,
					NULL);

	SelectObject(m_MemDC, m_Bitmap);
	SelectObject(m_MemDC, m_Font);
	SetTextColor(m_MemDC, RGB(255,255,255));
	SetBkColor(m_MemDC, RGB(0,0,0));

	ReleaseDC(hWnd, hDC);

	m_pMap = new sCharInfo[m_iNumCharactersSet];
	ZeroMemory(m_pMap, sizeof(sCharInfo) * m_iNumCharactersSet);

	return true;
}

bool CGutFontUniCode::AccessTexture(WCHAR c, int &x, int &y)
{
	int loc = -1;
	bool bUpdateTexture = false;
	std::map<WCHAR, int>::iterator iter = m_Map.find(c);

	if ( iter!=m_Map.end() )
	{
		loc = iter->second;
		m_pMap[loc].m_ReferencedFrame = m_frameCount;
	}
	else
	{
		// `�ݭn��s�K��`
		bUpdateTexture = true;
		// `��X�K�Ϥ��٨S���ϥΪ��Ŷ�`
		for ( int i=0; i<m_iNumCharactersSet; i++ )
		{
			if ( m_pMap[i].m_Code==0 )
			{
				loc = i;
				break;
			}
		}
		// `��i�K�Ϻ��F, ��ܤ[�S�Ψ쪺�r�M����.`
		if ( loc==-1 )
		{
			UINT oldest_frame = m_frameCount;
			for ( int i=0; i<m_iNumCharactersSet; i++ )
			{
				if ( oldest_frame > m_pMap[i].m_ReferencedFrame )
				{
					loc = i;
					oldest_frame = m_pMap[i].m_ReferencedFrame;
				}
			}
		}

		if ( loc != -1 )
		{
			m_pMap[loc].m_Code = c;
			m_pMap[loc].m_ReferencedFrame = m_frameCount;

			m_Map.insert(std::pair<WCHAR, int>(c, loc));
		}
		else
		{
			bUpdateTexture = false;
		}
	}
	
	if ( loc != -1 )
	{
		y = loc / m_iLayoutW;
		x = loc % m_iLayoutW;

		if ( bUpdateTexture )
		{
			RECT rect = {0,0, (int)m_fFontWidth, (int)m_fFontHeight};
			FillRect(m_MemDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
			TextOutW(m_MemDC, 0, 0, &c, 1);
		}
	}
	else
	{
		// `�䤣��Ŷ��ө�s���r`
		// `�Ǧ^ -1 �N��䤣��o�Ӧr`
		x = y = -1;
	}

	return bUpdateTexture;
}

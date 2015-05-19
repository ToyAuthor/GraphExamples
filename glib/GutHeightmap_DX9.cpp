#include "Gut.h"
#include "GutDX9.h"
#include "GutModel_DX9.h"
#include "GutHeightmap_DX9.h"

CGutHeightmapDX9::CGutHeightmapDX9(void)
{
}

CGutHeightmapDX9::~CGutHeightmapDX9()
{
	Release();
}

void CGutHeightmapDX9::Render(void)
{
	if ( m_iNumFaces==0 )
		return;

	sModelMaterial_DX9 mtl;
	mtl.m_bCullFace = false;
	mtl.Submit();

	// ���oDirect3D 9�˸m
	LPDIRECT3DDEVICE9 pDevice = GutGetGraphicsDeviceDX9();
	// �]�w���I��Ʈ榡
	pDevice->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL);
	// �e�X�Ҧ�����r
	pDevice->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLESTRIP, 0, 
		m_iNumVertices,	m_iNumFaces, 
		m_pIndexArray, D3DFMT_INDEX16, 
		m_pVertexArray, sizeof(sTerrainVertex));
}

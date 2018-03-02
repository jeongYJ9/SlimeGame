#include "../stdafx.h"
#include "TextureDrawer.h"

TextureDrawer::TextureDrawer(Size size, wstring keyName, float depth)
	: size(size), keyName(keyName)
{
	VertexPT* vertex = new VertexPT[6];
	vertex[0] = VertexPT(0, 0, 0, 0, depth); //0
	vertex[1] = VertexPT(size.width, 0, 1, 0, depth); //1
	vertex[2] = VertexPT(0, size.height, 0, 1, depth); //2
	vertex[3] = VertexPT(0, size.height, 0, 1, depth); //2
	vertex[4] = VertexPT(size.width, 0, 1, 0, depth); //1
	vertex[5] = VertexPT(size.width, size.height, 1, 1, depth); //3

	int dataSize = sizeof(VertexPT) * 6;
	SetData(vertex, dataSize, VertexPT::FVF);
}

TextureDrawer::~TextureDrawer()
{
}

void TextureDrawer::Render()
{
	DEVICE()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DEVICE()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	LPDIRECT3DTEXTURE9 texture = TEXTURE()->Get(keyName);
	DEVICE()->SetTexture(0, texture);

	DEVICE()->SetStreamSource(0, vertexBuffer->GetBuffer(), 0, sizeof(VertexPT));
	DEVICE()->SetFVF(VertexPT::FVF);
	DEVICE()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	
	DEVICE()->SetTexture(0, NULL);
	DEVICE()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

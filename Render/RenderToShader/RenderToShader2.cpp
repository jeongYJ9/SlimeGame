#include "../../stdafx.h"
#include "RenderToShader2.h"

DWORD RenderToShader2::Vertex::fvf = D3DFVF_XYZ | D3DFVF_TEX1;

RenderToShader2::RenderToShader2(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture)
	: Shader(device, L"./RenderToShader2.hlsl")
	, texture(texture)
	, selected(0)
	, color(0.5f, 0.5f, 0.5f, 1)
{
	Vertex vertex[6];

	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);

	vertex[0].position = D3DXVECTOR3(0, viewport.Height, 0);
	vertex[0].uv = D3DXVECTOR2(0, 1);

	vertex[1].position = D3DXVECTOR3(0, 0, 0);
	vertex[1].uv = D3DXVECTOR2(0, 0);

	vertex[2].position = D3DXVECTOR3(viewport.Width, viewport.Height, 0);
	vertex[2].uv = D3DXVECTOR2(1, 1);

	vertex[3].position = D3DXVECTOR3(viewport.Width, viewport.Height, 0);
	vertex[3].uv = D3DXVECTOR2(1, 1);

	vertex[4].position = D3DXVECTOR3(0, 0, 0);
	vertex[4].uv = D3DXVECTOR2(0, 0);

	vertex[5].position = D3DXVECTOR3(viewport.Width, 0, 0);
	vertex[5].uv = D3DXVECTOR2(1, 0);

	HRESULT hr;
	hr = device->CreateVertexBuffer
	(
		sizeof(Vertex) * 6, 0, Vertex::fvf
		, D3DPOOL_DEFAULT, &vertexBuffer, NULL
	);
	assert(SUCCEEDED(hr));

	void* vertexData = NULL;
	hr = vertexBuffer->Lock(0, sizeof(Vertex) * 6, &vertexData, 0);
	assert(SUCCEEDED(hr));

	memcpy(vertexData, vertex, sizeof(Vertex) * 6);

	hr = vertexBuffer->Unlock();
	assert(SUCCEEDED(hr));

}


RenderToShader2::~RenderToShader2()
{
	
}

void RenderToShader2::Update()
{
	
}

void RenderToShader2::Render()
{
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(Vertex::fvf);
	device->SetTransform(D3DTS_WORLD, &world);

	shader->SetVector("_color", (D3DXVECTOR4 *)&color);
	shader->SetTexture("_map", texture);
	
	UINT pass = 0;
	shader->Begin(&pass, NULL);
	{
		shader->BeginPass(selected);
		{
			device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		}
		shader->EndPass();
	}
	shader->End();
}

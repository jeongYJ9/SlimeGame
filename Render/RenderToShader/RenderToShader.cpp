#include "../../stdafx.h"
#include "RenderToShader.h"

DWORD RenderToShader::Vertex::fvf = D3DFVF_XYZ | D3DFVF_TEX1;

RenderToShader::RenderToShader(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./RenderToShader.hlsl")
	, texture(texture)
	, color(0, 0, 0, 1)
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

	target = new RenderTarget();
}


RenderToShader::~RenderToShader()
{
	SAFE_DELETE(target);
}

void RenderToShader::Update()
{
	target->BeginScene();
	{
		for (size_t i = 0; i < preShaders.size(); i++)
			preShaders[i]->Render();
	}
	target->EndScene();
}

void RenderToShader::Render()
{
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(Vertex::fvf);
	device->SetTransform(D3DTS_WORLD, &world);


	shader->SetVector("_color", (D3DXVECTOR4 *)&color);
	shader->SetTexture("_map", target->GetTexture());
	
	UINT pass = 0;
	shader->Begin(&pass, NULL);
	{
		shader->BeginPass(0);
		{
			device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		}
		shader->EndPass();
	}
	shader->End();
}

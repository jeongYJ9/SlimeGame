#include "../stdafx.h"
#include "Texture.h"

LPDIRECT3DVERTEXBUFFER9 Texture::vertexBuffer = NULL;
DWORD Texture::Vertex::fvf = D3DFVF_XYZ | D3DFVF_TEX1;
vector<Texture *> Texture::textures;
map<wstring, LPDIRECT3DTEXTURE9> Texture::textureMap;

Texture * Texture::Add(wstring fileName, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd)
{
	HRESULT hr;
	LPDIRECT3DTEXTURE9 dxTexture;

	if (textureMap.count(fileName) < 1)
	{
		hr = D3DXCreateTextureFromFileEx
		(
			Device::GetDevice()
			, fileName.c_str()
			, D3DX_DEFAULT_NONPOW2
			, D3DX_DEFAULT_NONPOW2
			, 1
			, NULL
			, D3DFMT_UNKNOWN
			, D3DPOOL_MANAGED
			, D3DX_FILTER_NONE
			, D3DX_FILTER_NONE
			, 0
			, NULL
			, NULL
			, &dxTexture
		);
		assert(SUCCEEDED(hr));

		textureMap[fileName] = dxTexture;
	}
	
	dxTexture = textureMap[fileName];

	Texture* texture = new Texture(dxTexture, cutStart, cutEnd);
	texture->number = textures.size();

	textures.push_back(texture);
	CreateVertexBuffer();

	return texture;
}

Texture::Texture(LPDIRECT3DTEXTURE9 texture, D3DXVECTOR2 cutStart, D3DXVECTOR2 cutEnd)
	: texture(texture)
{
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);

	textureFormat = desc.Format;
	textureSize.x = (float)desc.Width;
	textureSize.y = (float)desc.Height;

	start.x = (cutStart.x > 0) ? cutStart.x / textureSize.x : 0.0f;
	start.y = (cutStart.y > 0) ? cutStart.y / textureSize.y : 0.0f;
	end.x = (cutEnd.x > 0) ? cutEnd.x / textureSize.x : 1.0f;
	end.y = (cutEnd.y > 0) ? cutEnd.y / textureSize.y : 1.0f;


	this->cutStart = cutStart;

	this->cutEnd.x = (cutEnd.x < 1) ? textureSize.x : cutEnd.x;
	this->cutEnd.y = (cutEnd.y < 1) ? textureSize.y : cutEnd.y;
}

Texture::~Texture()
{
	
}

void Texture::CreateVertexBuffer()
{
	if (textures.size() < 1)
		return;

	SAFE_RELEASE(vertexBuffer);

	
	UINT vertexCount = textures.size() * 6;
	UINT vertexSize = vertexCount * sizeof(Vertex);

	HRESULT hr;
	hr = Device::GetDevice()->CreateVertexBuffer
	(
		vertexSize
		, 0
		, Vertex::fvf
		, D3DPOOL_DEFAULT
		, &vertexBuffer
		, NULL
	);
	assert(SUCCEEDED(hr));

	Vertex* vertex = new Vertex[vertexCount];
	for (size_t i = 0; i < textures.size(); i++)
	{
		D3DXVECTOR2 size;
		size.x = textures[i]->cutEnd.x - textures[i]->cutStart.x;
		size.y = textures[i]->cutEnd.y - textures[i]->cutStart.y;


		D3DXVECTOR2 start = textures[i]->start;
		D3DXVECTOR2 end = textures[i]->end;
		vertex[i * 6 + 0] = Vertex(0, size.y, start.x, end.y);
		vertex[i * 6 + 1] = Vertex(0, 0, start.x, start.y);
		vertex[i * 6 + 2] = Vertex(size.x, size.y, end.x, end.y);
		vertex[i * 6 + 3] = Vertex(size.x, size.y, end.x, end.y);
		vertex[i * 6 + 4] = Vertex(0, 0, start.x, start.y);
		vertex[i * 6 + 5] = Vertex(size.x, 0, end.x, start.y);
	}

	void* vertexData = NULL;
	hr = vertexBuffer->Lock(0, vertexSize, &vertexData, NULL);
	assert(SUCCEEDED(hr));

	memcpy(vertexData, vertex, vertexSize);

	hr = vertexBuffer->Unlock();
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertex);
}

void Texture::Delete()
{
	for (size_t i = 0; i < textures.size(); i++)
		SAFE_DELETE(textures[i]);

	map<wstring, LPDIRECT3DTEXTURE9>::iterator iter;
	for (iter = textureMap.begin(); iter != textureMap.end(); iter++)
		SAFE_RELEASE(iter->second);

	SAFE_RELEASE(vertexBuffer);
}

void Texture::Render()
{
	UINT start = number * 6;

	Device::GetDevice()->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	Device::GetDevice()->SetFVF(Vertex::fvf);
	Device::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, start, 2);
}

#include "../stdafx.h"
#include "TextureManager.h"

TextureManager* TextureManager::instance = NULL;

TextureManager * TextureManager::GetInstance()
{
	if (instance == NULL)
		instance = new TextureManager();

	return instance;
}

void TextureManager::DeleteInstance()
{
	SAFE_DELETE(instance);
}

void TextureManager::Load(wstring fileName, LPDIRECT3DTEXTURE9* texture, DWORD colorKey)
{
	HRESULT hr = D3DXCreateTextureFromFileEx
	(
		DEVICE()
		, fileName.c_str()
		, D3DX_DEFAULT_NONPOW2 //width
		, D3DX_DEFAULT_NONPOW2 //height
		, 1
		, NULL
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, colorKey
		, NULL
		, NULL
		, texture
	);
	assert(SUCCEEDED(hr));
}

void TextureManager::ChangeColor(LPDIRECT3DTEXTURE9 texture, DWORD color)
{
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);

	D3DLOCKED_RECT rect;
	texture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);

	DWORD* pixel = (DWORD *)rect.pBits;
	for (UINT y = 0; y < desc.Height; y++)
	{
		for (UINT x = 0; x < desc.Width; x++)
		{
			D3DXCOLOR xColor = pixel[y * rect.Pitch / 4 + x];

			if(xColor.a > 0.1f)
				pixel[y * rect.Pitch / 4 + x] = color;
		}
	}
	texture->UnlockRect(0);
}

void TextureManager::Add(wstring keyName, LPDIRECT3DTEXTURE9 texture, const Rect* srcRect)
{
	//assert(textureMap.count(keyName) < 1);
	if (textureMap.count(keyName) > 0)
		return;


	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);

	Rect area;
	if (srcRect != NULL)
		area = *srcRect;
	else
		area = Rect(0, 0, (float)desc.Width, (float)desc.Height);


	LPDIRECT3DTEXTURE9 destTexture;
	HRESULT hr = D3DXCreateTexture
	(
		DEVICE(), (UINT)area.size.width, (UINT)area.size.height,
		1, 0, desc.Format, D3DPOOL_MANAGED, &destTexture
	);
	assert(SUCCEEDED(hr));


	D3DSURFACE_DESC desc2;
	destTexture->GetLevelDesc(0, &desc2);

	D3DLOCKED_RECT rect;
	D3DLOCKED_RECT rect2;

	RECT tempArea;
	area.GetRECT(&tempArea);

	texture->LockRect(0, &rect, &tempArea, D3DLOCK_DISCARD);
	destTexture->LockRect(0, &rect2, NULL, D3DLOCK_DISCARD);

	DWORD* color = (DWORD *)rect.pBits;
	DWORD* color2 = (DWORD *)rect2.pBits;
	for (UINT y = 0; y < area.size.height; y++)
	{
		for (UINT x = 0; x < area.size.width; x++)
		{
			UINT index = y * rect.Pitch / 4 + x;
			UINT index2 = y * rect2.Pitch / 4 + x;

			color2[index2] = color[index];
		}
	}
	texture->UnlockRect(0);
	destTexture->UnlockRect(0);

	textureMap[keyName] = destTexture;
}

LPDIRECT3DTEXTURE9 TextureManager::Get(wstring keyName)
{
	if (textureMap.find(keyName) != textureMap.end())
		return textureMap[keyName];

	return NULL;
}

bool TextureManager::IsExist(wstring keyName)
{
	return textureMap.count(keyName) > 0;
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	map<wstring, LPDIRECT3DTEXTURE9>::iterator itor;
	
	for (itor = textureMap.begin(); itor != textureMap.end(); itor++)
		SAFE_RELEASE(itor->second);
}

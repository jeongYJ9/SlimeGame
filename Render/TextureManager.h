#pragma once

class TextureManager
{
public:
	static TextureManager* GetInstance();
	static void DeleteInstance();

	static void Load(wstring fileName, LPDIRECT3DTEXTURE9* texture, DWORD colorKey = 0);
	static void ChangeColor(LPDIRECT3DTEXTURE9 texture, DWORD color);

	void Add(wstring keyName, LPDIRECT3DTEXTURE9 texture
		, const Rect* srcRect = NULL);

	LPDIRECT3DTEXTURE9 Get(wstring keyName);

	bool IsExist(wstring keyName);

private:
	static TextureManager* instance;

	TextureManager();
	~TextureManager();

	map<wstring, LPDIRECT3DTEXTURE9> textureMap;
};
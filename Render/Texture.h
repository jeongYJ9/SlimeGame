#pragma once

class Texture
{
public:
	static Texture* Add
	(
		wstring fileName
		, D3DXVECTOR2 cutStart = D3DXVECTOR2(0, 0)
		, D3DXVECTOR2 cutEnd = D3DXVECTOR2(0, 0)
	);

	static void Delete();

	void GetTextureSize(D3DXVECTOR2* size) const
	{
		*size = this->textureSize;
	}
	void GetRealSize(D3DXVECTOR2* size) const
	{
		size->x = cutEnd.x - cutStart.x;
		size->y = cutEnd.y - cutStart.y;
	}

	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return texture;
	}

	void Render();

private:
	Texture
	(
		LPDIRECT3DTEXTURE9 texture
		, D3DXVECTOR2 cutStart
		, D3DXVECTOR2 cutEnd
	);
	~Texture();

	UINT number;

	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR2 textureSize;
	D3DFORMAT textureFormat;

	D3DXVECTOR2 cutStart;
	D3DXVECTOR2 cutEnd;
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;


	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;

		Vertex() {}
		Vertex(float x, float y, float u, float v)
		{
			position = D3DXVECTOR3(x, y, 0.0f);
			uv = D3DXVECTOR2(u, v);
		}

		static DWORD fvf;
	};

	static void CreateVertexBuffer();
	static LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	
	static vector<Texture *> textures;
	static map<wstring, LPDIRECT3DTEXTURE9> textureMap;
};


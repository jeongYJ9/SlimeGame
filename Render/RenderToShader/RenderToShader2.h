#pragma once
#include "../Shader/Shader.h"
class RenderToShader2 : public Shader
{
public:
	RenderToShader2(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 texture);
	~RenderToShader2();

	void Update();
	void Render();

private:

	LPDIRECT3DTEXTURE9 texture;

	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;

		static DWORD fvf;
	};

	UINT selected;
	D3DXCOLOR color;
};


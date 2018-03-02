#pragma once
#include "../Shader/Shader.h"
class RenderToShader : public Shader
{
public:
	RenderToShader(LPDIRECT3DDEVICE9 device);
	~RenderToShader();

	void Update();
	void Render();

private:

	RenderTarget* target;

	LPDIRECT3DTEXTURE9 texture;

	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;

		static DWORD fvf;
	};

	D3DXCOLOR color;
};


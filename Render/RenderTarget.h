#pragma once
class RenderTarget
{
public:
	RenderTarget(UINT width =0, UINT height=0);
	~RenderTarget();

	void BeginScene(DWORD color= 0xFF777777);
	void EndScene();

	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return texture;
	}

private:
	UINT width;
	UINT height;
	D3DFORMAT colorFormat;
	D3DFORMAT depthFormat;

	LPD3DXRENDERTOSURFACE renderToSurface;

	LPDIRECT3DTEXTURE9 texture;
	LPDIRECT3DSURFACE9 surface;
};


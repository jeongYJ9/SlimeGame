#include "../stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(UINT width, UINT height)
	: width(width), height(height)
{
	LPDIRECT3DSURFACE9 colorSurface;
	LPDIRECT3DSURFACE9 depthSurface;

	Device::GetDevice()->GetRenderTarget(0, &colorSurface);
	Device::GetDevice()->GetDepthStencilSurface(&depthSurface);

	D3DSURFACE_DESC colorDesc;//그릴대상의 픽셀정보를 가져오기위한것
	colorSurface->GetDesc(&colorDesc);

	D3DSURFACE_DESC depthDesc;
	depthSurface->GetDesc(&depthDesc);

	SAFE_RELEASE(depthSurface);
	SAFE_RELEASE(colorSurface);


	if (width < 1) width = colorDesc.Width;
	if (height < 1) height = colorDesc.Height;

	colorFormat = colorDesc.Format;
	depthFormat = depthDesc.Format;

	
	HRESULT hr;
	hr = D3DXCreateRenderToSurface//비긴신으로 열어서 이곳으로옴
		//렌더링을 관리하는것. back버퍼에 들어간게 일로들어옴.
	(
		Device::GetDevice()
		, width, height, colorFormat, TRUE, depthFormat
		, &renderToSurface
	);
	assert(SUCCEEDED(hr));

	hr = D3DXCreateTexture
	(
		Device::GetDevice()
		, width, height, 1, D3DUSAGE_RENDERTARGET
		, colorFormat, D3DPOOL_DEFAULT, &texture
	);
	assert(SUCCEEDED(hr));

	hr = texture->GetSurfaceLevel(0, &surface);
	assert(SUCCEEDED(hr));



	Device::GetDevice()->ColorFill(surface, NULL, 0xFF000000);
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(renderToSurface);
	SAFE_RELEASE(texture);
	SAFE_RELEASE(surface);
}

void RenderTarget::BeginScene(DWORD color)
{
	renderToSurface->BeginScene(surface, NULL);
	HRESULT hr = Device::GetDevice()->Clear
	(
		0, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER 
		, color
		, 1.0f
		, 0
	);
	assert(SUCCEEDED(hr));
}

void RenderTarget::EndScene()//present의 역할도 함. 후면에 그린것을 전면에 내보냄
{
	renderToSurface->EndScene(0);
}

#include "../stdafx.h"
#include "Direct2D.h"

LPDIRECT3D9 Direct2D::d3d = NULL;
LPDIRECT3DDEVICE9 Direct2D::device = NULL;

void Direct2D::Create(HWND winHandle)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	assert(d3d != NULL);

	D3DCAPS9 caps;
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	D3DPRESENT_PARAMETERS d3dParam = { 0 };
	d3dParam.Windowed = WIN_IS_WINDOW;
	d3dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dParam.AutoDepthStencilFormat = D3DFMT_D16;
	d3dParam.EnableAutoDepthStencil = TRUE;


	HRESULT hr = d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, winHandle
		, D3DCREATE_HARDWARE_VERTEXPROCESSING
		, &d3dParam
		, &device
	);
	assert(SUCCEEDED(hr));
}

void Direct2D::Delete()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}

void Direct2D::BeginScene(DWORD clearColor)
{
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);
	device->BeginScene();
}

void Direct2D::EndScene()
{
	device->EndScene();
	DEVICE()->Present(NULL, NULL, NULL, NULL);
}

void Direct2D::SetWorldLocation(const Point& point, float depth)
{
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, point.x, point.y, depth);
	DEVICE()->SetTransform(D3DTS_WORLD, &world);
}
void Direct2D::SetWorldLocation(const Point& point, const Size& size, float depth)
{
	D3DXMATRIX world, mat;
	D3DXMatrixScaling(&mat, size.width, size.height, 0.0f);
	D3DXMatrixTranslation(&world, point.x, point.y, depth);
	world = mat * world;
	DEVICE()->SetTransform(D3DTS_WORLD, &world);
}
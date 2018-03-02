#include "../stdafx.h"
#include "Device.h"

Device* Device::instance = NULL;
LPDIRECT3DDEVICE9 Device::device = NULL;

Device * Device::Get(HWND hwnd, UINT width, UINT height)
{
	if (instance == NULL)
		instance = new Device(hwnd, width, height);

	return instance;
}

void Device::Delete()
{
	SAFE_DELETE(instance);
}

void Device::BeginScene(DWORD clearColor)
{
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);
	device->BeginScene();
}

void Device::EndScene()
{
	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

Device::Device(HWND hwnd, UINT width, UINT height)
	: isFullScreen (false)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	assert(d3d != NULL);

	D3DPRESENT_PARAMETERS param = { 0 };
	param.Windowed = !isFullScreen;
	if (isFullScreen == true)
	{
		param.BackBufferWidth = width;
		param.BackBufferHeight = height;
		param.BackBufferFormat = D3DFMT_UNKNOWN;
	}
	else
		param.BackBufferFormat = D3DFMT_X8R8G8B8;

	param.hDeviceWindow = hwnd;
	param.BackBufferCount = 1;
	param.SwapEffect = D3DSWAPEFFECT_FLIP;
	param.AutoDepthStencilFormat = D3DFMT_D16;
	param.EnableAutoDepthStencil = TRUE;
	//param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//수직동기화 끄기

	HRESULT hr;
	hr = d3d->CreateDevice
	(
		D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, hwnd
		, D3DCREATE_HARDWARE_VERTEXPROCESSING
		, &param
		, &device
	);
	assert(SUCCEEDED(hr));
}

Device::~Device()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}


void Device::SetWorldLocation(const Point& point, float depth)
{
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, point.x, point.y, depth);
	GetDevice()->SetTransform(D3DTS_WORLD, &world);
}
void Device::SetWorldLocation(const Point& point, const Size& size, float depth)
{
	D3DXMATRIX world, mat;
	D3DXMatrixScaling(&mat, size.width, size.height, 0.0f);
	D3DXMatrixTranslation(&world, point.x, point.y, depth);
	world = mat * world;
	GetDevice()->SetTransform(D3DTS_WORLD, &world);
}
#pragma once
class Device
{
public:
	static Device* Get(HWND hwnd = NULL, UINT width = 0, UINT height = 0);
	static void Delete();

	static LPDIRECT3DDEVICE9 GetDevice()
	{
		return device;
	}

	void BeginScene(DWORD clearColor);
	void EndScene();
	
	static void SetWorldLocation(const Point& point, float depth);

	static void SetWorldLocation(const Point& point, const Size& size, float depth);

private:
	Device(HWND hwnd, UINT width , UINT height);
	~Device();

	static Device* instance;

	bool isFullScreen;

	LPDIRECT3D9 d3d;
	static LPDIRECT3DDEVICE9 device;
};
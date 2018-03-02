#pragma once
class Direct2D
{
public:
	static void Create(HWND winHandle);
	static void Delete();

	static LPDIRECT3DDEVICE9 Device() { return device; }

	static void BeginScene(DWORD clearColor = 0xFF6495ED);
	static void EndScene();

	static void SetWorldLocation(const Point& point, float depth);
	static void SetWorldLocation(const Point& point, const Size& size, float depth);

private:
	static LPDIRECT3D9 d3d;
	static LPDIRECT3DDEVICE9 device;
};


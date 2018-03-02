#include "../stdafx.h"
#include "Window.h"
#include "../GameMain.h"

#define WindowWidth 1024+16//양옆에 여백?이 있는듯
#define WindowHeight 768+40


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	GameMain* gameMain = new GameMain
	(
		hInstance, L"DirectX Game", { WindowWidth, WindowHeight }
	);
	gameMain->Create();

	WPARAM wParam = gameMain->Run();
	delete gameMain;

	return wParam;
}

Window::Window(HINSTANCE hInstance, wstring title, POINT windowSize)
	: instance(hInstance)
	, title(title)
	, windowSize(windowSize)
	, handle(NULL)
{
	
}

Window::~Window()
{
	DestroyWindow(handle);
}

LRESULT CALLBACK WndProc(HWND handle, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	Mouse::GetInstance()->InputProc(message, wParam, lParam);
//	TweakBar::Get()->InputProc(handle, message, wParam, lParam);
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

void Window::Create()
{
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_HAND);
	wndClass.hInstance = instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = title.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	
	WORD wHr = RegisterClass(&wndClass);
	assert(wHr != 0);


	handle = CreateWindow(
		title.c_str()
		, title.c_str()
		, WS_OVERLAPPEDWINDOW
		, 300 //x
		, 100 //y
		, WindowWidth //width
		, WindowHeight //height
		, NULL
		, (HMENU)NULL
		, instance
		, NULL
	);
	assert(handle != NULL);
	ShowWindow(handle, SW_SHOWNORMAL);
}

WPARAM Window::Run()
{
	MSG msg = { 0 };

	Device::Get(handle, WindowWidth, WindowHeight);
	Frames::Get()->Start();
	//TweakBar::Get()->Start();//
	Initialize();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Keyboard::GetInstance()->Update();
			Mouse::GetInstance()->Update();

			Frames::Get()->Update();

			Update();

			Device::Get()->BeginScene(0xFFFFAE49);//ARGB
			{
				Render();

				Frames::Get()->Print();
				if (Keyboard::GetInstance()->KeyDown(VK_F1))
				{
					//TweakBar::Get()->ChangeDraw();//
				}
				//TweakBar::Get()->Render();//
			}
			Device::Get()->EndScene();
		}
	}
	Destroy();

	//TweakBar::Delete();//

	Keyboard::DeleteInstance();
	Mouse::DeleteInstance();

	Device::Delete();

	UnregisterClass(title.c_str(), instance);
	return msg.wParam;
}

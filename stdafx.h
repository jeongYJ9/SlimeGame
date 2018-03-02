#pragma once

#include <Windows.h>
#include <assert.h>

#include <string>
#include <vector>
#include <map>

using namespace std;

#include <d3d9.h>
#include <d3dx9.h>



#pragma comment(lib, "winmm.lib")
//#include "AntTweakBar.h"
//#pragma comment(lib, "AntTweakBar.lib")

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")



#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p) { if (p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p)=NULL; } }



#include "Math.h"
#include "./Render/Rect/Size.h"
#include "./Render/Rect/Point.h"

#include "./Render/Rect/Rect.h"
#include "./System/Direct2D.h"
#include "./System/Keyboard.h"
#include "./System/Mouse.h"
#include "./Render/Buffers.h"

#include "./Render/Vertex.h"
#include "./Render/TextureManager.h"

#include "./Render/RectDrawer.h"
#include "./Render/TextureDrawer.h"
#include "./Object/ProgressBar.h"
#include "./Render/Texture.h"

#include "./Render/Animator.h"

#include "./System/Device.h"
#include "./System/Frames.h"
//#include "TweakBar.h"

#include "./Render/RenderTarget.h"

#define WIN_X 100
#define WIN_Y 100
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_TITLE L"DirectX Game"
#define WIN_IS_WINDOW TRUE
#define WIN_SHOW_CURSOR TRUE

#define DEVICE() Direct2D::Device()
#define KEYBOARD() Keyboard::GetInstance()
#define MOUSE() Mouse::GetInstance()
#define TEXTURE() TextureManager::GetInstance()


//게임메인에 srand 타임시드 넣었음.
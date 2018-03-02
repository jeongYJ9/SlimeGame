#include "../stdafx.h"
#include "Frames.h"

Frames* Frames::instance = NULL;

Frames::Frames( void ) :
isTimerStopped(true),
ticksPerSecond(0), currentTime(0), lastTime(0), lastFPSUpdate(0), fpsUpdateInterval(0),
frameCount(0), runningTime(0), timeElapsed(0), framePerSecond(0),
font(NULL)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);
	fpsUpdateInterval = ticksPerSecond >> 1;
}

Frames::~Frames( void )
{
	SAFE_RELEASE(font);
}

Frames* Frames::Get()
{
	if(instance == NULL)
		instance = new Frames();

	return instance;
}

void Frames::Delete()
{
	SAFE_DELETE(instance);
}

void Frames::Update()
{
	if(isTimerStopped) return;

	//1. 현재시간을 가져와 시간 간격 및 진행 시간을 계산한다.
	QueryPerformanceCounter( (LARGE_INTEGER *)&currentTime );
	timeElapsed = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	runningTime += timeElapsed;

	
	//2. FPS Update
	frameCount++;
	if(currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = (float)currentTime / (float)ticksPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)ticksPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = (INT64)currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

void Frames::Print()
{
	if (font == NULL)
	{
		D3DXFONT_DESC desc = { 0 };
		desc.Width = 20;
		desc.Height = 20;
		desc.CharSet = HANGUL_CHARSET;
		wcscpy(desc.FaceName, L"돋움체");

		HRESULT hr;
		hr = D3DXCreateFontIndirect(
			Device::GetDevice()
			, &desc, &font
		);
		assert(SUCCEEDED(hr));
	}

	wstring str = L"";
	str = to_wstring((int)framePerSecond);

	D3DVIEWPORT9 viewport;
	Device::GetDevice()->GetViewport(&viewport);

	RECT rect = { viewport.Width - 150, 5, viewport.Width-5, 25 };
	font->DrawText(NULL, str.c_str(), -1, &rect, DT_RIGHT, 0xFFFFFF00);
}

void Frames::Start()
{
	if(!isTimerStopped)
		assert(false);

	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	isTimerStopped = false;
}

void Frames::Stop()
{
	if(isTimerStopped)
		assert(false);
    
    INT64 stopTime = 0;
    QueryPerformanceCounter((LARGE_INTEGER *)&stopTime);
    runningTime += (float)(stopTime - lastTime) / (float)ticksPerSecond;
    isTimerStopped = true;
}
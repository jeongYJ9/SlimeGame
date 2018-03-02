#pragma once
#include "../Render/Shader/Shader.h"


class IProgressBar {
public:
	virtual void BarRect(Rect* rect) = 0;
};

class ProgressBar : public Shader
{
public:
	ProgressBar(LPDIRECT3DDEVICE9 device);
	~ProgressBar();

	void SetProgressBar(Rect* rect);
	void SetAdjestLocation(Point adLocation);

	void SetMax(float max);
	void SetCur(float cur);
	void SetWidth(float barWidth)
	{
		this->barWidth = barWidth;
	}
	void SetHeight(float barHeight)
	{
		this->barHeight = barHeight;
	}
	void SetColor(DWORD color)
	{
		this->color = color;
	}
	void SetColorBar(float ColorBar)
	{
		this->ColorBar = ColorBar;
	}

	void Update();
	void Render();
	
private:
	Rect* target;
	RectDrawer* rectDrawer;

	Point adjustLocation;
	Rect rect;
	DWORD color;
	float maxNumber;
	float curNumber;
	float barWidth;
	float barHeight;
	float ColorBar;
};
#pragma once

#include "Drawer.h"
class RectDrawer : public Drawer
{
public:
	RectDrawer(Size size, float depth = 0.0f, DWORD color = 0xFF000000);
	~RectDrawer();

	void Render();

private:
	DWORD color;
};
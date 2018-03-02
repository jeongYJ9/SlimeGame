#pragma once

#include "Drawer.h"
class TextureDrawer : public Drawer
{
public:
	TextureDrawer(Size size, wstring keyName, float depth = 0.0f);
	~TextureDrawer();

	void GetSize(Size* size) const
	{
		*size = this->size;
	}

	void Render();

private:
	Size size;

	wstring keyName;
};
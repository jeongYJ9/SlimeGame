#include "../stdafx.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./Render/Shader/Bar.hlsl")
	, target(NULL), adjustLocation(Point(0, 0))
	, maxNumber(1.0f), curNumber(1.0f), barWidth(0.0f)
	, color(0xFFFFFF00)
{
}

ProgressBar::~ProgressBar()
{
	SAFE_DELETE(rectDrawer);
}

void ProgressBar::SetProgressBar(Rect* target)
{
	this->target = target;

	rect.location = target->location;
	rect.size.width = target->size.width;
	rect.size.height = target->size.height;

	//temp
	//rect.size.height = target->size.height;

	rectDrawer = new RectDrawer(rect.size, -5.0f, color);//ARGB
}

void ProgressBar::SetAdjestLocation(Point adLocation)
{
	this->adjustLocation = adLocation;
}

void ProgressBar::SetMax(float max)
{
	this->maxNumber = max;
}

void ProgressBar::SetCur(float cur)
{
	this->curNumber = cur;
}

void ProgressBar::Update()
{
	if (target == NULL)
		return;
	rect.location = target->location - adjustLocation;
}

void ProgressBar::Render()
{
	//float temp = curNumber / maxNumber;

//	Device::SetWorldLocation(rect.location, Size(barWidth, barHeight), -5.0f);
//	rectDrawer->Render();


	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);


	shader->Begin(NULL, NULL);
	{
		D3DXMatrixTranslation(&world, rect.location.x, rect.location.y, 0);
		Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		Device::SetWorldLocation(rect.location, Size(barWidth, barHeight), -5.0f);
	
		shader->SetFloat("_barWidth", barWidth);

		shader->BeginPass(0);

		rectDrawer->Render();

		shader->EndPass();
	}
	shader->End();

}


//long Red = 0xFF;
//long Green = 0xFF;
//if (Hp >= 0.5f)
//{
//	Red /= Hp;
//}
//if (Hp <= 0.5f)
//{
//	Green *= Hp;
//}
//DWORD color = 0xFF000000 + (Red * 2 * 2 * 2 * 2) + (Green * 2 * 2);
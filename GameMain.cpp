#include "stdafx.h"
#include "GameMain.h"

#include "./Render/Shader/Shader.h"

#include "./Manager/GameManager.h"

#include "./Render/RenderToShader/RenderToShader.h"
#include "./Render/RenderToShader/RenderToShader2.h"


#include <ctime>
GameMain::GameMain(HINSTANCE hInstance, wstring title, POINT windowSize)
	: Window(hInstance, title, windowSize)
{
	srand((unsigned int)time(NULL));
}

GameMain::~GameMain()
{

}

void GameMain::Initialize()
{
	gameManager = new GameManager(Device::GetDevice());
//	shader = new SlimeShader(Device::GetDevice());

	D3DVIEWPORT9 viewport;
	Device::GetDevice()->GetViewport(&viewport);

	////postShader = new BlendShader(Device::GetDevice());//ReflectionShader
	//postShader = new BlurShader(Device::GetDevice());//BlurShader
	//postShader->PushPreShader(shader);


	D3DXMatrixOrthoOffCenterLH
	(
		&projection
		, 0.0f, (float)viewport.Width
		, (float)viewport.Height, 0.0f
		, -10.0f, 10.0f
	);

	Device::GetDevice()->SetTransform(D3DTS_PROJECTION, &projection);
	Device::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMatrixIdentity(&view);

	
}

void GameMain::Destroy()
{
	SAFE_DELETE(gameManager);
//	SAFE_DELETE(shader);
}

void GameMain::Update()
{
	Device::GetDevice()->SetTransform(D3DTS_VIEW, &view);
	gameManager->Update();
//	shader->Update();
//	postShader->Update();
	
}

void GameMain::Render()
{
	gameManager->Render();
	//shader->Render();
//	postShader->Render();

}
#pragma once

#include "./System/Window.h"

class GameManager;
//class Shader;
class GameMain : public Window
{
public:
	GameMain
	(
		HINSTANCE hInstance
		, wstring title
		, POINT windowSize
	);
	~GameMain();

	void Initialize();
	void Destroy();
	void Update();
	void Render();

private:
	//Shader* shader;
	//Shader* postShader;

	GameManager* gameManager;


	D3DXMATRIX view, projection;
};
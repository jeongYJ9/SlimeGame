#include "../stdafx.h"
#include "GameOverManager.h"

GameOverManager::GameOverManager(LPDIRECT3DDEVICE9 device) :Shader(device, L"./Render/Shader/default.hlsl")
, GameOverStart(false)
, CurrentScore(0), HighScore(0)
, RecentScore1(0), RecentScore2(0), RecentScore3(0)
, state(0) , Select(false)

{
	wstring fileNames[] =
	{
		L"./Textures/GameOver.png"	
		, L"./Textures/Score.png"
	};

	GameOverIMG.push_back(Texture::Add(fileNames[0],
	{ 0, 0 }, { 1024,768 }));

	for (float i = 0; i < 5; i++)//위치확인
	{
		Numbers.push_back(Texture::Add(fileNames[1],
		{ 0 + (i * 42), 0 }, { 42 + (i * 42) ,54 }));
	}//for(i)  (0 ,54), (42,54) 2번째 y축 값으로 54를 사용한다.

	for (float i = 0; i < 5; i++)//위치확인
	{
		Numbers.push_back(Texture::Add(fileNames[1],
		{ 0 + (i * 42), 72 }, { 42 + (i * 42) ,126 }));
	}//for(i)  (0 ,72), (42,126) 2번째 y축 값으로 72와 126을 사용한다.


}

GameOverManager::~GameOverManager()
{
}

void GameOverManager::Update()
{
	if (GameOverStart == true)
	{
		if (RecentScore1 != CurrentScore)
		{
			if (CurrentScore >= HighScore)
				HighScore = CurrentScore;//점수갱신

			RecentScore3 = RecentScore2;//최근점수 반영
			RecentScore2 = RecentScore1;
			RecentScore1 = CurrentScore;
		}
		GameOverStart = false;
	}
	
	if (Keyboard::GetInstance()->KeyDown(VK_RETURN))
	{
		Select = true;
	}
	//플레이어의 Dead가 true일시 활성화되는 업데이트
	//키보드 방향키 입력에 의해 어떤 버튼을 선택중인지 보여줌.
	//다시하기, 
}

void GameOverManager::Render()
{
	int tempNumber = 0;//렌더 내에서 사용되는 int
	int CurrentScorePositionX = 550;
	int CurrentScorePositionY = 340;
	int HighScorePositionX = 600;
	int HighScorePositionY = 440;

	//게임 오버화면 이미지 렌더
	//현재 점수 숫자 렌더
	//최고 기록점수 숫자 렌더
	//다시하기, 다른캐릭선택, 메인화면으로 버튼 3가지 렌더
	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);
	shader->Begin(NULL, NULL);
	{

		D3DXMatrixTranslation(&world, 0, 0, 0);
		Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		shader->SetTexture("_map", GameOverIMG[0]->GetTexture());
		shader->BeginPass(0);
		GameOverIMG[0]->Render();
		shader->EndPass();

		for (int i = 0; i < 3; i++)
		{
			
			switch (i)
			{
				case 0://100의 자리수 출력
					tempNumber = CurrentScore / 100;
					break;

				case 1://10의 자리수 출력
					tempNumber = CurrentScore / 10;
					break;

				case 2://1의 자리수 출력
					tempNumber = CurrentScore % 10;
					break;
			}
			D3DXMatrixTranslation(&world, CurrentScorePositionX +i*40, CurrentScorePositionY, 0);//위치를 다른곳에서 관리할수있게 변수로 빼서 여기에 변수를 넣어줘야됨.
			Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
			shader->SetTexture("_map", Numbers[tempNumber]->GetTexture());
			shader->BeginPass(0);
			Numbers[tempNumber]->Render();
			shader->EndPass();
		}

		for (int i = 0; i < 3; i++)
		{

			switch (i)
			{
				case 0://100의 자리수 출력
					tempNumber = HighScore / 100;
					break;

				case 1://10의 자리수 출력
					tempNumber = HighScore / 10;
					break;

				case 2://1의 자리수 출력
					tempNumber = HighScore % 10;
					break;
			}
			D3DXMatrixTranslation(&world, HighScorePositionX + i * 40, HighScorePositionY, 0);//위치를 다른곳에서 관리할수있게 변수로 빼서 여기에 변수를 넣어줘야됨.
			Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
			shader->SetTexture("_map", Numbers[tempNumber]->GetTexture());
			shader->BeginPass(0);
			Numbers[tempNumber]->Render();
			shader->EndPass();
		}
	}
	shader->End();
	
}

void GameOverManager::ResetData()
{
	state = 0;
	Select = false;
}


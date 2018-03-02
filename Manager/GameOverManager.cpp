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

	for (float i = 0; i < 5; i++)//��ġȮ��
	{
		Numbers.push_back(Texture::Add(fileNames[1],
		{ 0 + (i * 42), 0 }, { 42 + (i * 42) ,54 }));
	}//for(i)  (0 ,54), (42,54) 2��° y�� ������ 54�� ����Ѵ�.

	for (float i = 0; i < 5; i++)//��ġȮ��
	{
		Numbers.push_back(Texture::Add(fileNames[1],
		{ 0 + (i * 42), 72 }, { 42 + (i * 42) ,126 }));
	}//for(i)  (0 ,72), (42,126) 2��° y�� ������ 72�� 126�� ����Ѵ�.


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
				HighScore = CurrentScore;//��������

			RecentScore3 = RecentScore2;//�ֱ����� �ݿ�
			RecentScore2 = RecentScore1;
			RecentScore1 = CurrentScore;
		}
		GameOverStart = false;
	}
	
	if (Keyboard::GetInstance()->KeyDown(VK_RETURN))
	{
		Select = true;
	}
	//�÷��̾��� Dead�� true�Ͻ� Ȱ��ȭ�Ǵ� ������Ʈ
	//Ű���� ����Ű �Է¿� ���� � ��ư�� ���������� ������.
	//�ٽ��ϱ�, 
}

void GameOverManager::Render()
{
	int tempNumber = 0;//���� ������ ���Ǵ� int
	int CurrentScorePositionX = 550;
	int CurrentScorePositionY = 340;
	int HighScorePositionX = 600;
	int HighScorePositionY = 440;

	//���� ����ȭ�� �̹��� ����
	//���� ���� ���� ����
	//�ְ� ������� ���� ����
	//�ٽ��ϱ�, �ٸ�ĳ������, ����ȭ������ ��ư 3���� ����
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
				case 0://100�� �ڸ��� ���
					tempNumber = CurrentScore / 100;
					break;

				case 1://10�� �ڸ��� ���
					tempNumber = CurrentScore / 10;
					break;

				case 2://1�� �ڸ��� ���
					tempNumber = CurrentScore % 10;
					break;
			}
			D3DXMatrixTranslation(&world, CurrentScorePositionX +i*40, CurrentScorePositionY, 0);//��ġ�� �ٸ������� �����Ҽ��ְ� ������ ���� ���⿡ ������ �־���ߵ�.
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
				case 0://100�� �ڸ��� ���
					tempNumber = HighScore / 100;
					break;

				case 1://10�� �ڸ��� ���
					tempNumber = HighScore / 10;
					break;

				case 2://1�� �ڸ��� ���
					tempNumber = HighScore % 10;
					break;
			}
			D3DXMatrixTranslation(&world, HighScorePositionX + i * 40, HighScorePositionY, 0);//��ġ�� �ٸ������� �����Ҽ��ְ� ������ ���� ���⿡ ������ �־���ߵ�.
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


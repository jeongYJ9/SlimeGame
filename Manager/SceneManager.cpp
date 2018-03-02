#include "../stdafx.h"
#include "SceneManager.h"//�̸������ϵ� ��� ������ �ڱ��ڽ��� ������.




#include "../Render/Shader/Shader.h"
#include "../Object/SlimeShader.h"
#include "../Object/Map.h"
#include "../Object/Disaster.h"
#include "../Object/Skill.h"

SceneManager::SceneManager(LPDIRECT3DDEVICE9 device) :
	curTime(0), SceneState(0), SceneTimeCheck(true), SceneTime(0)
	,GameOver(false) , Score(0)
{


	state = (SlimeState::Idle_Front);
	slimePosition={ 0,0 };
	slime = new SlimeShader(Device::GetDevice());
	map = new Map(Device::GetDevice());
	disaster = new Disaster(Device::GetDevice());
	skill = new Skill(Device::GetDevice());
	bar = new ProgressBar(Device::GetDevice());


	bar->SetAdjestLocation(Point(0, -40));
	bar->SetProgressBar(slime->pGetRectLocation());
	bar->SetHeight(1.0f);
	bar->SetColor(0xFFFFFFFF);

	barEnergy = new ProgressBar(Device::GetDevice());
	barEnergy->SetAdjestLocation(Point(0, 0));
	barEnergy->SetProgressBar(slime->pGetUIRectLocation());
	barEnergy->SetWidth((float)slime->GetSlimeEnergy() / (float)slime->slimeMaxEnergy);
	barEnergy->SetHeight(1.0f);

	int a = 0;

}

SceneManager::~SceneManager()
{
	SAFE_DELETE(barEnergy);
	SAFE_DELETE(bar);
	SAFE_DELETE(disaster);
	SAFE_DELETE(map);
	SAFE_DELETE(slime);
}

void SceneManager::Initialize()
{
	map->Initialize();
//	disaster->Initialize();


	
}

void SceneManager::Destroy()
{

}

void SceneManager::Update()
{
	
	moveUpdate();//ĳ���Ϳ� map�� �浹�� ���� ����ó��
	
	disasterAndSlime();
	//ĳ���Ϳ� disaster �浹�� ĳ������ ���¸� Dead�� ���� �Լ�
	disasterAndMap();
	//disaster �� map�� Block�� �浹�� ������ �Լ�.

	slime->Update();//ĳ������ �̹����� ���¿� ���� �����ϱ����� ó��
	barEnergy->Update();//UI�� ���� bar�� ���� ������Ʈ
	disaster->Update();

	SceneMain();//�� ������ ��ǲ�� ���� �Լ�ȣ�ⵥ����ó��

	SceneUpdate();//���� ������Ʈ����, ��ü�� ���ذ� ����,����,�Ҹ���


	map->Update();

	//bar

	bar->Update();


	
}

void SceneManager::Render()
{


	if (Keyboard::GetInstance()->KeyPress('W'))
		map->RenderPrev();

	map->Render();
	slime->Render();
	disaster->Render();

	Device::SetWorldLocation(slime->GetRectLocation().location, 0.0f);
	if (isPress == true)
	{
		bar->Render();
	}
	barEnergy->Render();


}




//�Ʒ� �ڵ�� ���� ������Ʈ�� �� ������Ʈ �κ�

void SceneManager::moveUpdate()
{
	if (isPress == true)
		return ;

	slimePosition = slime->GetslimePosition();

	//������ �������� �����ͼ� �����ϰ� �ٽ� �����ش�.
	
	int Tolerance = 2;//�������� ���� ����

	//SlimeState prev = state;

	if (Keyboard::GetInstance()->KeyPress(VK_UP) && Keyboard::GetInstance()->KeyPress(VK_DOWN)) {}
	else if (Keyboard::GetInstance()->KeyPress(VK_UP) && Keyboard::GetInstance()->KeyPress(VK_CONTROL))
	{
		state = SlimeState::Move_Back;
	}
	else if (Keyboard::GetInstance()->KeyPress(VK_UP))
	{

		state = SlimeState::Move_Back;
		int BackType = map->GetTileState(state, slimePosition);

		int BackRightType = map->GetTileState(state, D3DXVECTOR2(slimePosition.x + 31, slimePosition.y));
		int BackLeftType = map->GetTileState(state, D3DXVECTOR2(slimePosition.x - 31, slimePosition.y));
		if (BackType == 0)
		{
			if ((BackRightType == 1 && (int)(slimePosition.x) % 32<Tolerance))
				slimePosition.y -= 100 * Frames::Get()->TimeElapsed();
			else if ((BackRightType == 1 && (int)(slimePosition.x) % 32 > Tolerance));
			else
				slimePosition.y -= 100 * Frames::Get()->TimeElapsed();
		}
		else if ((BackRightType == 0) && (BackType == 1) && ((int)(slimePosition.x) % 32 > 31 - Tolerance))
		{
			slimePosition.y -= 100 * Frames::Get()->TimeElapsed();
		}
	}
	else if (Keyboard::GetInstance()->KeyPress(VK_DOWN) && Keyboard::GetInstance()->KeyPress(VK_CONTROL))
	{
		state = SlimeState::Move_Front;
	}
	else if (Keyboard::GetInstance()->KeyPress(VK_DOWN))
	{
		state = SlimeState::Move_Front;
		//int a;

		//a = state;
		int FrontType = map->GetTileState(state, slimePosition);
		int FrontRightType = map->GetTileState(state, D3DXVECTOR2(slimePosition.x + 31, slimePosition.y));
		int FrontLeftType = map->GetTileState(state, D3DXVECTOR2(slimePosition.x - 31, slimePosition.y));
		if (FrontType == 0)
		{
			if ((FrontRightType == 1 && (int)(slimePosition.x) % 32<Tolerance))
				slimePosition.y += 100 * Frames::Get()->TimeElapsed();
			else if ((FrontRightType == 1 && (int)(slimePosition.x) % 32 > Tolerance));
			else
				slimePosition.y += 100 * Frames::Get()->TimeElapsed();
		}
		else if ((FrontRightType == 0) && (FrontType == 1) && ((int)(slimePosition.x) % 32 > 31 - Tolerance))
		{
			slimePosition.y += 100 * Frames::Get()->TimeElapsed();
		}
	}

	if (Keyboard::GetInstance()->KeyPress(VK_RIGHT) && Keyboard::GetInstance()->KeyPress(VK_LEFT))
	{
	}
	else if (Keyboard::GetInstance()->KeyPress(VK_LEFT) && Keyboard::GetInstance()->KeyPress(VK_CONTROL))
	{
		state = SlimeState::Move_Left;
	}
	else if (Keyboard::GetInstance()->KeyPress(VK_LEFT))
	{
		state = SlimeState::Move_Left;
		
		int LeftType = map->GetTileState
		(state, D3DXVECTOR2(slimePosition.x + 32, slimePosition.y));
		int LeftFrontType = map->GetTileState
		(state, D3DXVECTOR2(slimePosition.x + 32, slimePosition.y + 32));
		if (LeftType == 0)//�򰥸��� 
		{
			if ((LeftFrontType == 1) && (int)(slimePosition.x) % 32 < 32 - Tolerance)
				slimePosition.x -= 100 * Frames::Get()->TimeElapsed();
			else if (LeftFrontType == 0)
				slimePosition.x -= 100 * Frames::Get()->TimeElapsed();
			else if ((LeftFrontType == 1) && (int)(slimePosition.y) % 32 < Tolerance)
				slimePosition.x -= 100 * Frames::Get()->TimeElapsed();
		}
		else if (LeftType == 1)
		{
			if (((LeftFrontType == 0) && (int)(slimePosition.y) % 32 >31 - Tolerance))
				slimePosition.x -= 100 * Frames::Get()->TimeElapsed();
		}

	}
	else if (Keyboard::GetInstance()->KeyPress(VK_RIGHT) && Keyboard::GetInstance()->KeyPress(VK_CONTROL))
	{
		state = SlimeState::Move_Right;
	}
	else if (Keyboard::GetInstance()->KeyPress(VK_RIGHT))
	{

		state = SlimeState::Move_Right;

		int RightType = map->GetTileState
		(state, D3DXVECTOR2(slimePosition.x, slimePosition.y));
		int RightFrontType = map->GetTileState
		(state, D3DXVECTOR2(slimePosition.x, slimePosition.y + 32));

		if (RightType == 0)
		{
			if ((RightFrontType == 1) && (int)(slimePosition.y) % 32 > Tolerance);

			else
				slimePosition.x += 100 * Frames::Get()->TimeElapsed();
		}
		else if (RightType == 1)
		{
			if (RightFrontType == 0 && (int)(slimePosition.y) % 32 >31 - Tolerance)
				slimePosition.x += 100 * Frames::Get()->TimeElapsed();
		}




	}


	if (Keyboard::GetInstance()->KeyUp(0x26))
		state = SlimeState::Idle_Back;
	else if (Keyboard::GetInstance()->KeyUp(0x28))
		state = SlimeState::Idle_Front;
	else if (Keyboard::GetInstance()->KeyUp(0x25))
		state = SlimeState::Idle_Left;
	else if (Keyboard::GetInstance()->KeyUp(0x27))
		state = SlimeState::Idle_Right;



	slime->Setstate(state);

	//������������ ������ ������ �ٽ� �����ش�. �̰� �ټ��� ĳ���͸� �����ؾߵǴ°�� �������������� �Ǵܵ�.
	slime->SetslimePosition(slimePosition);
	slime->SetRectLocation_location(slimePosition);
}

void SceneManager::disasterAndSlime()
{
	bool Dead = false;
	bool Active;
	for (int i = 0; i < 50; i++)
	{
		Active = disaster->GetDisasterDataActive(i);
		if (Active == true)
		{
			Dead = Rect::Intersect(slime->GetRectCollision(), disaster->GetRect(i));
			if (Dead == true)
			{
				//slime->SetToneColor({ 1,0,0,0 });
				//slime->SetColorWeight(0.5f);
				GameOver = true;
				//slime->Setstate(9);
				return;
			}
		}
	/*	else
		{
			slime->SetToneColor({ 1,1,0,0 });
			slime->SetColorWeight(0.5f);
		}*/
	}
	

}

void SceneManager::disasterAndMap()
{
	bool check = false;


	
	for (int i = 0; i < 50; i++)
	{
		if (disaster->GetDisasterDataActive(i) == true)
		{
			for (int j = 0; j < map->GetMapSizeX()*map->GetMapSizeY(); j++)
			{
				
				if (map->GetTileType(j) == 1)
				{
					int posX = j % (map->GetMapSizeX());
					int posY = j / (map->GetMapSizeX());
					Rect MapRect;
					MapRect.location = { (float)posX * 32, (float)posY * 32 };
					MapRect.size = { 32,32 };

					check = Rect::Intersect(MapRect, disaster->GetRect(i));

					if (check == true)
					{//�̺κ� �����ؾߵ�
						Score++;

						disaster->DestroyDisasterData(i);
						int tempHp = map->GetTileHp(j);
						tempHp-=1;
						if (tempHp < 1)
							map->ResetData(j);
						else 
							map->SetTileHp(j, tempHp);

					}
					check = false;
				}//if

			}//for(j)
		}//if
	
	}//for(i)
}

bool SceneManager::GetGameOver()
{
	return GameOver;
}

void SceneManager::SceneUpdate()
{
	int _scene0 = 100;//���Ÿ�� ��ü����
	int _scene1 = 10000;//����Ÿ�� ����Ȱ��
	int _scene2 = 10000;//�̺�Ʈ�߻� ��������
	int _scene3 = 10000;//�𷡹ٶ�+ ���� ���ƿ�
	int _temp = 0;// ���� �ð� �������� �༮��
	switch (SceneState)//�Ź� temp�� ���� �־��ִ°͵� �̻��غ���. �ϴ� �̴��
	{
		case 0: _temp = _scene0; break;
		case 1: _temp = _scene1; break;
		case 2: _temp = _scene2; break;
		case 3: _temp = _scene3; break;
		default:
			break;
	}

	if ((SceneTimeCheck == true)&& (timeGetTime() - SceneTime > (DWORD)_temp))
	{		//���� Ÿ��üũ�� true �̸鼭 �ð��� ������ ���������� �ѱ�� false��
		if (SceneState < 3)//3�� ���̵ȴ�. scene count
			SceneState++;
		else
			SceneState = 0;
		SceneTime = timeGetTime();
		SceneTimeCheck = false;
	}

	if (SceneTimeCheck == false)//��ü����Ÿ��
	{
	//false�϶� ����ǰ� ������ true�� �ȴ�.
			//true�϶��� �ƹ� ���۵� ���ϰ� �ð��̵ɋ����� ��ٸ��� �ð��̵Ǹ� ����
		switch (SceneState)
		{
	
			case 0 ://�������Դϴ�.
				
				map->CreateWater();
				map->CreateWater();
				map->CreateWater();
				break;
			case 1://��ü�� �����ǰ� ����Ȱ���ϴ� �ð��Դϴ�.
				disaster->CreateDisaster(Math::RandomRange(0,3));
				map->CreateWater();
				//AcetDelay ����
				break;
			case 2:

				map->CreateWater();
				//AcetDelay 2�� ������
				break;
			case 3:
		
				map->CreateWater();
				break;

			default:
				break;
		}
		SceneTimeCheck = true;
	}

}

void SceneManager::EnergyUpdate(int Energy)//���� int ���������� ���� �������� ���������� ������Ŵ
{
	int tempEnergy = slime->GetSlimeEnergy();
	tempEnergy += Energy;
	slime->SetSlimeEnergy(tempEnergy);
	barEnergy->SetWidth((float)slime->GetSlimeEnergy() / (float)slime->slimeMaxEnergy);
}

void SceneManager::SceneResetData(int Character)
{
	skill->SetStartGame(Character);
	isPress = false;
	slime->ResetData();
	Score = 0;//���� �ʱ�ȭ
	disaster->ResetData();
	GameOver = false;
	SceneState = 0;
	SceneTimeCheck=true;
	state = SlimeState::Idle_Front;
	//���� Ż���ϴ� ���� GameOver�� false�� �ʱ�ȭ�ؾߵ�.

	for (int i = 0; i < map->GetMapSizeX() * map->GetMapSizeY(); i++)
	{
		map->ResetData(i);
	}
}

void SceneManager::SceneMain()
{
	if (Keyboard::GetInstance()->KeyDown(VK_SHIFT))
	{
		int TempEnergy;
		TempEnergy = slime->GetSlimeEnergy();

		if (TempEnergy > 5 && skill->GetActive()==false)
		{
			TempEnergy -= 5;
			slime->SetSlimeEnergy(TempEnergy);
			skill->ActiveSkill();

			barEnergy->SetWidth((float)slime->GetSlimeEnergy() / (float)slime->slimeMaxEnergy);
		}
		
	}

	if (Keyboard::GetInstance()->KeyDown('Q'))
	{
		isPress = true;
		curTime = timeGetTime();
	}
	if ((Keyboard::GetInstance()->KeyPress('Q')) && (slime->GetLiquidState() == 0) && (slime->GetLiquidState() == 0) && map->GetTileType(slime->Getstate(), slime->GetslimePosition()) == 5)
	{

		float Weight = (float)(timeGetTime() - curTime) / (float)slime->LiquidTime;//��ư ������������ ����Ʈ���� 0���� 1�� ������ �ö�.

		bar->SetWidth(Weight);
		slime->SetColorWeight(Weight);

		map->GetLiquidColor(slime->Getstate(), slime->GetslimePosition(), slime->Get_LiquidState(), slime->Get_LiquidColor());
		switch (slime->GetLiquidColor())
		{
			case 0:
				slime->SetToneColor({ 1,1,1,0 });
				break;
			case 4:
				slime->SetToneColor({ 0,1,1,0 });
				break;
			case 5:
				slime->SetToneColor({ 1,0,1,0 });
				break;
			case 6:
				slime->SetToneColor({ 1,1,0,0 });
				break;

			default:
				break;
		}

		if (timeGetTime() - curTime > slime->LiquidTime)//���� ���� 100�� �ൿ�ð����� ���ǵǾ���
		{
			map->GetLiquid(slime->Getstate(), slime->GetslimePosition(), slime->Get_LiquidState(), slime->Get_LiquidColor());

			bar->SetWidth(0.0f);
			isPress = false;
			EnergyUpdate(-2);//�Լ� �ȿ� �� ���ڴ� �ൿ�� ���� ������ ��������
		}
	}

	if (Keyboard::GetInstance()->KeyDown('W'))
	{
		isPress = true;
		curTime = timeGetTime();

		map->CreateBuildPrev(slime->Getstate(), slime->GetslimePosition(), slime->GetLiquidColor());

	}
	if ((Keyboard::GetInstance()->KeyPress('W')) && (slime->GetLiquidState() == 1))
	{
		//������ �ð���ŭ Press�� ���������� �Էµɽ� �Ǽ��Լ������� ��ü�� �÷��� ���� ���� �ʱ�ȭ
		//press�� �ߴܵɽ� E PressTime �ʱ�ȭ��.
		float Weight = (float)(timeGetTime() - curTime) / (float)slime->BuildTime;//��ư ������������ ����Ʈ���� 0���� 1�� ������ �ö�.

		bar->SetWidth(Weight);

		if (timeGetTime() - curTime > slime->BuildTime)
		{
			map->CreateBuild(slime->Getstate(), slime->GetslimePosition(), slime->GetLiquidColor());
			slime->SetLiquidState(0);
			slime->SetToneColor({ 1,1,1,0 });
			bar->SetWidth(0.0f);

			EnergyUpdate(-5);//�Լ� �ȿ� �� ���ڴ� �ൿ�� ���� ������ ��������
		}
	}

	if (Keyboard::GetInstance()->KeyDown('E'))
	{
		isPress = true;
		curTime = timeGetTime();
	}
	if ((Keyboard::GetInstance()->KeyPress('E')) && (slime->GetLiquidState() == 1))
	{
		//������ �ð���ŭ Press�� ���������� �Էµɽ� �Ǽ��Լ������� ��ü�� �÷��� ���� ���� �ʱ�ȭ
		//press�� �ߴܵɽ� E PressTime �ʱ�ȭ��.
		float Weight = (float)(timeGetTime() - curTime) / (float)slime->EatTime;//��ư ������������ ����Ʈ���� 0���� 1�� ������ �ö�.

		bar->SetWidth(Weight);
		slime->SetColorWeight(1 - Weight);

		if (timeGetTime() - curTime > slime->EatTime)
		{
			int TempEnergy = slime->GetSlimeEnergy();
			switch (slime->GetLiquidColor())
			{
				case 0: 	break;
				case 1: TempEnergy += slime->R_energy; break;
				case 2: TempEnergy += slime->G_energy;	break;
				case 3: TempEnergy += slime->B_energy;	break;
				case 4: TempEnergy += slime->C_energy;	break;
				case 5: TempEnergy += slime->M_energy;	break;
				case 6: TempEnergy += slime->Y_energy;	break;
				case 7: TempEnergy += slime->K_energy;	break;
			}

			slime->SetSlimeEnergy(TempEnergy);
			slime->SetLiquidState(0);
			slime->SetToneColor({ 1,1,1,0 });
			bar->SetWidth(0.0f);
			barEnergy->SetWidth((float)slime->GetSlimeEnergy() / (float)slime->slimeMaxEnergy);
		}
	}

	if (Keyboard::GetInstance()->KeyUp('Q') || Keyboard::GetInstance()->KeyUp('W') || Keyboard::GetInstance()->KeyUp('E'))
	{

		isPress = false;
		if (slime->GetLiquidState() != 1)
		{
			slime->SetToneColor({ 1,1,1,0 });
		}
		curTime = 0;
		bar->SetWidth(0.0f);
	}
}

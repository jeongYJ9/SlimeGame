#include "../stdafx.h"
#include "SceneManager.h"//미리컴파일된 헤더 다음엔 자기자신이 먼저옴.




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
	
	moveUpdate();//캐릭터와 map간 충돌에 관한 정보처리
	
	disasterAndSlime();
	//캐릭터와 disaster 충돌시 캐릭터의 상태를 Dead로 만들 함수
	disasterAndMap();
	//disaster 와 map의 Block의 충돌을 관리할 함수.

	slime->Update();//캐릭터의 이미지를 상태에 따라 갱신하기위한 처리
	barEnergy->Update();//UI로 사용될 bar에 대한 업데이트
	disaster->Update();

	SceneMain();//씬 내에서 인풋에 의한 함수호출데이터처리

	SceneUpdate();//씬의 업데이트정보, 액체와 재해가 생성,제어,소멸함


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




//아래 코드는 무브 업데이트랑 씬 업데이트 부분

void SceneManager::moveUpdate()
{
	if (isPress == true)
		return ;

	slimePosition = slime->GetslimePosition();

	//슬라임 포지션을 가져와서 연산하고 다시 갖다준다.
	
	int Tolerance = 2;//허용범위에 대한 숫자

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
		if (LeftType == 0)//헷갈리네 
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

	//슬라임포지션 연산이 끝난거 다시 갖다준다. 이거 다수의 캐릭터를 조종해야되는경우 좋지않은것으로 판단됨.
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
					{//이부분 수정해야됨
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
	int _scene0 = 100;//대기타임 액체생성
	int _scene1 = 10000;//생존타임 생산활동
	int _scene2 = 10000;//이벤트발생 생존방해
	int _scene3 = 10000;//모래바람+ 바위 날아옴
	int _temp = 0;// 씬의 시간 관리해줄 녀석들
	switch (SceneState)//매번 temp에 값을 넣어주는것도 이상해보임. 일단 이대로
	{
		case 0: _temp = _scene0; break;
		case 1: _temp = _scene1; break;
		case 2: _temp = _scene2; break;
		case 3: _temp = _scene3; break;
		default:
			break;
	}

	if ((SceneTimeCheck == true)&& (timeGetTime() - SceneTime > (DWORD)_temp))
	{		//만약 타임체크가 true 이면서 시간이 지나면 다음씬으로 넘기고 false로
		if (SceneState < 3)//3이 끝이된다. scene count
			SceneState++;
		else
			SceneState = 0;
		SceneTime = timeGetTime();
		SceneTimeCheck = false;
	}

	if (SceneTimeCheck == false)//액체생성타임
	{
	//false일때 실행되고 실행후 true가 된다.
			//true일때는 아무 동작도 안하고 시간이될떄까지 기다린후 시간이되면 실행
		switch (SceneState)
		{
	
			case 0 ://대기상태입니다.
				
				map->CreateWater();
				map->CreateWater();
				map->CreateWater();
				break;
			case 1://액체가 생성되고 생산활동하는 시간입니다.
				disaster->CreateDisaster(Math::RandomRange(0,3));
				map->CreateWater();
				//AcetDelay 정상
				break;
			case 2:

				map->CreateWater();
				//AcetDelay 2배 느리게
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

void SceneManager::EnergyUpdate(int Energy)//들어온 int 에너지량에 의해 슬라임의 에너지량을 증감시킴
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
	Score = 0;//점수 초기화
	disaster->ResetData();
	GameOver = false;
	SceneState = 0;
	SceneTimeCheck=true;
	state = SlimeState::Idle_Front;
	//씬을 탈출하는 변수 GameOver를 false로 초기화해야됨.

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

		float Weight = (float)(timeGetTime() - curTime) / (float)slime->LiquidTime;//버튼 누르고있으면 웨이트값이 0에서 1로 서서히 올라감.

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

		if (timeGetTime() - curTime > slime->LiquidTime)//여기 숫자 100이 행동시간으로 정의되야함
		{
			map->GetLiquid(slime->Getstate(), slime->GetslimePosition(), slime->Get_LiquidState(), slime->Get_LiquidColor());

			bar->SetWidth(0.0f);
			isPress = false;
			EnergyUpdate(-2);//함수 안에 들어갈 숫자는 행동에 의해 증감될 에너지량
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
		//설정한 시간만큼 Press가 연속적으로 입력될시 건설함수실행후 액체와 컬러에 대한 정보 초기화
		//press가 중단될시 E PressTime 초기화됨.
		float Weight = (float)(timeGetTime() - curTime) / (float)slime->BuildTime;//버튼 누르고있으면 웨이트값이 0에서 1로 서서히 올라감.

		bar->SetWidth(Weight);

		if (timeGetTime() - curTime > slime->BuildTime)
		{
			map->CreateBuild(slime->Getstate(), slime->GetslimePosition(), slime->GetLiquidColor());
			slime->SetLiquidState(0);
			slime->SetToneColor({ 1,1,1,0 });
			bar->SetWidth(0.0f);

			EnergyUpdate(-5);//함수 안에 들어갈 숫자는 행동에 의해 증감될 에너지량
		}
	}

	if (Keyboard::GetInstance()->KeyDown('E'))
	{
		isPress = true;
		curTime = timeGetTime();
	}
	if ((Keyboard::GetInstance()->KeyPress('E')) && (slime->GetLiquidState() == 1))
	{
		//설정한 시간만큼 Press가 연속적으로 입력될시 건설함수실행후 액체와 컬러에 대한 정보 초기화
		//press가 중단될시 E PressTime 초기화됨.
		float Weight = (float)(timeGetTime() - curTime) / (float)slime->EatTime;//버튼 누르고있으면 웨이트값이 0에서 1로 서서히 올라감.

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

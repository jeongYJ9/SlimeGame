#include"../stdafx.h"
#include"Disaster.h"


Disaster::Disaster(LPDIRECT3DDEVICE9 device) :Shader(device, L"./Render/Shader/default.hlsl")
, disasterDataSize(50), disasterSpeed(150), Direction(0)
, MapHeight(768), MapWidth(1024)
{
	wstring fileNames[] =
	{
		L"./Textures/disasterImg1.png"
		//,L"./Textures/slime.png"
	};

	//그림을 따올 이미지에서 가로길이, 세로길이, 갯수가 일정하면 미리 정의해놓는게 편할것같다.
	//가로로 변화하는 애니메이션
	//시작위치, 끝위치, 가로크기, 세로크기

	int startPosX = 0;//(0,128)좌표에서 32사이즈의 이미지가 나열되어있음.
	int startPosY = 128;
	int sizeX = 32;
	int sizeY = 32;
	int clipsWidthCount = 4;
	int clipsHeightCount = 1;

	for (float j = 0; j < clipsWidthCount; j++)
	{
		for (float i = 0; i < clipsHeightCount; i++)
		{
			disasterImg.push_back(Texture::Add(fileNames[0],
			{ startPosX + (j * sizeX), startPosY + (i * sizeY) }, { sizeX + (j * sizeX) ,startPosY + ((i+1) * sizeY) }));
		}
	}

	startPosX = 0;
	startPosY = 128 + 32;
	for (float j = 0; j < clipsWidthCount; j++)
	{
		for (float i = 0; i < clipsHeightCount; i++)
		{
			disasterImg.push_back(Texture::Add(fileNames[0],
			{ startPosX + (j * sizeX), startPosY + (i * sizeY) }, { sizeX + (j * sizeX) ,startPosY + ((i + 1) * sizeY) }));
		}
	}
	vector<AnimationClip> clips;

	for (int i = 0; i < 2; i++)//move
	{
		clips.clear();
		clips.push_back(AnimationClip(disasterImg[0 + i * 4], 100));
		clips.push_back(AnimationClip(disasterImg[1 + i * 4], 75));
		clips.push_back(AnimationClip(disasterImg[2 + i * 4], 75));
		clips.push_back(AnimationClip(disasterImg[3 + i * 4], 100));

		actions32size.push_back(new Animator({ 0,0 }, &clips, AniRepeatType_Loop));
	}//for(i)
	 ////////////////////////////////////////////////////////////
	for (int i = 0; i < 50; i++)
	{
		disasterData[i].Active = false;
		disasterData[i].Hp = 0;
		disasterData[i].state = 1;
		disasterData[i].disasterPosition = { 0,0 };
		disasterData[i].size = { 32,32 };
	}//for(i) disasterData배열의 초기화.

	rectCollision.location = Point(400, 368);
	rectCollision.size = Size(32, 32);//충돌체크할 Rect를 하나 만들어둔후
									  //for문을 통해서 Rect에 정보를 넣고 하나씩 플레이어와 비교한다.
}			

Disaster::~Disaster()
{
	for (size_t i = 0; i < actions32size.size(); i++)
		SAFE_DELETE(actions32size[i]);
}

//void Disaster::CreateDisasterData()
//{
//}
//
//void Disaster::Initialize()
//{
//}

void Disaster::Update()
{
	actions32size[disasterData[0].state]->Update();

	
	Crash();//충돌체크
	Move();//이동과 비활성화
}

void Disaster::Render()
{
	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);


	shader->Begin(NULL, NULL);
	{
		for (int i = 0; i < disasterDataSize; i++)
		{
			if (disasterData[i].Active == true) 
			{
				D3DXMatrixTranslation(&world, disasterData[i].disasterPosition.x, disasterData[i].disasterPosition.y, 0);
				Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
				
				Texture* texture = actions32size[disasterData[i].state]->GetCurrentClipTexture();

				shader->SetTexture("_map", texture->GetTexture());
				shader->BeginPass(0);
				actions32size[disasterData[i].state]->Render();
				shader->EndPass();
			}//if(Active)
		}//for(i)
	}//shader->Begin()
	shader->End();
}

void Disaster::Crash()//외부에서 맵 정보를 가져와야함.
{//맵의 block과 충돌시 block의 hp 감소시킴
//플레이어와 충돌시 플레이어 패배함.

	

}

void Disaster::Move()//이동과 비활성화
{
	for (int i = 0; i < disasterDataSize; i++)
	{
		if (disasterData[i].Active == true)//활성화상태시 무브함수호출?
		{
			switch (Direction)// Direction//0Front 1Right 2Left 3Back
			{
				case 0:
					disasterData[i].disasterPosition.y += disasterSpeed *Frames::Get()->TimeElapsed();
					//해당 방향으로 객체 이동시킴
					if (disasterData[i].disasterPosition.y > MapHeight)
						disasterData[i].Active = false;
					//해당 방향에 대해 맵 사이즈 벗어나면 비활성화 상태로 변경
					break;
				case 1:
					disasterData[i].disasterPosition.x += disasterSpeed *Frames::Get()->TimeElapsed();
					if (disasterData[i].disasterPosition.x > MapWidth)
						disasterData[i].Active = false;
					break;
				case 2:
					disasterData[i].disasterPosition.x -= disasterSpeed *Frames::Get()->TimeElapsed();
					if (disasterData[i].disasterPosition.x < 0)
						disasterData[i].Active = false;
					break;
				case 3:
					disasterData[i].disasterPosition.y -= disasterSpeed *Frames::Get()->TimeElapsed();
					if (disasterData[i].disasterPosition.y < -32)
						disasterData[i].Active = false;
					break;
			}

			
		}


	}


}

void Disaster::CreateDisaster(int direction)
{
	Direction = direction;//밖에서 들어온거 한번 넣어줌. 그리고 계속 사용.
	int createCount = Math::RandomRange(20, 30);
	createCount = 50;//확인을 위해 최대갯수로 고정해둠.
	
	for (int i = 0; i < createCount; i++)
	{
		switch (Direction)// Direction//0Front 1Right 2Left 3Back
		{
			case 0:
				disasterData[i].disasterPosition.x = (float)Math::RandomRange(0, MapWidth);
				disasterData[i].disasterPosition.y = -((float)Math::RandomRange(64, MapWidth));
				disasterData[i].Active = true;//-64는 큰사이즈 돌이 생성되도 안짤릴범위
				break;
			case 1:
				disasterData[i].disasterPosition.x = -((float)Math::RandomRange(64, MapWidth));
				disasterData[i].disasterPosition.y = ((float)Math::RandomRange(0, MapHeight));
				disasterData[i].Active = true;//오른쪽으로 날아가는 오브제들
				break;
			case 2:
				disasterData[i].disasterPosition.x = (float)Math::RandomRange(MapWidth, MapWidth*2);
				disasterData[i].disasterPosition.y = (float)Math::RandomRange(0, MapHeight-64);
				disasterData[i].Active = true;//왼쪽으로 날아갈 오브제들
				break;
			case 3:
				disasterData[i].disasterPosition.x = (float)Math::RandomRange(0, MapWidth);
				disasterData[i].disasterPosition.y = (float)Math::RandomRange(MapHeight, MapHeight*2);
				disasterData[i].Active = true;//위로 올라갈 오브제 프론트의 반대.
				break;
		}
	}
	
}

void Disaster::ResetData()
{
	for (int i = 0; i < disasterDataSize; i++)
	{
		disasterData[i].Active = false;
		disasterData[i].Hp = 0;
		disasterData[i].state = 1;
		disasterData[i].disasterPosition = { 0,0 };
		disasterData[i].size = { 32,32 };
	}//for(i) disasterData배열의 초기화.
}


//설정된 값에 의해 애니메이션이 렌더된다.

//애니메이션 클립 관련 주석
//for (int i = 0; i < 4; i++)//Idle
//{
//	clips.clear();
//	clips.push_back(AnimationClip(disasterImg[0 + i * 5], 200));
//	actions32size.push_back(new Animator(slimePosition, &clips, AniRepeatType_End));
//}//for(i)

//for (int i = 0; i < 1; i++)//move
//{
//	clips.clear();
//	clips.push_back(AnimationClip(disasterImg[0 + i * 5], 75));//75시간
//	clips.push_back(AnimationClip(disasterImg[1 + i * 5], 75));
//	clips.push_back(AnimationClip(disasterImg[2 + i * 5], 75));
//	clips.push_back(AnimationClip(disasterImg[3 + i * 5], 100));
//	clips.push_back(AnimationClip(disasterImg[4 + i * 5], 150));
//	actions32size.push_back(new Animator(slimePosition, &clips, AniRepeatType_Loop));
//}//for(i)


//void Disaster::Render()
//{
//	D3DXMATRIX world;
//
//	D3DXMatrixIdentity(&world);
//
//
//	shader->Begin(NULL, NULL);
//	{
//		for (int i = 0; i < disasterDataSize; i++)
//		{
//			//if (disasterData[i].Active == true) 
//			{
//				D3DXMatrixTranslation(&world, disasterData[i].disasterPosition.x, disasterData[i].disasterPosition.y, 0);
//				Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
//
//
//
//				Texture* texture = actions32size[disasterData[i].state]->GetCurrentClipTexture();
//				//disasterImg//actions32size
//
//				shader->SetTexture("_map", texture->GetTexture());
//				shader->BeginPass(0);
//				actions32size[disasterData[i].state]->Render();
//				shader->EndPass();
//			}//if(Active)
//		}//for(i)
//	}//shader->Begin()
//	shader->End();
//}
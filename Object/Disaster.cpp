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

	//�׸��� ���� �̹������� ���α���, ���α���, ������ �����ϸ� �̸� �����س��°� ���ҰͰ���.
	//���η� ��ȭ�ϴ� �ִϸ��̼�
	//������ġ, ����ġ, ����ũ��, ����ũ��

	int startPosX = 0;//(0,128)��ǥ���� 32�������� �̹����� �����Ǿ�����.
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
	}//for(i) disasterData�迭�� �ʱ�ȭ.

	rectCollision.location = Point(400, 368);
	rectCollision.size = Size(32, 32);//�浹üũ�� Rect�� �ϳ� ��������
									  //for���� ���ؼ� Rect�� ������ �ְ� �ϳ��� �÷��̾�� ���Ѵ�.
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

	
	Crash();//�浹üũ
	Move();//�̵��� ��Ȱ��ȭ
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

void Disaster::Crash()//�ܺο��� �� ������ �����;���.
{//���� block�� �浹�� block�� hp ���ҽ�Ŵ
//�÷��̾�� �浹�� �÷��̾� �й���.

	

}

void Disaster::Move()//�̵��� ��Ȱ��ȭ
{
	for (int i = 0; i < disasterDataSize; i++)
	{
		if (disasterData[i].Active == true)//Ȱ��ȭ���½� �����Լ�ȣ��?
		{
			switch (Direction)// Direction//0Front 1Right 2Left 3Back
			{
				case 0:
					disasterData[i].disasterPosition.y += disasterSpeed *Frames::Get()->TimeElapsed();
					//�ش� �������� ��ü �̵���Ŵ
					if (disasterData[i].disasterPosition.y > MapHeight)
						disasterData[i].Active = false;
					//�ش� ���⿡ ���� �� ������ ����� ��Ȱ��ȭ ���·� ����
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
	Direction = direction;//�ۿ��� ���°� �ѹ� �־���. �׸��� ��� ���.
	int createCount = Math::RandomRange(20, 30);
	createCount = 50;//Ȯ���� ���� �ִ밹���� �����ص�.
	
	for (int i = 0; i < createCount; i++)
	{
		switch (Direction)// Direction//0Front 1Right 2Left 3Back
		{
			case 0:
				disasterData[i].disasterPosition.x = (float)Math::RandomRange(0, MapWidth);
				disasterData[i].disasterPosition.y = -((float)Math::RandomRange(64, MapWidth));
				disasterData[i].Active = true;//-64�� ū������ ���� �����ǵ� ��©������
				break;
			case 1:
				disasterData[i].disasterPosition.x = -((float)Math::RandomRange(64, MapWidth));
				disasterData[i].disasterPosition.y = ((float)Math::RandomRange(0, MapHeight));
				disasterData[i].Active = true;//���������� ���ư��� ��������
				break;
			case 2:
				disasterData[i].disasterPosition.x = (float)Math::RandomRange(MapWidth, MapWidth*2);
				disasterData[i].disasterPosition.y = (float)Math::RandomRange(0, MapHeight-64);
				disasterData[i].Active = true;//�������� ���ư� ��������
				break;
			case 3:
				disasterData[i].disasterPosition.x = (float)Math::RandomRange(0, MapWidth);
				disasterData[i].disasterPosition.y = (float)Math::RandomRange(MapHeight, MapHeight*2);
				disasterData[i].Active = true;//���� �ö� ������ ����Ʈ�� �ݴ�.
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
	}//for(i) disasterData�迭�� �ʱ�ȭ.
}


//������ ���� ���� �ִϸ��̼��� �����ȴ�.

//�ִϸ��̼� Ŭ�� ���� �ּ�
//for (int i = 0; i < 4; i++)//Idle
//{
//	clips.clear();
//	clips.push_back(AnimationClip(disasterImg[0 + i * 5], 200));
//	actions32size.push_back(new Animator(slimePosition, &clips, AniRepeatType_End));
//}//for(i)

//for (int i = 0; i < 1; i++)//move
//{
//	clips.clear();
//	clips.push_back(AnimationClip(disasterImg[0 + i * 5], 75));//75�ð�
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
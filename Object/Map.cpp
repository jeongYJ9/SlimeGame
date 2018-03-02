#include "../stdafx.h"
#include "Map.h"

Map::Map(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./Render/Shader/MapShader.hlsl")
	

{

	wstring fileNames[] =
	{
		L"./Textures/buildSrc.png"
		//,L"./Textures/slime.png"
		//이런식으로 늘린다.
	};
	for (float j = 0; j < 8; j++)
	{
		for (float i = 0; i < 4; i++)
		{
			blocks.push_back(Texture::Add(fileNames[0],
			{ 0 + (i * 32), 0 + (j * 32) }, { 32 + (i * 32) ,32 + (j * 32) }));
		}//for(i)
	}//for(j)//벽과 저장블럭이미지 가져온부분// 액체는 31까지
	for (float j = 0; j < 3; j++)
	{
		for (float i = 4; i < 7; i++)//위치확인
		{
			blocks.push_back(Texture::Add(fileNames[0],
			{ 0 + (i * 32), 0 + (j * 32) }, { 32 + (i * 32) ,32 + (j * 32) }));
		}//for(i)
	}//for(j)//빌딩과 이펙트// 빌딩은 32~35, 이펙트는 36~43


	 //blocks의 44,45,46번째가 액체(CMY) 이미지다.
	blocks.push_back(Texture::Add(fileNames[0], { 129,96 }, { 192,160 }));//41
	blocks.push_back(Texture::Add(fileNames[0], { 192,96 }, { 256,160 }));//42
	blocks.push_back(Texture::Add(fileNames[0], { 129,160 }, { 192,224 }));//43
																		   //액체 생성 CMY

	mapSizeX = 32;
	mapSizeY = 24;

	R_WallMaxHp = 20;
	G_WallMaxHp = 20;
	B_WallMaxHp = 20;
	C_WallMaxHp = 10;
	M_WallMaxHp = 5;
	Y_WallMaxHp = 3;
	K_WallMaxHp = 50;

	TempPrevTile = 0;
	TempPosXY = -1;

	for (int i = 0; i<mapSizeX * mapSizeY; i++)
	{
		mapData[i].blocked = 0;//전부 통행가능
		mapData[i].tileType = 0;//구조체 주석참고
		mapData[i].tileColor = 0;//D=0,R1G2B3MYCK
		mapData[i].hp = 0;//체력에 따른 이미지 변환필요 0 초기화
		mapData[i].maxHp = 0;
		mapData[i].LineWidth = 0;
		mapData[i].LineHeight = 0;
		mapData[i].Room = 0;
		mapData[i].RoomState = 0;
	}//맵 사이즈를 결정하는게 i의 반복문, 정적할당이라 헤더파일에서 사이즈 정해줬음.
}

Map::~Map()
{
}

void Map::Initialize()
{

}

void Map::CreateBuild(int state, D3DXVECTOR2 Position, int color)
{
	int PosXY = (int)((Position.x + 16) / 32) + ((int)((Position.y + 16) / 32)) * mapSizeX;

	int tempHp = 0;
	switch (color)
	{
		case 1: tempHp = R_WallMaxHp;
			break;
		case 2: tempHp = G_WallMaxHp;
			break;
		case 3: tempHp = B_WallMaxHp;
			break;
		case 4: tempHp = C_WallMaxHp;
			break;
		case 5: tempHp = M_WallMaxHp;
			break;
		case 6: tempHp = Y_WallMaxHp;
			break;
		case 7: tempHp = K_WallMaxHp;
			break;
	}

	switch (state % 4)
	{
		case 0://_Front
			if ((int)Position.y % 32 < 16)
					PosXY += mapSizeX;
			if (color == 6)	{mapData[PosXY + mapSizeX].blocked = 0;}//노란벽은 통과 가능하게 하려고 추가한 if문
			else
			mapData[PosXY + mapSizeX].blocked = 1;
			mapData[PosXY + mapSizeX].tileType = 1;
			mapData[PosXY + mapSizeX].tileColor = color;
			mapData[PosXY + mapSizeX].hp = tempHp;//지워야됨
			mapData[PosXY + mapSizeX].maxHp = tempHp;//지워야됨
			break;

		case 1://_Right,
			if ((int)Position.x % 32 < 16)
				PosXY += 1;
			if (color == 6)	{mapData[PosXY + 1].blocked = 0;}
			else
			mapData[PosXY + 1].blocked = 1;
			mapData[PosXY + 1].tileType = 1;
			mapData[PosXY + 1].tileColor = color;
			mapData[PosXY + 1].hp = tempHp;//지워야됨
			mapData[PosXY + 1].maxHp = tempHp;//지워야됨
			break;

		case 2://_Left,
			if ((int)Position.x % 32 > 15)
				PosXY -= 1;
			if (color == 6)	{mapData[PosXY - 1].blocked = 0;}
			else
			mapData[PosXY - 1].blocked = 1;
			mapData[PosXY - 1].tileType = 1;
			mapData[PosXY - 1].tileColor = color;
			mapData[PosXY - 1].hp = tempHp;//지워야됨
			mapData[PosXY - 1].maxHp = tempHp;//지워야됨
			break;

		case 3://_Back,
			if ((int)Position.y % 32 > 15)
				PosXY -= mapSizeX;
			if (color == 6)	{mapData[PosXY - mapSizeX].blocked = 0;}
			else
			mapData[PosXY - mapSizeX].blocked = 1;
			mapData[PosXY - mapSizeX].tileType = 1;
			mapData[PosXY - mapSizeX].tileColor = color;
			mapData[PosXY - mapSizeX].hp = tempHp;//지워야됨
			mapData[PosXY - mapSizeX].maxHp = tempHp;//지워야됨
			break;
		default:
			break;
	}
	//TempPosXY = PosXY;



	checkRoom();
}

void Map::CreateBuildPrev(int state, D3DXVECTOR2 Position, int color)
{
	int PosXY = (int)((Position.x + 16) / 32) + ((int)((Position.y + 16) / 32)) * mapSizeX;

	switch (state % 4)
	{
		case 0://_Front
			if ((int)Position.y % 32 < 16)
				PosXY += mapSizeX;
			TempPosXY = PosXY + mapSizeX;
			break;

		case 1://_Right,
			if ((int)Position.x % 32 < 16)
				PosXY += 1;
			TempPosXY = PosXY + 1;
			break;

		case 2://_Left,
			if ((int)Position.x % 32 > 15)
				PosXY -= 1;
			TempPosXY = PosXY - 1;
			break;

		case 3://_Back,
			if ((int)Position.y % 32 > 15)
				PosXY -= mapSizeX;
			TempPosXY = PosXY - mapSizeX;
			break;
	}
	TempPrevTile = 0;
}


void Map::Createblock(int state, D3DXVECTOR2 Position, int color)
{
	int PosXY = (int)((Position.x + 16) / 32) + ((int)((Position.y + 16) / 32)) * mapSizeX;


	switch (state % 4)
	{
		case 0://_Front
			if ((int)Position.y % 32 < 16)
				PosXY += mapSizeX;
			mapData[PosXY + mapSizeX].blocked = 1;
			mapData[PosXY + mapSizeX].tileType = 2;
			mapData[PosXY + mapSizeX].tileColor = color;
			break;

		case 1://_Right,
			if ((int)Position.x % 32 < 16)
				PosXY += 1;
			mapData[PosXY + 1].blocked = 1;
			mapData[PosXY + 1].tileType = 2;
			mapData[PosXY + 1].tileColor = color;
			break;

		case 2://_Left,
			if ((int)Position.x % 32 > 16)
				PosXY -= 1;
			mapData[PosXY - 1].blocked = 1;
			mapData[PosXY - 1].tileType = 2;
			mapData[PosXY - 1].tileColor = color;
			break;

		case 3://_Back,
			if ((int)Position.y % 32 > 16)
				PosXY -= mapSizeX;
			mapData[PosXY - mapSizeX].blocked = 1;
			mapData[PosXY - mapSizeX].tileType = 2;
			mapData[PosXY - mapSizeX].tileColor = color;
			break;


		default:
			break;
	}

	checkRoom();
}

void Map::CreateWater()//(int state, D3DXVECTOR2 Position)
{
	int escape=0;
	while (true)
	{
	
		int tempXY = Math::RandomRange(0, mapSizeX * mapSizeY);
		int tempColor = Math::RandomRange(4, 6);
		if ((mapData[tempXY].tileType == 0)&& (mapData[tempXY + 1].tileType == 0) && (mapData[tempXY + mapSizeX].tileType == 0) && (mapData[tempXY + 1 + mapSizeX].tileType == 0))
		{
			mapData[tempXY].tileType = 5;//5번이 액체생성
			mapData[tempXY].tileColor = tempColor;

			//실질적으로 채집이 가능한곳은 4범위로 두고 그래픽표현할것은 좌상단의 타일으로
			mapData[tempXY].liquid = 1;//1이 채집가능, 0이 불가능
			mapData[tempXY+1].liquid = 1;//1이 채집가능, 0이 불가능
			mapData[tempXY+ mapSizeX].liquid = 1;//1이 채집가능, 0이 불가능
			mapData[tempXY+1+mapSizeX].liquid = 1;//1이 채집가능, 0이 불가능

			mapData[tempXY].tileColor = tempColor;
			mapData[tempXY + 1].tileColor = tempColor;
			mapData[tempXY + mapSizeX].tileColor = tempColor;
			mapData[tempXY + 1 + mapSizeX].tileColor = tempColor;

			break;
		}
		else if (mapData[tempXY].tileType != 0)
		{

		}
		escape++;

		if (escape > 20)//맵에 생성할곳이 없어서 무한루프가 생기는것을 방지하기위해 넣음. 20회 돌면 액체가 생성될 공간 찾는것 탈출
		{
			break;
		}
	}
}

void Map::DeleteWater(int XY)//모래바람이나 알파카의 이유로 액체가 사라질때 실행됨.
{
	mapData[XY].tileType = 0;

	mapData[XY].liquid = 0;//1이 채집가능, 0이 불가능
	mapData[XY + 1].liquid = 0;//1이 채집가능, 0이 불가능
	mapData[XY + mapSizeX].liquid = 0;//1이 채집가능, 0이 불가능
	mapData[XY + 1 + mapSizeX].liquid = 0;//1이 채집가능, 0이 불가능

	mapData[XY].tileColor = 0;
	mapData[XY + 1].tileColor = 0;
	mapData[XY + mapSizeX].tileColor = 0;
	mapData[XY + 1 + mapSizeX].tileColor = 0;
}


void Map::checkRoom()//블럭을 생성하거나 블럭의 체력이0이되어 소멸될때 호출된다.
{
	
	int LinesWidth = 0;
	int LinesHeight = 0;



	//가로 세로로 줄지어 확인하여 외부와 연결된 공간을 구분함.
	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{
		if (mapData[i].tileType == 0)//x축으로 순차적으로 탐색하다가 첫블럭발견
		{
			mapData[i].LineWidth = LinesWidth;//현재의 라인을 넣어준다.
		}
		else if (mapData[i].tileType == 1)//벽을 만나면
		{
			if ((mapData[i - 1].tileType == 1) && (i > 1));//이전타일이 벽이면 통과
			else//이번 타일이 벽이면서 이전타일이 벽이 아니면 Lines 증가시켜줌.
				LinesWidth++;
		}
		//현재 상태로는 제일 우측에서 한줄아래 좌측으로 넘어가는부분의 계산이 미묘함.
	}//for(i) 모든 가로열에 대해 Lines 값 입력완료

	for (int i = 0; i < mapSizeX*mapSizeY; i+= (mapSizeX-1))
	{
		LinesWidths.push_back(mapData[i].LineWidth);
		i++;
		LinesWidths.push_back(mapData[i].LineWidth);
	}//가로로 이은 라인에 대한 값을 따로 저장해둔다. 

	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if (mapData[i + (j*mapSizeX)].tileType == 0)
			mapData[i + (j*mapSizeX)].LineHeight = LinesHeight;
			else if (mapData[i + (j*mapSizeX)].tileType == 1)//벽을 만나면
			{
				if ((mapData[i + ((j-1)*mapSizeX)].tileType == 1) && (j > 1));//이전타일이 벽이면 통과
				else//이번 타일이 벽이면서 이전타일이 벽이 아니면 Lines 증가시켜줌.
					LinesHeight++;
			}
		}
	}//for(i) 모든 세로행에 대해 Lines 값 입력완료
	
	for (int i = 0; i < mapSizeX; i++)
	{
		LinesHeights.push_back(mapData[i].LineHeight);
		LinesHeights.push_back(mapData[i+(mapSizeX*(mapSizeY-1))].LineHeight);
		
	}//세로로 이은 라인에 대한 값을 따로 저장해둔다. 


	//가로열과 세로행을 외부와 연결됬는지 비교하여 연결되면 0으로 바꿈.
	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{
		for (int j = 0; j < LinesWidths.size(); j++)
		{
			if (mapData[i].LineWidth == LinesWidths[j])
				mapData[i].LineWidth = 0;
		}
		for (int j = 0; j < LinesHeights.size(); j++)
		{
			if (mapData[i].LineHeight == LinesHeights[j])
				mapData[i].LineHeight = 0;
		}
	}//for(i)매실행시 이구간만 10만번의 for문이 반복된다.

	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{
		if ((mapData[i].LineWidth != 0) && (mapData[i].LineHeight != 0))
			mapData[i].tileType = 2;
		//실내공간은 비주얼 확인을 위해 tileType 2를 대입했다.
	}

	LinesWidths.clear();
	LinesHeights.clear();

	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{//모든 타일 상대로 문을 탐색한다. 벽돌블럭 + 컬러 5인 상황에 대한 실행
		if ((mapData[i].tileType == 1) && (mapData[i].tileColor == 5))
		{//해당 좌표가 문이 맞다면 재귀함수를 실행한다.
			//int a = FindRoom(i);
		}
	}

}

//int Map::FindRoom(int num)
//{
//
//	//아래 코드는 예외처리지만 나중에 지울것임.
//	if (((num % mapSizeX - 1) == 0) || (num<mapSizeX) || (num>(mapSizeX*mapSizeY - 1)))
//		num++;
//	//나중에 모서리부분 벽생성금지 추가하면 지울 코드
//
//	for (int i = 0; i < FindRoomNum.size(); i++)
//	{
//		if (num == FindRoomNum[i])
//			return;
//			//만약 이전에 탐색한공간이랑 지금 탐색하려는 공간이 같으면?
//			//탐색 방향을 전환해야한다.
//	}
//
//	
//
//	//내 룸 상태가 0인지 1인지는 내 왼쪽을 통해 확인한다.
//	if (mapData[num - 1].tileType == 2)//왼쪽이 룸영역이야!
//	mapData[num].RoomState = FindRoom(num - 1);
//	else if (mapData[num - mapSizeX].tileType == 2)//왼쪽이 룸영역은 아니지만 위는 룸영역이야
//		mapData[num].RoomState = FindRoom(num - mapSizeX);
//
//	
//}

int Map::GetTileState(int state, D3DXVECTOR2 Position)
{
	int PosXY = (int)((Position.x) / 32) + ((int)((Position.y) / 32)) * mapSizeX;
	switch (state % 4)
	{
		case 0:
			PosXY += mapSizeX;//Front
			break;
		case 1:
			PosXY += 1;//Right
			break;
		case 2:
			PosXY -= 1;//Left
			break;
		case 3:
			//PosXY -= 32;//back
			break;
		default:
			break;
	}
	return mapData[PosXY].blocked;
}

int Map::GetTileType(int state, D3DXVECTOR2 Position)
{
	int PosXY = (int)((Position.x) / 32) + ((int)((Position.y) / 32)) * mapSizeX;
	return mapData[PosXY].tileType;

}

void Map::GetLiquidColor(int state, D3DXVECTOR2 Position,int* liquid, int* color)
{
	int PosXY = (int)((Position.x) / 32) + ((int)((Position.y) / 32)) * mapSizeX;
	//*liquid = mapData[PosXY].liquid;
	*color = mapData[PosXY].tileColor;
}
void Map::GetLiquid(int state, D3DXVECTOR2 Position, int* liquid, int* color)
{
	int PosXY = (int)((Position.x) / 32) + ((int)((Position.y) / 32)) * mapSizeX;
	*liquid = mapData[PosXY].liquid;
	//*color = mapData[PosXY].tileColor;
}


void Map::Update()
{
}

void Map::Render()
{
	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);
	shader->Begin(NULL, NULL);
	{
		for (int i = 0; i < mapSizeX * mapSizeY; i++)
		{
			float x = (float)(i % mapSizeX);
			float y = (float)(i / mapSizeX);
			int BlockNumber = 0;
			if (mapData[i].tileType == 0)
			{
				continue;
			}
			else if (mapData[i].tileType == 1)//벽 블럭
			{
				BlockNumber = (mapData[i].tileColor * 4);

				if (((float)(mapData[i].hp) / (float)(mapData[i].maxHp)) > 0.9f)
					BlockNumber += 0;
				else if (((float)(mapData[i].hp) / (float)(mapData[i].maxHp)) > 0.6f)
					BlockNumber += 1;
				else// if ((float)(mapData[i].hp) / (float)(mapData[i].maxHp) > 0.3f)
					BlockNumber += 2;
			}
			else if (mapData[i].tileType == 2)//저장블럭
			{
				BlockNumber = (mapData[i].tileColor * 4);
				BlockNumber += 3;

			}
			else if (mapData[i].tileType == 5)//액체 블럭
			{
				BlockNumber = (mapData[i].tileColor);//4,5,6, MYC
				BlockNumber += 37;//44,45,46이라서 40을 더해줌.

			}
		
			D3DXMatrixTranslation(&world, x * 32, y * 32, 0);
			Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
			shader->SetTexture("_map", blocks[BlockNumber]->GetTexture());
			shader->BeginPass(0);
			blocks[BlockNumber]->Render();
			shader->EndPass();


		}

	}
	shader->End();
}

void Map::RenderPrev()
{

	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);

	shader->Begin(NULL, NULL);
	{	
		float x = (float)(TempPosXY % mapSizeX);
		float y = (float)(TempPosXY / mapSizeX);

		D3DXMatrixTranslation(&world, x * 32, y * 32, 0);
		Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		shader->SetTexture("_map", blocks[TempPrevTile]->GetTexture());
		shader->BeginPass(1);
		blocks[TempPrevTile]->Render();
		shader->EndPass();
	}
	shader->End();
}

void Map::ResetData(int index)
{
	
		mapData[index].blocked = 0;//전부 통행가능
		mapData[index].tileType = 0;//구조체 주석참고
		mapData[index].tileColor = 0;//D=0,R1G2B3MYCK
		mapData[index].hp = 0;//체력에 따른 이미지 변환필요 0 초기화
		mapData[index].maxHp = 0;
		mapData[index].LineWidth = 0;
		mapData[index].LineHeight = 0;
		mapData[index].Room = 0;
		mapData[index].RoomState = 0;
	
}


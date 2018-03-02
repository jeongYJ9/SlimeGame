#include "../stdafx.h"
#include "Map.h"

Map::Map(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./Render/Shader/MapShader.hlsl")
	

{

	wstring fileNames[] =
	{
		L"./Textures/buildSrc.png"
		//,L"./Textures/slime.png"
		//�̷������� �ø���.
	};
	for (float j = 0; j < 8; j++)
	{
		for (float i = 0; i < 4; i++)
		{
			blocks.push_back(Texture::Add(fileNames[0],
			{ 0 + (i * 32), 0 + (j * 32) }, { 32 + (i * 32) ,32 + (j * 32) }));
		}//for(i)
	}//for(j)//���� ������̹��� �����ºκ�// ��ü�� 31����
	for (float j = 0; j < 3; j++)
	{
		for (float i = 4; i < 7; i++)//��ġȮ��
		{
			blocks.push_back(Texture::Add(fileNames[0],
			{ 0 + (i * 32), 0 + (j * 32) }, { 32 + (i * 32) ,32 + (j * 32) }));
		}//for(i)
	}//for(j)//������ ����Ʈ// ������ 32~35, ����Ʈ�� 36~43


	 //blocks�� 44,45,46��°�� ��ü(CMY) �̹�����.
	blocks.push_back(Texture::Add(fileNames[0], { 129,96 }, { 192,160 }));//41
	blocks.push_back(Texture::Add(fileNames[0], { 192,96 }, { 256,160 }));//42
	blocks.push_back(Texture::Add(fileNames[0], { 129,160 }, { 192,224 }));//43
																		   //��ü ���� CMY

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
		mapData[i].blocked = 0;//���� ���డ��
		mapData[i].tileType = 0;//����ü �ּ�����
		mapData[i].tileColor = 0;//D=0,R1G2B3MYCK
		mapData[i].hp = 0;//ü�¿� ���� �̹��� ��ȯ�ʿ� 0 �ʱ�ȭ
		mapData[i].maxHp = 0;
		mapData[i].LineWidth = 0;
		mapData[i].LineHeight = 0;
		mapData[i].Room = 0;
		mapData[i].RoomState = 0;
	}//�� ����� �����ϴ°� i�� �ݺ���, �����Ҵ��̶� ������Ͽ��� ������ ��������.
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
			if (color == 6)	{mapData[PosXY + mapSizeX].blocked = 0;}//������� ��� �����ϰ� �Ϸ��� �߰��� if��
			else
			mapData[PosXY + mapSizeX].blocked = 1;
			mapData[PosXY + mapSizeX].tileType = 1;
			mapData[PosXY + mapSizeX].tileColor = color;
			mapData[PosXY + mapSizeX].hp = tempHp;//�����ߵ�
			mapData[PosXY + mapSizeX].maxHp = tempHp;//�����ߵ�
			break;

		case 1://_Right,
			if ((int)Position.x % 32 < 16)
				PosXY += 1;
			if (color == 6)	{mapData[PosXY + 1].blocked = 0;}
			else
			mapData[PosXY + 1].blocked = 1;
			mapData[PosXY + 1].tileType = 1;
			mapData[PosXY + 1].tileColor = color;
			mapData[PosXY + 1].hp = tempHp;//�����ߵ�
			mapData[PosXY + 1].maxHp = tempHp;//�����ߵ�
			break;

		case 2://_Left,
			if ((int)Position.x % 32 > 15)
				PosXY -= 1;
			if (color == 6)	{mapData[PosXY - 1].blocked = 0;}
			else
			mapData[PosXY - 1].blocked = 1;
			mapData[PosXY - 1].tileType = 1;
			mapData[PosXY - 1].tileColor = color;
			mapData[PosXY - 1].hp = tempHp;//�����ߵ�
			mapData[PosXY - 1].maxHp = tempHp;//�����ߵ�
			break;

		case 3://_Back,
			if ((int)Position.y % 32 > 15)
				PosXY -= mapSizeX;
			if (color == 6)	{mapData[PosXY - mapSizeX].blocked = 0;}
			else
			mapData[PosXY - mapSizeX].blocked = 1;
			mapData[PosXY - mapSizeX].tileType = 1;
			mapData[PosXY - mapSizeX].tileColor = color;
			mapData[PosXY - mapSizeX].hp = tempHp;//�����ߵ�
			mapData[PosXY - mapSizeX].maxHp = tempHp;//�����ߵ�
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
			mapData[tempXY].tileType = 5;//5���� ��ü����
			mapData[tempXY].tileColor = tempColor;

			//���������� ä���� �����Ѱ��� 4������ �ΰ� �׷���ǥ���Ұ��� �»���� Ÿ������
			mapData[tempXY].liquid = 1;//1�� ä������, 0�� �Ұ���
			mapData[tempXY+1].liquid = 1;//1�� ä������, 0�� �Ұ���
			mapData[tempXY+ mapSizeX].liquid = 1;//1�� ä������, 0�� �Ұ���
			mapData[tempXY+1+mapSizeX].liquid = 1;//1�� ä������, 0�� �Ұ���

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

		if (escape > 20)//�ʿ� �����Ұ��� ��� ���ѷ����� ����°��� �����ϱ����� ����. 20ȸ ���� ��ü�� ������ ���� ã�°� Ż��
		{
			break;
		}
	}
}

void Map::DeleteWater(int XY)//�𷡹ٶ��̳� ����ī�� ������ ��ü�� ������� �����.
{
	mapData[XY].tileType = 0;

	mapData[XY].liquid = 0;//1�� ä������, 0�� �Ұ���
	mapData[XY + 1].liquid = 0;//1�� ä������, 0�� �Ұ���
	mapData[XY + mapSizeX].liquid = 0;//1�� ä������, 0�� �Ұ���
	mapData[XY + 1 + mapSizeX].liquid = 0;//1�� ä������, 0�� �Ұ���

	mapData[XY].tileColor = 0;
	mapData[XY + 1].tileColor = 0;
	mapData[XY + mapSizeX].tileColor = 0;
	mapData[XY + 1 + mapSizeX].tileColor = 0;
}


void Map::checkRoom()//���� �����ϰų� ���� ü����0�̵Ǿ� �Ҹ�ɶ� ȣ��ȴ�.
{
	
	int LinesWidth = 0;
	int LinesHeight = 0;



	//���� ���η� ������ Ȯ���Ͽ� �ܺο� ����� ������ ������.
	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{
		if (mapData[i].tileType == 0)//x������ ���������� Ž���ϴٰ� ù���߰�
		{
			mapData[i].LineWidth = LinesWidth;//������ ������ �־��ش�.
		}
		else if (mapData[i].tileType == 1)//���� ������
		{
			if ((mapData[i - 1].tileType == 1) && (i > 1));//����Ÿ���� ���̸� ���
			else//�̹� Ÿ���� ���̸鼭 ����Ÿ���� ���� �ƴϸ� Lines ����������.
				LinesWidth++;
		}
		//���� ���·δ� ���� �������� ���پƷ� �������� �Ѿ�ºκ��� ����� �̹���.
	}//for(i) ��� ���ο��� ���� Lines �� �Է¿Ϸ�

	for (int i = 0; i < mapSizeX*mapSizeY; i+= (mapSizeX-1))
	{
		LinesWidths.push_back(mapData[i].LineWidth);
		i++;
		LinesWidths.push_back(mapData[i].LineWidth);
	}//���η� ���� ���ο� ���� ���� ���� �����صд�. 

	for (int i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			if (mapData[i + (j*mapSizeX)].tileType == 0)
			mapData[i + (j*mapSizeX)].LineHeight = LinesHeight;
			else if (mapData[i + (j*mapSizeX)].tileType == 1)//���� ������
			{
				if ((mapData[i + ((j-1)*mapSizeX)].tileType == 1) && (j > 1));//����Ÿ���� ���̸� ���
				else//�̹� Ÿ���� ���̸鼭 ����Ÿ���� ���� �ƴϸ� Lines ����������.
					LinesHeight++;
			}
		}
	}//for(i) ��� �����࿡ ���� Lines �� �Է¿Ϸ�
	
	for (int i = 0; i < mapSizeX; i++)
	{
		LinesHeights.push_back(mapData[i].LineHeight);
		LinesHeights.push_back(mapData[i+(mapSizeX*(mapSizeY-1))].LineHeight);
		
	}//���η� ���� ���ο� ���� ���� ���� �����صд�. 


	//���ο��� �������� �ܺο� �������� ���Ͽ� ����Ǹ� 0���� �ٲ�.
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
	}//for(i)�Ž���� �̱����� 10������ for���� �ݺ��ȴ�.

	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{
		if ((mapData[i].LineWidth != 0) && (mapData[i].LineHeight != 0))
			mapData[i].tileType = 2;
		//�ǳ������� ���־� Ȯ���� ���� tileType 2�� �����ߴ�.
	}

	LinesWidths.clear();
	LinesHeights.clear();

	for (int i = 0; i < mapSizeX*mapSizeY; i++)
	{//��� Ÿ�� ���� ���� Ž���Ѵ�. ������ + �÷� 5�� ��Ȳ�� ���� ����
		if ((mapData[i].tileType == 1) && (mapData[i].tileColor == 5))
		{//�ش� ��ǥ�� ���� �´ٸ� ����Լ��� �����Ѵ�.
			//int a = FindRoom(i);
		}
	}

}

//int Map::FindRoom(int num)
//{
//
//	//�Ʒ� �ڵ�� ����ó������ ���߿� �������.
//	if (((num % mapSizeX - 1) == 0) || (num<mapSizeX) || (num>(mapSizeX*mapSizeY - 1)))
//		num++;
//	//���߿� �𼭸��κ� ���������� �߰��ϸ� ���� �ڵ�
//
//	for (int i = 0; i < FindRoomNum.size(); i++)
//	{
//		if (num == FindRoomNum[i])
//			return;
//			//���� ������ Ž���Ѱ����̶� ���� Ž���Ϸ��� ������ ������?
//			//Ž�� ������ ��ȯ�ؾ��Ѵ�.
//	}
//
//	
//
//	//�� �� ���°� 0���� 1������ �� ������ ���� Ȯ���Ѵ�.
//	if (mapData[num - 1].tileType == 2)//������ �뿵���̾�!
//	mapData[num].RoomState = FindRoom(num - 1);
//	else if (mapData[num - mapSizeX].tileType == 2)//������ �뿵���� �ƴ����� ���� �뿵���̾�
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
			else if (mapData[i].tileType == 1)//�� ��
			{
				BlockNumber = (mapData[i].tileColor * 4);

				if (((float)(mapData[i].hp) / (float)(mapData[i].maxHp)) > 0.9f)
					BlockNumber += 0;
				else if (((float)(mapData[i].hp) / (float)(mapData[i].maxHp)) > 0.6f)
					BlockNumber += 1;
				else// if ((float)(mapData[i].hp) / (float)(mapData[i].maxHp) > 0.3f)
					BlockNumber += 2;
			}
			else if (mapData[i].tileType == 2)//�����
			{
				BlockNumber = (mapData[i].tileColor * 4);
				BlockNumber += 3;

			}
			else if (mapData[i].tileType == 5)//��ü ��
			{
				BlockNumber = (mapData[i].tileColor);//4,5,6, MYC
				BlockNumber += 37;//44,45,46�̶� 40�� ������.

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
	
		mapData[index].blocked = 0;//���� ���డ��
		mapData[index].tileType = 0;//����ü �ּ�����
		mapData[index].tileColor = 0;//D=0,R1G2B3MYCK
		mapData[index].hp = 0;//ü�¿� ���� �̹��� ��ȯ�ʿ� 0 �ʱ�ȭ
		mapData[index].maxHp = 0;
		mapData[index].LineWidth = 0;
		mapData[index].LineHeight = 0;
		mapData[index].Room = 0;
		mapData[index].RoomState = 0;
	
}


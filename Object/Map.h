#pragma once
#include "../Render/Shader/Shader.h"

struct MapData
{
	int blocked;//�̵����ɿ��� 0���డ�� 1����
	int tileType;//Ÿ�� Ư��: ��ĭ=0, ��, �����, ȸ���ǹ�, �Ĵ�, ��ü, 
	int liquid;
	int tileColor;//0 R G B C M Y K;
	int hp;
	int maxHp;
	/////////////�Ʒ� �� ������ �ǳ����п� �ʿ��� ����
	int LineWidth;
	int LineHeight;

	int Room;//0�����, 1���� 2���� 3���� ...
	int RoomState;//0 �����, 1���,�Ķ���ǥ�� 2�����ִ¹�,���ǥ��, 3�ļյȹ� �����Ȥ�� ��������ǥ��
};
struct RoomData
{
	int RoomNum;
	int RoomState;
};
class Map : public Shader
{
public:
	Map(LPDIRECT3DDEVICE9 device);
	~Map();

	void Initialize();

	void CreateBuild(int state, D3DXVECTOR2 Position,int color);
	//������ǥ���� ���¹������� ��ĭ��
	void CreateBuildPrev(int state, D3DXVECTOR2 Position, int color);

	void Createblock(int state, D3DXVECTOR2 Position, int color);//���������

	void CreateWater();
	void DeleteWater(int XY);//����� ������ ������������
	//��ü �����Ҹ�

	void checkRoom();

	//int FindRoom(int num);

	int GetTileState(int state, D3DXVECTOR2 Position);//�浹üũ���� Ÿ�ϻ���Ȯ��

	int GetTileType(int state, D3DXVECTOR2 Position);
	void GetLiquidColor(int state, D3DXVECTOR2 Position,int* liquid, int* color);
	void GetLiquid(int state, D3DXVECTOR2 Position, int* liquid, int* color);
	//Ÿ���� ��ü���� Ȯ����, ��ü�� �����ӿ� ����

	MapData GetMapData(int index)
	{
		return mapData[index];
	}

	int GetTileType(int index)
	{
		return mapData[index].tileType;
	}
	int GetTileHp(int index)
	{
		return mapData[index].hp;
	}
	void SetTileHp(int index, int hp)
	{
		mapData[index].hp = hp;
	}

	void Update();
	void Render();
	void RenderPrev();

	void ResetData(int index);

	int GetMapSizeX()
	{
		return mapSizeX;
	}
	int GetMapSizeY()
	{
		return mapSizeY;
	}
private:

	vector<Animator *> effects;
	vector<Texture *> blocks;
	vector<Texture *> bulids;

	
	vector<int> LinesWidths;
	vector<int> LinesHeights;
	
	vector<int> FindRoomNum;

	int TempPrevTile;
	int TempPosXY;

	MapData mapData[32 * 24];
	int mapSizeX;//32
	int mapSizeY;//24
	//32by24�� ���¶� yŸ���� ��ġ�� Ȯ���ϱ����� 32��ŭ �����־� �ε�����ȣ�� ������ �Ǿ��ִ�. �̰� �� 

	int R_WallMaxHp;
	int G_WallMaxHp;
	int B_WallMaxHp;
	int C_WallMaxHp;
	int M_WallMaxHp;
	int Y_WallMaxHp;
	int K_WallMaxHp;

	Rect MapCheckRect;
};
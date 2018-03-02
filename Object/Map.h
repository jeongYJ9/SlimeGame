#pragma once
#include "../Render/Shader/Shader.h"

struct MapData
{
	int blocked;//이동가능여부 0통행가능 1막힘
	int tileType;//타일 특성: 빈칸=0, 벽, 저장블럭, 회복건물, 식당, 액체, 
	int liquid;
	int tileColor;//0 R G B C M Y K;
	int hp;
	int maxHp;
	/////////////아래 두 변수는 실내구분에 필요한 변수
	int LineWidth;
	int LineHeight;

	int Room;//0방없음, 1번방 2번방 3번방 ...
	int RoomState;//0 방없음, 1빈방,파란색표현 2주인있는방,녹색표현, 3파손된방 노란색혹은 적색으로표현
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
	//현재좌표에서 보는방향으로 한칸앞
	void CreateBuildPrev(int state, D3DXVECTOR2 Position, int color);

	void Createblock(int state, D3DXVECTOR2 Position, int color);//저장블럭생성

	void CreateWater();
	void DeleteWater(int XY);//어떤것을 지울지 포지션정해줌
	//액체 생성소멸

	void checkRoom();

	//int FindRoom(int num);

	int GetTileState(int state, D3DXVECTOR2 Position);//충돌체크위한 타일상태확인

	int GetTileType(int state, D3DXVECTOR2 Position);
	void GetLiquidColor(int state, D3DXVECTOR2 Position,int* liquid, int* color);
	void GetLiquid(int state, D3DXVECTOR2 Position, int* liquid, int* color);
	//타일이 액체인지 확인후, 액체를 슬라임에 저장

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
	//32by24인 상태라서 y타일의 위치를 확인하기위해 32만큼 곱해주어 인덱스번호를 얻어오게 되어있다. 이거 꽤 

	int R_WallMaxHp;
	int G_WallMaxHp;
	int B_WallMaxHp;
	int C_WallMaxHp;
	int M_WallMaxHp;
	int Y_WallMaxHp;
	int K_WallMaxHp;

	Rect MapCheckRect;
};
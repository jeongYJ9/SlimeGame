#pragma once

#include "../Render/Shader/Shader.h"

struct DisasterData
{
	bool Active;
	int state;//어떤 타입에 따라 어떤게 렌더링될지 결정됨.
	//혹은 이것을 통해서 부서진 돌의 렌더링도 같이할수있음 이펙트와 같이사용
	int Hp;//벽과 충돌시 
	D3DXVECTOR2 disasterPosition;//location
	D3DXVECTOR2 size;
	//void crash();
};

class Disaster : public Shader
{
public:
	Disaster(LPDIRECT3DDEVICE9 device);
	~Disaster();

	

//	void Initialize();//이미지, 애니메이션 생성

//	void CreateDisasterData();//난이도, 스테이지, 방향정보

	void Update();
	void Render();

	void Crash();


	Rect GetRect(int index)
	{
		rectCollision.location = disasterData[index].disasterPosition;
		rectCollision.size = disasterData[index].size;

		return rectCollision;
	}//해당 인덱스의 Rect를 가져온다.

	bool GetDisasterDataActive(int index)
	{
		return disasterData[index].Active;
	}

	int GetDisasterDataHp(int index)
	{//
			return disasterData[index].Hp;
	}//Hp를 얻어온다.
	void SetDisasterDataHp(int index, int Hp)
	{
			disasterData[index].Hp = Hp;
	}//벽보다 재해의 hp가 클시 벽hp-hp값을 set해준다. 

	void DestroyDisasterData(int index)
	{
		disasterData[index].Active = false;
	}//파괴된 디재스터는 비활성화 합니다.

	void Move();
	void CreateDisaster(int direction);

	void ResetData();

private:



	vector<Texture *> disasterImg;

	vector<Animator *> actions32size;
	vector<Animator *> actions64size;

	Rect rectCollision;

	int Direction;//0Front 1Right 2Left 3Back
	float disasterSpeed;

	DisasterData disasterData[50];
	int disasterDataSize;//50으로 초기화함
	
	int MapHeight;
	int MapWidth;
};


//갖고있어야 될 함수 :이동하게 될 4 방향에 대한 move함수 
//생성, 소멸, 필요시 메모리 풀링을? 동일한 내용에 이미지만 교체?

// 이펙트는 따로 클래스 만들어줄것.
#pragma once

#include "../Render/Shader/Shader.h"

struct DisasterData
{
	bool Active;
	int state;//� Ÿ�Կ� ���� ��� ���������� ������.
	//Ȥ�� �̰��� ���ؼ� �μ��� ���� �������� �����Ҽ����� ����Ʈ�� ���̻��
	int Hp;//���� �浹�� 
	D3DXVECTOR2 disasterPosition;//location
	D3DXVECTOR2 size;
	//void crash();
};

class Disaster : public Shader
{
public:
	Disaster(LPDIRECT3DDEVICE9 device);
	~Disaster();

	

//	void Initialize();//�̹���, �ִϸ��̼� ����

//	void CreateDisasterData();//���̵�, ��������, ��������

	void Update();
	void Render();

	void Crash();


	Rect GetRect(int index)
	{
		rectCollision.location = disasterData[index].disasterPosition;
		rectCollision.size = disasterData[index].size;

		return rectCollision;
	}//�ش� �ε����� Rect�� �����´�.

	bool GetDisasterDataActive(int index)
	{
		return disasterData[index].Active;
	}

	int GetDisasterDataHp(int index)
	{//
			return disasterData[index].Hp;
	}//Hp�� ���´�.
	void SetDisasterDataHp(int index, int Hp)
	{
			disasterData[index].Hp = Hp;
	}//������ ������ hp�� Ŭ�� ��hp-hp���� set���ش�. 

	void DestroyDisasterData(int index)
	{
		disasterData[index].Active = false;
	}//�ı��� ���罺�ʹ� ��Ȱ��ȭ �մϴ�.

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
	int disasterDataSize;//50���� �ʱ�ȭ��
	
	int MapHeight;
	int MapWidth;
};


//�����־�� �� �Լ� :�̵��ϰ� �� 4 ���⿡ ���� move�Լ� 
//����, �Ҹ�, �ʿ�� �޸� Ǯ����? ������ ���뿡 �̹����� ��ü?

// ����Ʈ�� ���� Ŭ���� ������ٰ�.
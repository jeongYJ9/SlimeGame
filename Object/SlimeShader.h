#pragma once
#include "../Render/Shader/Shader.h"



class SlimeShader : public Shader
{
public:
	SlimeShader(LPDIRECT3DDEVICE9 device);
	~SlimeShader();

	void Update();
	void Render();
	///////////////////////////
	//float SetPower(float power);

	///////매니져쓸려고만든 겟셋들
	int GetLiquidColor(){return LiquidColor;}
	void SetLiquidColor(int LiquidColor)
	{ this->LiquidColor= LiquidColor; }

	int GetLiquidState() { return LiquidState; }
	void SetLiquidState(int LiquidState)
	{ this->LiquidState = LiquidState; }

	int* Get_LiquidColor() {return &LiquidColor;}
	int* Get_LiquidState() {return &LiquidState;}

	D3DXVECTOR4 GetToneColor(){return toneColor;}
	void SetToneColor(D3DXVECTOR4 toneColor)
	{this->toneColor = toneColor;}

	int Getstate() {return state;}
	void Setstate(int state) { this->state = state; }

	D3DXVECTOR2 GetslimePosition() {return slimePosition;}

	float GetslimePositionX() {return slimePosition.x; }
	float GetslimePositionY() {return slimePosition.y; }
	void SetslimePosition(D3DXVECTOR2 slimePosition)
	{ this->slimePosition = slimePosition; }


	//enum SlimeState
	//{
	//	Idle_Front = 0,
	//	Idle_Right,//1
	//	Idle_Left,//2
	//	Idle_Back,//3
	//	Move_Front,
	//	Move_Right,
	//	Move_Left,
	//	Move_Back,
	//	Act_Front,
	//	Act_Right,
	//	Act_Left,
	//	Act_Back
	//} state;

	DWORD LiquidTime;
	DWORD BuildTime;
	DWORD SaveBlockTime;
	DWORD EatTime;
	DWORD RepairTime;
	
	//Bar에 사용될 Rect location
	Rect GetRectLocation()
	{
		return location;
	}
	Rect* pGetRectLocation()
	{
		return p_location;
	}

	Rect GetRectCollision()
	{
		return collision;
	}

	Rect* pGetUIRectLocation()
	{
		return p_UI_location;
	}
	void SetRectLocation_location(D3DXVECTOR2 location)
	{
		this->location.location = location;
	}

	float GetColorWeight()
	{
		return colorWeight;
	}

	void SetColorWeight(float colorWeight)
	{
		this->colorWeight = colorWeight;
	}

	int GetSlimeEnergy()
	{
		return  slimeEnergy;
	}

	void SetSlimeEnergy(int slimeEnergy)
	{
		this->slimeEnergy = slimeEnergy;
		if (this->slimeEnergy > slimeMaxEnergy)
			this->slimeEnergy = slimeMaxEnergy;
	}
	int R_energy;
	int G_energy;
	int B_energy;
	int C_energy;
	int M_energy;
	int Y_energy;
	int K_energy;
	int slimeMaxEnergy;

	void ResetData();
private:

	int LiquidColor;
	int LiquidState;//0이 기본, 1이 보유중인상태
	int* _LiquidColor;
	int* _LiquidState;
	D3DXVECTOR4 toneColor;
	float colorWeight;

	int state;
	bool Dead;

	vector<Texture *> marios;
	vector<Texture *> slimes;
	vector<Animator *> actions;
	D3DXVECTOR2 slimePosition;//location

	//////////////////////////추가한부분

	Rect location;
	Rect* p_location;

	Rect UI_location;
	Rect* p_UI_location;

	Rect collision;

	TextureDrawer* drawer;
	D3DVIEWPORT9 viewport;

	///////////////슬라임이 행동하기 위한 에너지
	int slimeEnergy;
	


};
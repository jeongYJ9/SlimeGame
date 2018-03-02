#include "../stdafx.h"
#include "SlimeShader.h"
#include "Map.h"
SlimeShader::SlimeShader(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./Render/Shader/SlimeShader.hlsl")
	, toneColor(1, 1, 1, 1)
	, colorWeight(1.0f)
	, slimePosition(512, 368)//512
	//, state(SlimeState::Idle_Right)
	, state(0), Dead(false)
	//�÷��̾��� ��ü ������λ���
	, LiquidColor(0), LiquidState(0)
	//�� �ൿ�� �ʿ��� �ð� ������
	, LiquidTime(1000), BuildTime(1000)
	, SaveBlockTime(1000) , EatTime(500)
	, RepairTime(1000)
	, R_energy(30)	, G_energy(30)
	, B_energy(30)	, C_energy(15)
	, M_energy(15)	, Y_energy(30)
	, K_energy(0)	
	, slimeEnergy(100) , slimeMaxEnergy(200)

{
	p_location = &location;
	p_UI_location = &UI_location;
	//rect �ּҰ����� ����� ����� bar�� ��� ������Ʈ�Ǳ�����.

	_LiquidColor = &LiquidColor;
	_LiquidState = &LiquidState;



	

//	map = new Map(device);//�� ����.

	wstring fileNames[] =
	{
		L"./Textures/buildSrc.jpg",
		L"./Textures/slime.png",
	};
	for (float j = 0; j < 4; j++) 
	{
		for (float i = 0; i < 5; i++)
		{
			slimes.push_back(Texture::Add(fileNames[1],
			{ 0 + (i * 32), 0 + (j * 32) }, { 31 + (i * 32) ,31 + (j * 32) }));
		}
	}
	vector<AnimationClip> clips;
	for (int i = 0; i < 4; i++)//Idle
	{
		clips.clear();
		clips.push_back(AnimationClip(slimes[0+i*5], 200));
		actions.push_back(new Animator(slimePosition, &clips, AniRepeatType_End));
	}//for(i)
	for (int i = 0; i < 4; i++)//move
	{
		clips.clear();
		clips.push_back(AnimationClip(slimes[0+i*5], 75));
		clips.push_back(AnimationClip(slimes[1+i*5], 75));
		clips.push_back(AnimationClip(slimes[2+i*5], 75));
		clips.push_back(AnimationClip(slimes[3+i*5], 100));
		clips.push_back(AnimationClip(slimes[4+i*5], 150));
		actions.push_back(new Animator(slimePosition, &clips, AniRepeatType_Loop));
	}//for(i)


	location.location = Point(400, 368);
	location.size = Size(32, 5);

	UI_location.location = Point(0, 0);
	UI_location.size = Size(200, 10);//ü�¹� ũ��

	collision.location = Point(512, 368);
	collision.size = Size(32, 32);

	////////////////////////////////////////
	Device::GetDevice()->GetViewport(&viewport);
	//Direct2D::Device()->GetViewport(&viewport);

}

SlimeShader::~SlimeShader()
{
	for (size_t i = 0; i < actions.size(); i++)
		SAFE_DELETE(actions[i]);

	

}

void SlimeShader::Update()
{
	collision.location = slimePosition;//�����ǿ��� ������ Rect

	actions[state]->Update();

}

void SlimeShader::Render()
{


	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);
	
	
	shader->Begin(NULL, NULL);
	{
		D3DXMatrixTranslation(&world, slimePosition.x, slimePosition.y, 0);
		Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);

		Texture* texture = actions[state]->GetCurrentClipTexture();
		
		shader->SetVector("_toneColor", (D3DXVECTOR4 *)&toneColor);
		shader->SetFloat("_colorWeight", colorWeight);
		shader->SetTexture("_map", texture->GetTexture());
		shader->BeginPass(0);
		actions[state]->Render();
		shader->EndPass();
	}
	shader->End();

	////////////////

	

}

void SlimeShader::ResetData()
{
	
	slimePosition = { 512,368 };

	toneColor = { 0,0,0,0};
	colorWeight = 0;

	state = 0;
	Dead = false;
	//�÷��̾��� ��ü ������λ���
	LiquidColor=0;
	LiquidState=0;
	//�� �ൿ�� �ʿ��� �ð� ������
	LiquidTime		=1000;
	BuildTime		=1000;
	SaveBlockTime	=1000;
	EatTime			=500;
	RepairTime		=1000;

	slimeEnergy		=100;
	slimeMaxEnergy	=200;

	location.location = Point(400, 368);
	location.size = Size(32, 5);

	UI_location.location = Point(30, 30);//ü�¹� ��ġ
	UI_location.size = Size(200, 200);

	collision.location = Point(512, 368);
	collision.size = Size(32, 32);
}

/////////////////////////////
//float SlimeShader::SetPower(float power)
//{
//	return 0.0f;
//}

//�Ǽ� ����, ���强�� = ������ �︮�¼Ҹ�// ����~
//�Ǽ�Ȥ�� �������� �����ȵ� = �Ƿη�? �Ƿ�? �����Ҹ�? or �й��� Ģ! �Ҹ�
//�̵� -> �ٽ����ٽ���
//��ü�� �̵� -> ÷��÷��
//�𷡹ٶ��Ҹ�

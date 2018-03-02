#pragma once



class Shader;//������ �ʿ��Ѱ�.
class SlimeShader;//ĳ����
class Map;//�� ����
class Disaster;//����, ��ֹ� ���
class Skill;//��ų��� �� ����Ʈ�̹���


class SceneManager
{
public:
	SceneManager(LPDIRECT3DDEVICE9 device);
	~SceneManager();

	void Initialize();
	void Destroy();
	void Update();
	void Render();


	void moveUpdate();//�̵�����
	void disasterAndSlime();//��Ӱ� �÷��̾�
	void disasterAndMap();//��Ӱ� �� ��ȣ�ۿ�
	bool GetGameOver();

	void SceneUpdate();//�� ���°���
	
	void EnergyUpdate(int Energy);
	void SceneResetData(int Character);

	void SceneMain();

	int GetScore()
	{
		return Score;
	}

private:

	int Score;


	SlimeShader* slime;
	Map* map;
	Disaster* disaster;
	Skill* skill;

	DWORD curTime;

	enum SlimeState//�̰� �� �����Ľ�����? 
	{
		Idle_Front = 0,
		Idle_Right,//1
		Idle_Left,//2
		Idle_Back,//3
		Move_Front,
		Move_Right,
		Move_Left,
		Move_Back,
		Act_Front,
		Act_Right,
		Act_Left,
		Act_Back
	} state;
	D3DXVECTOR2 slimePosition;//moveUpdate��

	ProgressBar* barEnergy;
	ProgressBar* bar;
	bool isPress;
	bool isUp;
	
	int SceneState;// 0 :��ü�� �������� �غ���� 10��, 1 : ��ü�� �����Ͽ� �Ǽ��ϰ� �÷��̾��� ������ Ű��� ���� 60�� // 2 : �����´� ���µ� �������� �÷��̾��� ü���� ������ ��� ��ü����, �Ǽ����� �Ҹ�ð��� 2��� �ø���. �����̺�Ʈ�� �ߵ��Ǳ⵵ ��. 3: �𷡹ٶ����� : �÷��̾�� ���ڿ� ���������� ������ �𷡹ٶ��� ���� ���� �ջ�ȴ�.

	DWORD SceneTime;
	bool SceneTimeCheck;

	bool GameOver;

	int SelectCharacter;

};
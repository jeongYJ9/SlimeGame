#pragma once



class Shader;//렌더에 필요한것.
class SlimeShader;//캐릭터
class Map;//맵 정보
class Disaster;//재해, 장애물 요소
class Skill;//스킬사용 및 이펙트이미지


class SceneManager
{
public:
	SceneManager(LPDIRECT3DDEVICE9 device);
	~SceneManager();

	void Initialize();
	void Destroy();
	void Update();
	void Render();


	void moveUpdate();//이동관련
	void disasterAndSlime();//재앙과 플레이어
	void disasterAndMap();//재앙과 맵 상호작용
	bool GetGameOver();

	void SceneUpdate();//맵 상태관련
	
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

	enum SlimeState//이게 왜 여깄냐싶은데? 
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
	D3DXVECTOR2 slimePosition;//moveUpdate용

	ProgressBar* barEnergy;
	ProgressBar* bar;
	bool isPress;
	bool isUp;
	
	int SceneState;// 0 :액체가 떨어지는 준비상태 10초, 1 : 액체를 수집하여 건설하고 플레이어의 생존을 키우는 상태 60초 // 2 : 경고상태는 오픈된 지형에서 플레이어의 체력을 서서히 깎고 액체수집, 건설등의 소모시간을 2배로 늘린다. 동료이벤트가 발동되기도 함. 3: 모래바람상태 : 플레이어는 벽뒤에 숨지않으면 죽으며 모래바람에 의해 벽이 손상된다.

	DWORD SceneTime;
	bool SceneTimeCheck;

	bool GameOver;

	int SelectCharacter;

};
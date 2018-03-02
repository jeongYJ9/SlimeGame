#pragma once
#include "../Render/Shader/Shader.h"


class Skill : public Shader
{
public:
	Skill(LPDIRECT3DDEVICE9 device);
	~Skill();


	void Update();
	void Render();

	void ActiveSkill();
	void SetStartGame(int SelectCharacter);
	bool GetActive()
	{
		return Active;
	}


private:

	int SelectCharacter;//어떤스킬을 쓸지 정하는부분
	bool Active;//스킬 사용상태여부
	bool effectPlay;//이펙트 재생
	DWORD duration;//스킬 지속시간
	DWORD CoolTime;//재사용시간
};


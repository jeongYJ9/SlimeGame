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

	int SelectCharacter;//���ų�� ���� ���ϴºκ�
	bool Active;//��ų �����¿���
	bool effectPlay;//����Ʈ ���
	DWORD duration;//��ų ���ӽð�
	DWORD CoolTime;//����ð�
};


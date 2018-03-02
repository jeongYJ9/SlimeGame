#include "../stdafx.h"
#include "Skill.h"



Skill::Skill(LPDIRECT3DDEVICE9 device)
: Shader(device, L"./Render/Shader/Default.hlsl"), SelectCharacter(0), Active(false)
, CoolTime(0) , effectPlay(0)
{
	wstring fileNames[] =
	{
		L"./Textures/buildSrc.jpg"
	};
	//����Ʈ ���õ� �̹����� ��� �̰����� ó���Ѵ�.


}

Skill::~Skill()
{

}

void Skill::Update()
{//����Ʈ�̹��� state�� ������Ʈ���ش�.

}

void Skill::Render()
{//����

}

void Skill::ActiveSkill()
{//ĳ���� Ÿ�Կ� ���� ���� ��ų�� �޶�����.
	switch (SelectCharacter)//�������� �ְ�, ����Ʈ Ű�� ������, ����ð��� �ƴ϶�� ����.
	{
		case 0:
			//����� �����ϸ� �ȵ�.
			break;
		case 1:
			Active = true;
			//�̰������� ����Ʈ�� ������ش�.
			break;
		case 2:

			break;
		case 3:

			break;
		default:
			break;
	}

}

void Skill::SetStartGame(int SelectCharacter)
{//������ ���۵ɶ� �ѹ��� �����.
	this->SelectCharacter = SelectCharacter;
}

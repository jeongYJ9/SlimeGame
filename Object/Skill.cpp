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
	//이펙트 관련된 이미지는 모두 이곳에서 처리한다.


}

Skill::~Skill()
{

}

void Skill::Update()
{//이펙트이미지 state를 업데이트해준다.

}

void Skill::Render()
{//렌더

}

void Skill::ActiveSkill()
{//캐릭터 타입에 따라 사용될 스킬이 달라진다.
	switch (SelectCharacter)//에너지가 있고, 시프트 키를 누르고, 재사용시간이 아니라면 사용됨.
	{
		case 0:
			//여기로 진입하면 안됨.
			break;
		case 1:
			Active = true;
			//이곳에서는 이펙트만 재생해준다.
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
{//게임이 시작될때 한번만 실행됨.
	this->SelectCharacter = SelectCharacter;
}

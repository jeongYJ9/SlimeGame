#pragma once

#include "../Render/Shader/Shader.h"

enum MainButtonActive
{
	None = 0,
	Start = 1,
	Option = 2,
	_Quit = 3
};

class MenuManager : public Shader
{
public:

	MenuManager(LPDIRECT3DDEVICE9 device);
	~MenuManager();

	void Update();
	void Render();

	void SelectButton();//어떤버튼 활성화할지

	bool GetStartGameState()
	{
		return StartGame;
	}
	void SetStartGameState(bool StartGame)
	{
		this->StartGame = StartGame;
	}

	int GetCharacterSelect()//게임매니져에서 사용
	{
		return menuState;
	}

	//int GetCharacter()
	//{
	//	return Character;
	//}

	//void SetCharacter()//캐릭터선택
	//{
	//	Character = menuState - 1;
	//}
private:

	
	
	int menuState;
	//0 메뉴 1 A캐릭 2 B캐릭 3 C캐릭

	//int Character;//menuState 에서 값 가져옴.
	//캐릭터 선택 1 A캐릭 2 B캐릭 3 C캐릭 

	MainButtonActive buttonActive;
	//메인화면에 어떤버튼활성화인지 여부

	//캐릭터선택창의 어떤캐릭활성화인지 여부


	bool Select;//활성화된 버튼 선택.
	bool StartGame;//
	vector<Texture *> MainMenuIMG;
	vector<Texture *> MainMenuButton;


	//메뉴 관련 배경이미지 모두 메인메뉴에 넣어둔다.
	//버튼이미지는 버튼에 모아둔다.
	//어떤 이미지 선택할지.


};
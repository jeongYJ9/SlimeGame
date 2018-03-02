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

	void SelectButton();//���ư Ȱ��ȭ����

	bool GetStartGameState()
	{
		return StartGame;
	}
	void SetStartGameState(bool StartGame)
	{
		this->StartGame = StartGame;
	}

	int GetCharacterSelect()//���ӸŴ������� ���
	{
		return menuState;
	}

	//int GetCharacter()
	//{
	//	return Character;
	//}

	//void SetCharacter()//ĳ���ͼ���
	//{
	//	Character = menuState - 1;
	//}
private:

	
	
	int menuState;
	//0 �޴� 1 Aĳ�� 2 Bĳ�� 3 Cĳ��

	//int Character;//menuState ���� �� ������.
	//ĳ���� ���� 1 Aĳ�� 2 Bĳ�� 3 Cĳ�� 

	MainButtonActive buttonActive;
	//����ȭ�鿡 ���ưȰ��ȭ���� ����

	//ĳ���ͼ���â�� �ĳ��Ȱ��ȭ���� ����


	bool Select;//Ȱ��ȭ�� ��ư ����.
	bool StartGame;//
	vector<Texture *> MainMenuIMG;
	vector<Texture *> MainMenuButton;


	//�޴� ���� ����̹��� ��� ���θ޴��� �־�д�.
	//��ư�̹����� ��ư�� ��Ƶд�.
	//� �̹��� ��������.


};
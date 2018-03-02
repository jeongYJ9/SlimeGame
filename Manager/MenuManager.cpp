#include "../stdafx.h"
#include"MenuManager.h"
#include"GameManager.h"

MenuManager::MenuManager(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./Render/Shader/default.hlsl") , menuState(0)//�޴����¿� �̹���
	, buttonActive (MainButtonActive::None)
	, Select(false) , StartGame(false)// , Character(1)
{
	//TODO: �̹��� �ҷ��ð�. ���ȭ�鿡 �´°� 
	//��ư�̹������� �Է¿� ���� ���ӸŴ����� state �ٲ��ٰ�.
	//�޴� ���¿����� �޴��� ������ ���� �̹����� ���� �����Ҽ� �ֵ����Ұ�.
	wstring fileNames[] =
	{
		L"./Textures/mainMenu.jpg",
		L"./Textures/CharacterSelect.jpg",
		L"./Textures/MainButton.png",
		L"./Textures/CharacterSelect1.jpg",
		L"./Textures/CharacterSelect2.jpg",
		L"./Textures/CharacterSelect3.jpg",
	};

	for (float j = 0; j < 2; j++)//���� 60, ���� 210
	{
		for (float i = 0; i < 3; i++)
		{
			MainMenuButton.push_back(Texture::Add(fileNames[2],
			{ 0 + (j * 210), 0 + (i * 60) }, { 210 + (j * 210) ,60 + (i * 60) }));
		}//for(i)���� ���� 210
	}//for(j)���α��� 60

	MainMenuIMG.push_back(Texture::Add(fileNames[0],
	{ 0, 0 }, { 1024,768}));
	MainMenuIMG.push_back(Texture::Add(fileNames[3],
	{ 0, 0 }, { 1024,768 }));
	MainMenuIMG.push_back(Texture::Add(fileNames[4],
	{ 0, 0 }, { 1024,768 }));
	MainMenuIMG.push_back(Texture::Add(fileNames[5],
	{ 0, 0 }, { 1024,768 }));

	

}

MenuManager::~MenuManager()
{
	//������ �̹����ִϸ��̼� ����
}

void MenuManager::Update()
{
	//�޴� ���ý� gamaManager�� state ����
	//���� � ��ư�� ������������ ���� �����ִ� �̹��� ����

	SelectButton();
	

}

void MenuManager::Render()
{
	//�޴� ���� �̹��� �����ٰ�.

	D3DXMATRIX world;

	D3DXMatrixIdentity(&world);
	shader->Begin(NULL, NULL);
	{

		D3DXMatrixTranslation(&world, 0, 0, 0);
		Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		shader->SetTexture("_map", MainMenuIMG[menuState]->GetTexture());
		shader->BeginPass(0);
		MainMenuIMG[menuState]->Render();
		shader->EndPass();


		switch (menuState)
		{
			case 0://����ȭ���ϰ�� ��ư����ϱ� �����ڵ�
				for (int i = 0; i < 3; i++)
					{
					int j;
					if (i == buttonActive-1)
						j = i + 3;
					else
						j = i;

						D3DXMatrixTranslation(&world, 430, 550 + (i * 60), 0);
						Device::GetDevice()->SetTransform(D3DTS_WORLD, &world);
						shader->SetTexture("_map", MainMenuButton[j]->GetTexture());
						shader->BeginPass(0);
						MainMenuButton[j]->Render();
						shader->EndPass();
					}
				break;
			case 1:

				break;
		}
		

	}
	shader->End();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void MenuManager::SelectButton()
{
	if (menuState == 0)//�޴�
	{
		if (Keyboard::GetInstance()->KeyDown(VK_UP))
		{
			switch (buttonActive)
			{
				case None: buttonActive = MainButtonActive::Start;
					break;
				case Start: buttonActive = MainButtonActive::_Quit;
					break;
				case Option: buttonActive = MainButtonActive::Start;
					break;
				case _Quit: buttonActive = MainButtonActive::Option;
					break;
			}//switch
		}//if
		else if (Keyboard::GetInstance()->KeyDown(VK_DOWN))
		{
			switch (buttonActive)
			{
				case None: buttonActive = MainButtonActive::Start;
					break;
				case Start: buttonActive = MainButtonActive::Option;
					break;
				case Option: buttonActive = MainButtonActive::_Quit;
					break;
				case _Quit: buttonActive = MainButtonActive::Start;
					break;
			}//switch
		}//if

		if (Select == true)
		{
			switch (buttonActive)
			{
				case MainButtonActive::None:
					break;
				case MainButtonActive::Start: menuState = 1;
					break;
				case MainButtonActive::Option: menuState = 0;
					//�ɼ� Ȱ��ȭ�ؾߵ�.
					break;
				case MainButtonActive::_Quit: exit(1);
					break;
			}//switch

			Select = false;
		}
	}//if(menuState == 0)

	if (menuState != 0)
	{
		if (Keyboard::GetInstance()->KeyDown(VK_UP))
		{
			if (menuState > 1)
				menuState--;
			else if (menuState == 1)
				menuState = 3;
		}//if(Keyboard::GetInstance()->KeyDown(VK_UP))
		else if (Keyboard::GetInstance()->KeyDown(VK_DOWN))
		{
			if (menuState < 3)
				menuState++;
			else if (menuState ==3)
				menuState = 1;
		}//if (Keyboard::GetInstance()->KeyDown(VK_DOWN))

		if (Select == true)
		{
			StartGame = true;
			Select = false;
			//SetCharacter();
		}
		//ĳ������â���� ESCŰ ������ �������� ��.
		if (Keyboard::GetInstance()->KeyDown(VK_ESCAPE))
		{
			menuState = 0;
			buttonActive = MainButtonActive::None;
	
		}
	}//if (menuState != 0)


	if (Keyboard::GetInstance()->KeyDown(VK_RETURN))
	{
		Select = true;
	}

	if(StartGame == true)
		menuState = 0;
}


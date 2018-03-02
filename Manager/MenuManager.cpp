#include "../stdafx.h"
#include"MenuManager.h"
#include"GameManager.h"

MenuManager::MenuManager(LPDIRECT3DDEVICE9 device)
	: Shader(device, L"./Render/Shader/default.hlsl") , menuState(0)//메뉴상태와 이미지
	, buttonActive (MainButtonActive::None)
	, Select(false) , StartGame(false)// , Character(1)
{
	//TODO: 이미지 불러올것. 배경화면에 맞는것 
	//버튼이미지관련 입력에 따라서 게임매니져의 state 바꿔줄것.
	//메뉴 상태에서는 메뉴나 설정에 관한 이미지를 띄우고 조정할수 있도록할것.
	wstring fileNames[] =
	{
		L"./Textures/mainMenu.jpg",
		L"./Textures/CharacterSelect.jpg",
		L"./Textures/MainButton.png",
		L"./Textures/CharacterSelect1.jpg",
		L"./Textures/CharacterSelect2.jpg",
		L"./Textures/CharacterSelect3.jpg",
	};

	for (float j = 0; j < 2; j++)//세로 60, 가로 210
	{
		for (float i = 0; i < 3; i++)
		{
			MainMenuButton.push_back(Texture::Add(fileNames[2],
			{ 0 + (j * 210), 0 + (i * 60) }, { 210 + (j * 210) ,60 + (i * 60) }));
		}//for(i)가로 길이 210
	}//for(j)세로길이 60

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
	//생성한 이미지애니메이션 삭제
}

void MenuManager::Update()
{
	//메뉴 선택시 gamaManager의 state 변경
	//현재 어떤 버튼을 선택중인지에 따라 보여주는 이미지 변경

	SelectButton();
	

}

void MenuManager::Render()
{
	//메뉴 관련 이미지 보여줄것.

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
			case 0://메인화면일경우 버튼출력하기 위한코드
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
	if (menuState == 0)//메뉴
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
					//옵션 활성화해야됨.
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
		//캐릭선택창에서 ESC키 누르면 메인으로 감.
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


#include "../stdafx.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GameOverManager.h"

GameManager::GameManager(LPDIRECT3DDEVICE9 device) : gameState(GameState::MainMenu)
{
	sceneManager = new SceneManager(Device::GetDevice());
	menuManager = new MenuManager(Device::GetDevice());
	gameOverManager = new GameOverManager(Device::GetDevice());

}

GameManager::~GameManager()
{
	SAFE_DELETE(gameOverManager);
	SAFE_DELETE(menuManager);
	SAFE_DELETE(sceneManager);
}


void GameManager::Initialize()
{
	sceneManager->Initialize();
}

void GameManager::Destroy()
{

//testing code
}

void GameManager::Update()
{
	

	switch (gameState)
	{
		case GameState::MainMenu: 	menuManager->Update();
			//메뉴에서 startGame을 true로 바꿔줄시 게임이 시작됨.
			if (menuManager->GetStartGameState() == true)
			{
				gameOverManager->ResetData();
				sceneManager->SceneResetData(menuManager->GetCharacterSelect());
				gameState = GameState::GameScene;
			}
				menuManager->SetStartGameState(false);

			break;
		case GameState::GameScene: 	sceneManager->Update();
			if (sceneManager->GetGameOver() == true)
			{
				int tempScore;
				tempScore = sceneManager->GetScore();
				gameOverManager->SetCurrentScore(tempScore);
				gameOverManager->SetGameOverStart();

				gameState = GameState::GameOver;
			}
			break;
		case GameState::GameOver: 	gameOverManager->Update();
		//	gameState = GameState::MainMenu;
			if ((gameOverManager->GetState() == 0) && (gameOverManager->GetSelect() == true))
			{
				
				gameState = GameState::MainMenu;
			}
			//게임오버를 탈출할때 씬 리셋	sceneManager->SceneResetData();
			break;
	}
}

void GameManager::Render()
{
	switch (gameState)
	{
		case GameState::MainMenu: 	menuManager->Render();
			break;

		case GameState::GameScene: 	sceneManager->Render();
			break;

		case GameState::GameOver: 	
			sceneManager->Render();
			gameOverManager->Render();
									

			break;

		default:
			break;
	}


}

void GameManager::ResetSceneData()
{
//	sceneManager->SceneResetData();
}

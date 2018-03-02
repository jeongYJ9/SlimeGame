#pragma once


class SceneManager;
class MenuManager;
class GameOverManager;

enum GameState
{
	MainMenu = 0,
	GameScene =1,
	GameOver = 2

};

class GameManager
{
public:
	GameManager(LPDIRECT3DDEVICE9 device);
	~GameManager();

	void Initialize();
	void Destroy();
	void Update();
	void Render();

	void ResetSceneData();
	

private:

	

	SceneManager* sceneManager;
	MenuManager* menuManager;
	GameOverManager* gameOverManager;
	GameState gameState;
};
#pragma once

#include "../Render/Shader/Shader.h"

class GameOverManager : public Shader
{
public:

	GameOverManager(LPDIRECT3DDEVICE9 device);
	~GameOverManager();

	void SetCurrentScore(int Score)
	{
		CurrentScore = Score;
	}

	void SetGameOverStart()
	{
		GameOverStart = true;
	}

	void Update();
	void Render();

	void ResetData();
	
	int GetState()
	{
		return state;
	}
	bool GetSelect()
	{
		return Select;
	}

private:

	vector<Texture *> GameOverIMG;

	vector<Texture *> Numbers;
	//vector<Texture *> GameOverButton;

	//
	bool GameOverStart;//점수를 한번만 받을것임.
	int CurrentScore;//Scene에서 가져온점수
	
	int HighScore;

	int RecentScore1;
	int RecentScore2;
	int RecentScore3;

	int state;//0 : None, 1: 다시하기,
	bool Select;//ture, false;

	//bool 
};
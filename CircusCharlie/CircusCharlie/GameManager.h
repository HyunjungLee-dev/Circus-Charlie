#pragma once
#include"Background.h"
#include"Player.h"
class GameManager
{
private:
	HWND	 m_hWnd;
	GAMESTATE m_eGameState;
	int m_iStage;
	Background m_Backgrd;
	Player m_player;
public:
	GameManager();
	~GameManager();

	void Init(HWND hWnd);
	void Update();
	void Release();
	void Render();
};


#pragma once
#include"Background.h"
#include"Player.h"
#include"Enemy.h"
class GameManager
{
private:
	HWND	 m_hWnd;
	GAMESTATE m_eGameState;
	RECT m_state;
	int m_iStage;
	int m_iDistance;

	Background m_Backgrd;
	Player m_player;
	Enemy m_enemy;
public:
	GameManager();
	~GameManager();

	void Init(HWND hWnd);
	void Update();
	void state();
	void Release();
	void Render();
};


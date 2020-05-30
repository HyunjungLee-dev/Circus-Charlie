#pragma once
#include"Font.h"
#include"Background.h"
#include"Player.h"
class GameManager
{
private:
	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;

	float	m_fStartTime;
	float	m_fTextTime;

	HWND m_hWnd;
	HDC m_backbufferDC;
	RECT m_ClientRct;

	Font m_Font;

	GAMESTATE m_eGameState;
	GAMESTATE m_eStartCheck;

	Stat m_stat;

	Background m_Backgrd;
	Player m_Player;


public:
	GameManager();
	~GameManager();

	//Init
	void Init(HWND hWnd);
	void StateReset();

	//Update
	void Update();
	void MainUpdate();
	void StatUpdate();
	void EndLine();
	void EndScore();

	//Render
	void Render();
	void Main();
	void Stage();
	void StatWindow();
	void StatBox();

	//Release
	void Release();

};


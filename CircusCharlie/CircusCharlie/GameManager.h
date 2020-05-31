#pragma once
#include"Font.h"
#include"Background.h"
#include"Player.h"
#include"Enemy.h"
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
	LINE m_eLine;
	PASS m_ePasscheck;
	bool m_bPassRingtype;

	Stat m_stat;

	Background m_Backgrd;
	Player m_Player;
	Enemy m_Enemy;


	vector<Star*> m_Star;

public:
	GameManager();
	~GameManager();

	//Init
	void Init(HWND hWnd);
	void StateReset();
	void SetMainStar();

	//Update
	void Update();
	void ObjectUpdate();
	void MainUpdate();
	void StatUpdate();

	void EndLine();
	void EndScore();

	void Collision(); //Ãæµ¹
	void EnemyPass(); 

	void StarUpdate();
	void StarColor(IMG* s);


	//Render
	void Render();
	void Main();
	void Stage();
	void StatWindow();
	void StatBox();
	void StarRender();

	//Release
	void Release();
	void ObejectClear();

};


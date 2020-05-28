#pragma once
#include"Background.h"
#include"Player.h"
#include"Enemy.h"

struct Star
{
	ICON_IMG starColor;
	POS pos;
};


class GameManager
{
private:
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	HWND	 m_hWnd;
	GAMESTATE m_eGameState;
	RECT m_state;

	int m_iScore; // 구조체로
	int m_iStage;
	int m_iBonus;
	int m_iHI;

	int m_iDistance;
	PASS Passcheck;
	bool Ringtype;

	Background m_Backgrd;
	Player m_player;
	Enemy m_enemy;
	vector<Star*> star;
public:
	GameManager();
	~GameManager();

	void Init(HWND hWnd);
	void StateReset();



	void Update();
	void TextUpdate();
	void Font(int x, int y, TCHAR *str, COLORREF color);
	void End();
	void TextRender();
	void Title();
	void TitleStar();
	void Stage();
	void EndScore();
	void EnemyPass();
	void Collision();
	void Release();
	void Render();
};


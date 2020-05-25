#pragma once
#include"Background.h"
#include"Player.h"
#include"Enemy.h"
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
	int m_Life;

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
	void Font(HDC hdc, int x, int y, TCHAR *str, COLORREF color);
	void TextRender();
	void Collision();
	void Release();
	void Render();
};


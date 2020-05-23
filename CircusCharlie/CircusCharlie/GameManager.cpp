#include "GameManager.h"



GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_eGameState = GAME_PLAY;
	m_iStage = 1;
	BitMapManager::GetSingleton()->Init(hWnd);
	m_Backgrd.Init(0, 100);
	m_enemy.Init();
	m_iDistance = 0;
	m_state = { 0,0,514,100 };
}

void GameManager::Update()
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (m_iDistance == 0)
			return;
		m_iDistance -= 100;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)		
	{
		m_iDistance += 100;
	}
	m_enemy.Update();
	m_player.Update();
	m_Backgrd.Update();

	Render();
}

void GameManager::state()
{
	HDC hdc = GetDC(m_hWnd);
	TCHAR str[128];
	HFONT hFont, OldFont;
	hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	wsprintf(str, TEXT("Test"));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, 0x00ffffff);
	TextOut(hdc, 0,0, str, lstrlen(str));

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
	//InvalidateRect(m_hWnd, &m_state, false);
}

void GameManager::Render()
{
	//배경
	//캐릭터
	//상태바
	HDC hdc = GetDC(m_hWnd);

	if (m_eGameState == GAME_START) // 시작 화면
	{

	}
	else if (m_eGameState == GAME_PLAY)
	{
		//state();

		m_Backgrd.Render();
		m_enemy.Render();
		m_player.Render();
		m_enemy.HalfRender();
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc);
	}
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::Release()
{
	BitMapManager::Release();
}

GameManager::~GameManager()
{
}

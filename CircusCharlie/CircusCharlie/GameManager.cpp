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
}

void GameManager::Update()
{
	m_player.Update(m_hWnd);
	m_Backgrd.Update();
	Render();
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
		m_Backgrd.Render();
		m_player.Render(m_hWnd);
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

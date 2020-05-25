#include "GameManager.h"



GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_eGameState = GAME_PLAY;
	m_iBonus = 5000;
	m_iStage = 1;
	m_iDistance = 0;
	m_iScore = 0;
	m_iHI = 20000;
	m_Life = 4;
	m_state = { 0,0,514,100 };

	BitMapManager::GetSingleton()->Init(hWnd);
	m_Backgrd.Init(0, 100);
	m_player.Init();
	m_enemy.Init();

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
	state();
	m_enemy.Update();
	m_player.Update();
	Collision();
	m_Backgrd.Update();
	Render();
}

void GameManager::Collision()
{
	if (m_enemy.Collision(m_player.GetPlayerRct()))
	{
		if(m_player.GetState() != JUMP)
			m_player.SetPlayerMotion(PLAYER_DIE);
	}
}

void GameManager::Font(HDC hdc,int x, int y, TCHAR *str, COLORREF color)
{
	HFONT hFont, OldFont;
	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Terminal"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	SetBkColor(hdc, 0x00000000);
	SetTextColor(hdc, color);

	TextOut(hdc, x, y, str, lstrlen(str));


	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}

void GameManager::TextRender()
{
	TCHAR str[128];
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	wsprintf(str, TEXT("STAGE-%02d"), m_iStage);
	Font(hdc,350, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("HI-%06d"), m_iHI);
	Font(hdc,210, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("%06d"), m_iScore);
	Font(hdc,105, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("BONUS"));
	Font(hdc,210, 60, str, RGB(255, 0, 127));


	int x = 430;
	int y = 60;

	for (int i = 0; i < m_Life; i++)
	{
		if (i == 0)
			BitMapManager::GetSingleton()->GetIcon(ICON_LIFE).Draw(hdc, x, y, 1, 1);
		else
		{
			x -= BitMapManager::GetSingleton()->GetIcon(ICON_LIFE).GetSize().cx;
			BitMapManager::GetSingleton()->GetIcon(ICON_LIFE).Draw(hdc, x, y, 1, 1);
		}
	}

}

void GameManager::state() 
{

	TCHAR str[128];
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if ( m_fDeltaTime > 0.3f) 
	{
		wsprintf(str, TEXT("1P-"));
		Font(hdc,70, 40, str, 0x00ffffff);

		m_iBonus -= 10;
		if (m_iBonus < 0)
			m_iBonus = 0;
		m_dwLastTime = m_dwCurTime;
	}
	wsprintf(str, TEXT("-%d"), m_iBonus);
	Font(hdc, 270, 60, str, 0x00ffffff);
	InvalidateRect(m_hWnd, &m_state, false);
	//ReleaseDC(m_hWnd, hdc);
}


void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);
	if (m_eGameState == GAME_START) // 시작 화면
	{
	}
	else if (m_eGameState == GAME_PLAY) // 게임 플레이
	{
		TextRender();
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

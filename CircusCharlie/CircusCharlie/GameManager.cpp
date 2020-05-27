#include "GameManager.h"



GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd)
{

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_hWnd = hWnd;
	m_eGameState = GAME_START;
	m_iBonus = 5000;
	m_iStage = 1;
	m_iScore = 0;
	m_iHI = 20000;
	m_state = { 0,0,514,100 };
	Passcheck = PASS_NOT;
	Ringtype = false;

	BitMapManager::GetSingleton()->Init(hWnd);
	m_Backgrd.Init(0, 100);
	m_player.Init();
	m_enemy.Init();

}

void GameManager::Update()
{
	if (m_eGameState == GAME_STOP)
	{
		Collision();
		TextRender();
		return;
	}
	else if (m_eGameState == GAME_START || m_eGameState == GAME_RE) // 시작 화면
	{
		Stage();
	}
	else if (m_eGameState == GAME_END)
	{
		m_player.Update(ENDPOS);
		m_Backgrd.EndMotion();
		m_enemy.backRing();
		EndScore();
		TextRender();
		Render();
	}
	else if (m_eGameState == GAME_PLAY) // 게임 플레이
	{
		TextUpdate();

		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			if (m_player.GetPlayX() < m_Backgrd.GetMitterPos(0)*0.35)
			{
				m_enemy.Update(NOTEND);
				//Collision();
				EnemyPass();
				Render();
				return;
			}
		}
	
		if (m_player.GetPlayX() > m_Backgrd.GetMitterPos(9))
		{
			m_enemy.Update(ENDLINE);
			End();
			m_Backgrd.CheckDistacne(m_player.GetPlayX());
			//Collision();
			EnemyPass();
			Render();
			return;
		}
		
		m_enemy.Update(NOTEND);
		m_player.Update(NOTEND);
		m_Backgrd.CheckDistacne(m_player.GetPlayX());
		m_Backgrd.Update();
	//	Collision();
		EnemyPass();
		Render();
	}
}

void GameManager::Stage()		
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	RECT temp;
	GetClientRect(m_hWnd, &temp);

	TCHAR str[128];
	TextRender();
	wsprintf(str, TEXT("STAGE-%02d"), m_iStage);
	Font(temp.right*0.4 , temp.bottom*0.5, str, 0x00ffffff);
	if (m_fDeltaTime > 3.0f)
	{
		if (m_eGameState == GAME_RE)  
		{

			if (m_player.GetPlayX() < m_Backgrd.GetMitterPos(0)*0.35)
			{
				m_enemy.backRing();
			}
			else
			{
				m_Backgrd.backBgd(m_player.GetPlayX());
				m_enemy.backRing();
				m_enemy.backEnemy(m_Backgrd.GetBacklength());
			}
		}
		m_eGameState = GAME_PLAY;
		m_player.SetLife();
		Passcheck = PASS_NOT;
		m_dwLastTime = m_dwCurTime;
	}
}

void GameManager::EnemyPass() 
{
	PASS Rpatype, Jpatype;

	if (m_player.GetState() == IDLE && Passcheck != PASS_NOT)
	{
		switch (Passcheck)
		{
		case PASS_JAR:
			m_iScore += (int)PASS_JAR;
			break;
		case PASS_RING:
			m_iScore += (int)PASS_RING;
			break;
		case PASS_DOUBLE:
			if (Ringtype)
			{
				m_iScore += (int)(PASS_RING + PASS_JAR + PASS_ITEMRING + PASS_BONOUS);
				Ringtype = false;
			}
			else
				m_iScore += (int)(PASS_RING + PASS_JAR + PASS_BONOUS);
			break;
		case PASS_ITEMRING:
			m_iScore += (int)(PASS_RING + PASS_ITEMRING);
			break;

		}
		Passcheck = PASS_NOT;
	}

	if (m_player.GetState() == JUMP)
	{
		Rpatype = m_enemy.RingPassCheck(m_player.GetPlayX());
		Jpatype = m_enemy.PassCheck(m_player.GetPlayX());

		if (Passcheck != PASS_DOUBLE)
		{
		
			if(Jpatype != PASS_NOT)
				Passcheck = PASS_JAR;
			if (Rpatype == PASS_ITEMRING)
			{
				Passcheck = PASS_ITEMRING;
			}
			else if (Rpatype == PASS_RING)
			{
				Passcheck = PASS_RING;
			}
		}
		if (Rpatype != PASS_NOT && Jpatype != PASS_NOT)
		{
			if (Rpatype == PASS_ITEMRING)
			{
				Ringtype = true;
			}
			Passcheck = PASS_DOUBLE;
		}
	}

	
}

void GameManager::EndScore()
{
	int tmp = m_iBonus;
	if (tmp != 0)
	{
		tmp -= 10;
		m_iScore += 10;
	}
	if (m_iBonus != 0)
		m_iBonus -= 10;
}

void GameManager::End()
{
	if (m_Backgrd.EndCheck(m_player.GetPlayerRct()))
	{
		m_eGameState = GAME_END;
		m_player.Update(ENDPOS);
	}
	else
		m_player.Update(ENDLINE);
}

void GameManager::Collision()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_enemy.Collision(m_player.GetPlayerRct()))
	{
		m_player.SetPlayerMotion(PLAYER_DIE);
		m_eGameState = GAME_STOP;
		if (m_fDeltaTime > 2.0f)
		{
			m_eGameState = GAME_RE;
			InvalidateRect(m_hWnd, NULL, TRUE);
			m_dwLastTime = m_dwCurTime;
		}
	}


}

void GameManager::Font(int x, int y, TCHAR *str, COLORREF color)
{
	HDC hdc = GetDC(m_hWnd);
	HFONT hFont, OldFont;
	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Terminal"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	SetBkColor(hdc, 0x00000000);
	SetTextColor(hdc, color);

	TextOut(hdc, x, y, str, lstrlen(str));


	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::TextRender() // 윈도우 작업영역 기준으로 바꾸기
{
	TCHAR str[128];
	HDC hdc = GetDC(m_hWnd);
	if (m_eGameState != GAME_PLAY)
	{
		wsprintf(str, TEXT("1P-"));
		Font(70, 40, str, 0x00ffffff);
	}
	wsprintf(str, TEXT("-%d"), m_iBonus);
	Font(270, 60, str, 0x00ffffff);

	wsprintf(str, TEXT("STAGE-%02d"), m_iStage);
	Font(350, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("HI-%06d"), m_iHI);
	Font(210, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("%06d"), m_iScore);
	Font(105, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("BONUS"));
	Font(210, 60, str, RGB(255, 0, 127));


	int x = 430;
	int y = 60;

	for (int i = 0; i < m_player.GetLife(); i++)
	{
		if (i == 0)
			BitMapManager::GetSingleton()->GetIcon(ICON_LIFE).Draw(hdc, x, y, 1, 1);
		else
		{
			x -= BitMapManager::GetSingleton()->GetIcon(ICON_LIFE).GetSize().cx;
			BitMapManager::GetSingleton()->GetIcon(ICON_LIFE).Draw(hdc, x, y, 1, 1);
		}
	}
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::TextUpdate() // 윈도우 작업 영역 기준으로
{

	TCHAR str[128];

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if ( m_fDeltaTime > 0.3f) 
	{
		wsprintf(str, TEXT("1P-"));
		Font(70, 40, str, 0x00ffffff);

		m_iBonus -= 10;
		if (m_iBonus < 0)
			m_iBonus = 0;
		//InvalidateRect(m_hWnd, NULL, false);
		m_dwLastTime = m_dwCurTime;
	}
	wsprintf(str, TEXT("-%d"), m_iBonus);
	Font(270, 60, str, 0x00ffffff);
	TextRender();
}


void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);
	m_Backgrd.Render();
	m_enemy.Render();
	m_player.Render();
	m_enemy.HalfRender();	
	BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::Release()
{
	BitMapManager::Release();
}

GameManager::~GameManager()
{
}

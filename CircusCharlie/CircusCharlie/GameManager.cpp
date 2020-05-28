#include "GameManager.h"



GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd)
{

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_hWnd = hWnd;

	m_state = { 0,0,514,100 };
	Passcheck = PASS_NOT;
	Ringtype = false;

	BitMapManager::GetSingleton()->Init(hWnd);

	TitleStar();
	StateReset();


}

void GameManager::StateReset()
{
	m_eGameState = GAME_MAIN;
	m_iBonus = 5000;
	m_iStage = 1;
	m_iScore = 0;
	m_iHI = 20000;

	Passcheck = PASS_NOT;
	Ringtype = false;

	m_player.Init();
	m_Backgrd.Init(0, 100);
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
	else if (m_eGameState == GAME_MAIN || m_eGameState == GAME_INIT)
	{
		Title();
	}
	else if (m_eGameState == GAME_START || m_eGameState == GAME_RE || m_eGameState == GAME_DIE)
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
	else if (m_eGameState == GAME_PLAY)
	{
		TextUpdate();

		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			if (m_player.GetPlayX() < m_Backgrd.GetMitterPos(0)*0.35)
			{
				m_enemy.Update(NOTEND);
			Collision();
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
			Collision();
			EnemyPass();
			Render();
			return;
		}
		
		m_enemy.Update(NOTEND);
		m_player.Update(NOTEND);
		m_Backgrd.CheckDistacne(m_player.GetPlayX());
		m_Backgrd.Update();
		Collision();
		EnemyPass();
		Render();
	}
}

void GameManager::Stage()		
{
	RECT temp;
	GetClientRect(m_hWnd, &temp);

	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	PatBlt(hdc, 0, 0, temp.right, temp.bottom, BLACKNESS);
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;


	TCHAR str[128];
	if (m_eGameState == GAME_DIE)
	{
		wsprintf(str, TEXT("GAME OVER"));
		Font(temp.right*0.4, temp.bottom*0.5, str, 0x00ffffff);
	}
	else
	{
		wsprintf(str, TEXT("STAGE-%02d"), m_iStage);
		Font(temp.right*0.4, temp.bottom*0.5, str, 0x00ffffff);
	}
	TextRender();
	if (m_fDeltaTime > 3.0f)
	{
		if (m_eGameState == GAME_DIE)
		{
			m_eGameState = GAME_INIT;
			return;
		}
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

void GameManager::Title()
{
	TCHAR str[128];
	RECT temp;
	GetClientRect(m_hWnd, &temp);

	HDC hdc2 = GetDC(m_hWnd);
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (m_eGameState == GAME_INIT)
		{
			Release();
			StateReset();
		}
		m_eGameState = GAME_START;
	}

	
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;


	if (m_fDeltaTime > 0.1f)
	{
		for (int i = 0; i < 46; i++)
		{
			switch (star[i]->starColor)
			{
			case ICON_STAR_B:
				star[i]->starColor = ICON_STAR_Y;
				break;
			case ICON_STAR_Y:
				star[i]->starColor = ICON_STAR_R;
				break;
			case ICON_STAR_R:
				star[i]->starColor = ICON_STAR_B;
				break;
			}
		}
		m_dwLastTime = m_dwCurTime;
	}


	BitMapManager::GetSingleton()->GetIcon(ICON_TITLE).Draw(hdc, temp.right*0.27, temp.bottom*0.23, 1);
	wsprintf(str, TEXT("PLAY SELECT"), m_iStage);
	Font(temp.right*0.4, temp.bottom*0.6, str, 0x00ffffff);

	wsprintf(str, TEXT("ขั 1 PLAYER A"), m_iStage);
	Font(temp.right*0.35, temp.bottom*0.7, str, 0x00ffffff);
	for (int i = 0; i < 46; i++)
	{
		BitMapManager::GetSingleton()->GetIcon(star[i]->starColor).Draw(hdc, star[i]->pos.m_fX, star[i]->pos.m_fY, 1, 1);
	}
	BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc2);
	PatBlt(hdc, 0, 0, temp.right, temp.bottom, BLACKNESS);
	ReleaseDC(m_hWnd, hdc2);
}

void GameManager::TitleStar()
{
	ICON_IMG tmp = ICON_STAR_B;
	RECT temp;
	GetClientRect(m_hWnd, &temp);


	for (int i = 0; i < 46; i++)
	{
		star.push_back(new Star);
		switch (tmp)
		{
		case ICON_STAR_B:
			tmp = ICON_STAR_Y;
			break;
		case ICON_STAR_Y:
			tmp = ICON_STAR_R;
			break;
		case ICON_STAR_R:
			tmp = ICON_STAR_B;
			break;
		}
		star[i]->starColor = tmp;
		if (i < 16)
		{
			star[i]->pos.m_fX = temp.right*0.25 + i * BitMapManager::GetSingleton()->GetIcon(tmp).GetSize().cx * 1.2;
			star[i]->pos.m_fY = temp.bottom*0.18;
		}
		else if (i < 32)
		{
			star[i]->pos.m_fX = temp.right*0.25 + (i - 16)* BitMapManager::GetSingleton()->GetIcon(tmp).GetSize().cx * 1.2;
			star[i]->pos.m_fY = temp.bottom*0.23 + BitMapManager::GetSingleton()->GetIcon(ICON_TITLE).GetSize().cy;
		}
		else if (i < 39)
		{
			float y = (i - 32) * BitMapManager::GetSingleton()->GetIcon(tmp).GetSize().cy * 1.2;
			star[i]->pos.m_fX = temp.right*0.21;
			star[i]->pos.m_fY = temp.bottom*0.21 + y;
		}
		else
		{
			float y = (i - 39) * BitMapManager::GetSingleton()->GetIcon(tmp).GetSize().cy * 1.2;
			star[i]->pos.m_fX = temp.right*0.29 + BitMapManager::GetSingleton()->GetIcon(ICON_TITLE).GetSize().cx;
			star[i]->pos.m_fY = temp.bottom*0.21 + y;
		}
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
		m_enemy.ResetItem();
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
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	PatBlt(hdc, 0, 0, m_state.right, m_state.bottom, BLACKNESS);

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	int tmp = m_iBonus;
	if (tmp != 0)
	{
		tmp -= 10;
		m_iScore += 10;
	}
	if (m_iBonus != 0)
		m_iBonus -= 10;

	if (m_iBonus == 0)
	{
		if (m_fDeltaTime > 15.0f)
		{
			m_eGameState = GAME_INIT;
			m_dwLastTime = m_dwCurTime;
		}
	}
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
			if(m_player.GetLife() <= 0)
				m_eGameState = GAME_DIE;
			else
				m_eGameState = GAME_RE;
			InvalidateRect(m_hWnd, NULL, TRUE);
			m_dwLastTime = m_dwCurTime;
		}
	}


}

void GameManager::Font(int x, int y, TCHAR *str, COLORREF color)
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	HFONT hFont, OldFont;
	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Terminal"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	SetBkColor(hdc, 0x00000000);
	SetTextColor(hdc, color);

	TextOut(hdc, x, y, str, lstrlen(str));


	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
	ReleaseDC(m_hWnd, hdc);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::TextRender() 
{
	TCHAR str[128];
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	HDC hdc2 = GetDC(m_hWnd);

	RECT rc1 = {60,30,460,80};
	RECT rc2 = {55,25,465,85 };

	HPEN hpen, hpenOld;
	HBRUSH myBrush, oldBrush;


	myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

	hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 127));
	hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

	Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);

	hpen = CreatePen(PS_SOLID, 2, RGB(0, 216, 255));
	hpenOld = (HPEN)::SelectObject(hdc, (HGDIOBJ)hpen);

	Rectangle(hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);

	hpen = (HPEN)::SelectObject(hdc, hpenOld);
	DeleteObject(hpen);

	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);


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
	BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc2);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::TextUpdate() 
{

	TCHAR str[128];
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	PatBlt(hdc, 0, 0, m_state.right, m_state.bottom, BLACKNESS);

	if ( m_fDeltaTime > 0.3f) 
	{
		wsprintf(str, TEXT("1P-"));
		Font(70, 40, str, 0x00ffffff);

		m_iBonus -= 10;
		if (m_iBonus < 0)
			m_iBonus = 0;
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
	m_Backgrd.Release();
	m_enemy.Release();
}

GameManager::~GameManager()
{
	Release();
	BitMapManager::Release();
}

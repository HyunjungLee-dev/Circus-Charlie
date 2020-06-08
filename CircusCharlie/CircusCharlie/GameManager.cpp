#include "GameManager.h"



GameManager::GameManager()
{
}

// Init

void GameManager::Init(HWND hWnd)
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_hWnd = hWnd;
	GetClientRect(m_hWnd, &m_ClientRct);
	BitMapManager::GetSingleton()->Init(hWnd);
	m_backbufferDC = BitMapManager::GetSingleton()->GetBufferDC();
	m_Font.Init();

	m_eGameState = GAME_MAIN;
	m_eStartCheck = GAME_NONE;
	m_fStartTime = 0.0f;
	m_fTextTime = 0.0f;
	SetMainStar();

}


void GameManager::StateReset()
{
	m_stat.Bonus = 5000;
	m_stat.Score = 0;
	m_stat.Stage = 1;
	m_stat.Hi = 20000;

	m_ePasscheck = PASS_NOT;

	m_Backgrd.Init(0, 100);
	m_Player.Init();
	m_Enemy.Init();

}

void GameManager::SetMainStar()
{
	IMG Color = ICON_STAR_B;
	int iconsizeX = BitMapManager::GetSingleton()->GetImg(Color)->GetSize().cx;
	int StartX = m_ClientRct.right*0.22;
	int StartY = m_ClientRct.bottom*0.18;

	for (int i = 1; i <= 16; i++)
	{
		StarColor(&Color);
		m_Star.push_back(new Star);
		m_Star.back()->pos.m_fX = StartX + i * iconsizeX*1.2;
		m_Star.back()->pos.m_fY = StartY;
		m_Star.back()->starColor = Color;
	}

	for (int i = 1; i <= 16; i++)
	{
		StarColor(&Color);
		m_Star.push_back(new Star);
		m_Star.back()->pos.m_fX = StartX + i * iconsizeX*1.2;
		m_Star.back()->pos.m_fY = StartY  + BitMapManager::GetSingleton()->GetImg(ICON_TITLE)->GetSize().cy * 1.25;
		m_Star.back()->starColor = Color;
	}

	StartY = m_ClientRct.bottom*0.18;

	for (int i = 0; i < 6; i++)
	{
		int y = BitMapManager::GetSingleton()->GetImg(Color)->GetSize().cy * 1.45;
		StartX = m_ClientRct.right*0.21;
		StartY += y;

		StarColor(&Color);
		m_Star.push_back(new Star);
		m_Star.back()->pos.m_fX = StartX;
		m_Star.back()->pos.m_fY = StartY;
		m_Star.back()->starColor = Color;
	}

	StartY = m_ClientRct.bottom*0.18;

	for (int i = 0; i < 6; i++)
	{
		StarColor(&Color);
		int y = BitMapManager::GetSingleton()->GetImg(Color)->GetSize().cy * 1.45;
		StartX = m_ClientRct.right*0.29 + BitMapManager::GetSingleton()->GetImg(ICON_TITLE)->GetSize().cx;
		StartY += y;

		StarColor(&Color);
		m_Star.push_back(new Star);
		m_Star.back()->pos.m_fX = StartX;
		m_Star.back()->pos.m_fY = StartY;
		m_Star.back()->starColor = Color;
	}
}

//Update
void GameManager::Update()
{
	if (m_eGameState == GAME_MAIN || m_eGameState == GAME_INIT)
		MainUpdate();
	else if (m_eGameState == GAME_START || m_eGameState == GAME_RE || m_eGameState == GAME_DIE)
		Stage();
	else if (m_eGameState == GAME_PLAY)
	{
		StatUpdate();

		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			if (m_Player.GetPlayX() < m_Backgrd.GetMitterPos(0))
			{
				m_eLine = STARTLINE;
				ObjectUpdate();
				return;
			}
		}
		if (m_Player.GetPlayX() > m_Backgrd.GetMitterPos(9)*1.2)
		{
			m_eLine = ENDLINE;
			ObjectUpdate();
			return;
		}
		m_eLine = NOTEND;
		ObjectUpdate();
	}
	else if (m_eGameState == GAME_STOP)
	{
		Collision();
		StatWindow();
		return;
	}
	else if (m_eGameState == GAME_END)
	{
		m_Backgrd.EndMotion();
		m_Backgrd.Update(ENDPOS,m_Player.GetMove());
		m_Player.Update(ENDPOS);
		StatWindow();
		EndScore();
	}
	Render();
}

void GameManager::ObjectUpdate()
{
	Collision(); 
	EnemyPass();

	if (m_eLine == NOTEND)
	{
		m_Backgrd.Update(NOTEND,m_Player.GetMove());
		m_Enemy.Update(NOTEND, m_Player.GetMove());
		m_Player.Update(NOTEND);
		m_Backgrd.CheckDistacne(m_Player.GetPlayX());
		m_Enemy.HalfRender();
	}
	else if (m_eLine == STARTLINE)
	{
		m_Backgrd.Render();
		m_Player.Update(NOTEND);
		m_Enemy.Update(STARTLINE, m_Player.GetMove());
		m_Enemy.HalfRender();
		Render();
	}
	else if (m_eLine == ENDLINE)
	{
		m_Backgrd.Update(ENDLINE,m_Player.GetMove());
		m_Enemy.Update(ENDLINE, m_Player.GetMove());
		EndLine();
		m_Backgrd.CheckDistacne(m_Player.GetPlayX());
		m_Enemy.HalfRender();
		Render();
	}
}

void GameManager::Collision()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_Enemy.Collision(m_Player.GetPlayerRct()))
	{
		m_Player.SetPlayerMotion(PLAYER_DIE);
		m_eGameState = GAME_STOP;
		if (m_fDeltaTime > 2.0f)
		{
			if (m_Player.GetLife() <= 0)
				m_eGameState = GAME_DIE;
			else
				m_eGameState = GAME_RE;
			m_dwLastTime = m_dwCurTime;
		}
	}
}

void GameManager::MainUpdate()
{
	StarUpdate();

	if (m_eGameState == GAME_INIT)
	{
		ObejectClear();
		PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS); 
		m_eGameState = GAME_MAIN;
	}
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		m_dwLastTime = GetTickCount();
		m_eStartCheck = GAME_WAIT;
	}
	Main();

}

void GameManager::StatUpdate()
{

	TCHAR str[128];

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_fDeltaTime > 0.3f)
	{
		m_stat.Bonus -= 10;
		if (m_stat.Bonus < 0)
			m_stat.Bonus = 0;
		m_fTextTime += m_fDeltaTime;
		m_dwLastTime = m_dwCurTime;
	}
	StatWindow();
}

void GameManager::EndLine()
{
	if (m_Backgrd.PodiumCheck(m_Player.GetPlayerRct()))
	{
		m_eGameState = GAME_END;
		m_Player.Update(ENDPOS);
	}
	else
		m_Player.Update(ENDLINE);
}

void GameManager::EndScore()
{
	int m_eStarColor = m_stat.Bonus;
	if (m_eStarColor != 0)
	{
		m_eStarColor -= 10;
		m_stat.Score += 10;
	}
	if (m_stat.Bonus != 0)
		m_stat.Bonus -= 10;

	if (m_stat.Bonus == 0)
	{
		m_dwCurTime = GetTickCount();
		m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

		if (m_fDeltaTime > 10.0f)
		{
			m_eGameState = GAME_INIT;
			m_dwLastTime = m_dwCurTime;
		}
	}
}

void GameManager::EnemyPass()
{
	PASS Rpatype, Jpatype;

	if (!m_Player.GetMove() && m_ePasscheck != PASS_NOT)
	{
		switch (m_ePasscheck)
		{
		case PASS_JAR:
			m_stat.Score += (int)PASS_JAR;
			break;
		case PASS_RING:
			m_stat.Score += (int)PASS_RING;
			break;
		case PASS_DOUBLE:
			if (m_bPassRingtype)
			{
				m_stat.Score += (int)(PASS_RING + PASS_JAR + PASS_ITEMRING + PASS_BONOUS);
				m_bPassRingtype = false;
			}
			else
				m_stat.Score += (int)(PASS_RING + PASS_JAR + PASS_BONOUS);
			break;
		case PASS_ITEMRING:
			m_stat.Score += (int)(PASS_RING + PASS_ITEMRING);
			break;

		}
		m_ePasscheck = PASS_NOT;
	}

	if (m_Player.GetMove())
	{
		Rpatype = m_Enemy.RingPassCheck(m_Player.GetPlayX());
		Jpatype = m_Enemy.JarPassCheck(m_Player.GetPlayX());

		if (m_ePasscheck != PASS_DOUBLE)
		{

			if (Jpatype != PASS_NOT)
				m_ePasscheck = PASS_JAR;
			if (Rpatype == PASS_ITEMRING)
			{
				m_ePasscheck = PASS_ITEMRING;
			}
			else if (Rpatype == PASS_RING)
			{
				m_ePasscheck = PASS_RING;
			}
		}
		if (Rpatype != PASS_NOT && Jpatype != PASS_NOT)
		{
			if (Rpatype == PASS_ITEMRING)
			{
				m_bPassRingtype = true;
			}
			m_ePasscheck = PASS_DOUBLE;
		}
	}
}

void GameManager::StarUpdate()
{
	
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_eStartCheck != GAME_WAIT)
	{
		if (m_fDeltaTime > 0.3f)
		{
			PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS);
			for (int i = 0; i < m_Star.size(); i++)
			{
				StarColor(&m_Star[i]->starColor);
			}
			m_dwLastTime = m_dwCurTime;
		}
	}
	StarRender();


}


void GameManager::StarRender()
{
	for (int i = 0; i < m_Star.size(); i++)
	{
		BitMapManager::GetSingleton()->GetImg(m_Star[i]->starColor)->Draw(m_backbufferDC, m_Star[i]->pos.m_fX, m_Star[i]->pos.m_fY, 1, 1);
	}
}

void GameManager::StarColor(IMG* s)
{
	switch (*s)
	{
	case ICON_STAR_B:
		*s = ICON_STAR_Y;
		break;
	case ICON_STAR_Y:
		*s = ICON_STAR_R;
		break;
	case ICON_STAR_R:
		*s = ICON_STAR_B;
		break;
	}
}


//Render
void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);
	BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc);
	ReleaseDC(m_hWnd, hdc);
}

void  GameManager::Main()
{
	RECT SelectRct = { m_ClientRct.right * 0.3, m_ClientRct.bottom * 0.68, m_ClientRct.right * 0.7, m_ClientRct.bottom * 0.75 };



	BitMapManager::GetSingleton()->GetImg(ICON_TITLE)->
		Draw(m_backbufferDC, m_ClientRct.right*0.27, m_ClientRct.bottom*0.23, 1);

	m_Font.Text(m_ClientRct.right*0.4, m_ClientRct.bottom*0.6, L"PLAY SELECT", 0x00ffffff);

	if (m_eStartCheck == GAME_WAIT)
	{
		m_dwCurTime = GetTickCount();
		m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

		if (m_fDeltaTime > 0.2f)
		{
			PatBlt(m_backbufferDC, SelectRct.left, SelectRct.top, SelectRct.right, SelectRct.bottom, BLACKNESS);
		}
		if (m_fDeltaTime > 0.4f)
		{
			m_Font.Text(m_ClientRct.right*0.35, m_ClientRct.bottom*0.7, L"ขั 1 PLAYER A", 0x00ffffff);
			m_dwLastTime = m_dwCurTime;
			m_fStartTime += m_fDeltaTime;
		}
		if (m_fStartTime > 2.0f)
		{
			m_eStartCheck = GAME_NONE;
			m_fStartTime = 0.0f;
			StateReset();
			m_eGameState = GAME_START;
		}
	}
	else
		m_Font.Text(m_ClientRct.right*0.35, m_ClientRct.bottom*0.7, L"ขั 1 PLAYER A", 0x00ffffff);

}

void GameManager::Stage()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS); 

	StatWindow();

	TCHAR str[128];
	if (m_eGameState == GAME_DIE)
	{
		wsprintf(str, TEXT("GAME OVER"));
	}
	else
		wsprintf(str, TEXT("STAGE-%02d"), m_stat.Stage);
	m_Font.Text(m_ClientRct.right*0.4, m_ClientRct.bottom*0.5, str, 0x00ffffff);

	if (m_fDeltaTime > 3.0f)
	{
		if (m_eGameState == GAME_DIE)
		{
			m_eGameState = GAME_INIT;
			return;
		}
		if (m_eGameState == GAME_RE)
		{

			if (m_Player.GetPlayX() < m_Backgrd.GetMitterPos(0)*0.35)
			{
				m_Enemy.RingClear();
			}
			else
			{
				m_Backgrd.RestoreBckgrd(m_Player.GetPlayX());
				m_Enemy.RingClear();
				m_Enemy.RestoreEnemy(m_Backgrd.GetBacklength());
			}
		}
		PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS);
		m_eGameState = GAME_PLAY;
		m_Player.SetPlayerMotion(PLAYER_MOVE0);
		m_Player.SetLife();
		m_dwLastTime = m_dwCurTime;
	}
}

void GameManager::StatBox()
{
	RECT rc1 = { 60,30,460,80 };
	RECT rc2 = { 55,25,465,85 };

	HPEN hpen, hpenOld;
	HBRUSH myBrush, oldBrush;


	myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oldBrush = (HBRUSH)SelectObject(m_backbufferDC, myBrush);

	hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 127));
	hpenOld = (HPEN)::SelectObject(m_backbufferDC, (HGDIOBJ)hpen);

	Rectangle(m_backbufferDC, rc1.left, rc1.top, rc1.right, rc1.bottom);

	hpen = (HPEN)::SelectObject(m_backbufferDC, hpenOld);
	DeleteObject(hpen);


	myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oldBrush = (HBRUSH)SelectObject(m_backbufferDC, myBrush);

	hpen = CreatePen(PS_SOLID, 2, RGB(0, 216, 255));
	hpenOld = (HPEN)::SelectObject(m_backbufferDC, (HGDIOBJ)hpen);

	Rectangle(m_backbufferDC, rc2.left, rc2.top, rc2.right, rc2.bottom);

	hpen = (HPEN)::SelectObject(m_backbufferDC, hpenOld);
	DeleteObject(hpen);

	SelectObject(m_backbufferDC, oldBrush);
	DeleteObject(myBrush);
}

void GameManager::StatWindow()
{
	TCHAR str[128];
	RECT m_TextRct = { 70,40,100,60 };
	
	StatBox();

	if (m_eGameState != GAME_PLAY)
		m_Font.Text(70, 40, L"1P-", 0x00ffffff);
	else
	{
		if (m_fTextTime > 0.2f)
		{
			m_Font.Text(70, 40, L"1P-", 0x00ffffff);
		}
		if (m_fTextTime > 0.4f)
		{
			PatBlt(m_backbufferDC, m_TextRct.left, m_TextRct.top, m_TextRct.right, m_TextRct.bottom, BLACKNESS);
			m_fTextTime = 0.0f;
		}
	}


	wsprintf(str, TEXT("-%d"), m_stat.Bonus);
	m_Font.Text(270, 60, str, 0x00ffffff);

	wsprintf(str, TEXT("STAGE-%02d"), m_stat.Stage);
	m_Font.Text(350, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("HI-%06d"), m_stat.Hi);
	m_Font.Text(210, 40, str, 0x00ffffff);

	wsprintf(str, TEXT("%06d"), m_stat.Score);
	m_Font.Text(105, 40, str, 0x00ffffff);

	m_Font.Text(210, 60, L"BONUS", RGB(255, 0, 127));

	int iconX = 430;

	for (int i = 0; i < m_Player.GetLife(); i++)
	{
		if (i == 0)
			BitMapManager::GetSingleton()->GetImg(ICON_LIFE)->Draw(m_backbufferDC, iconX,60, 1, 1);
		else
		{
			iconX -= BitMapManager::GetSingleton()->GetImg(ICON_LIFE)->GetSize().cx;
			BitMapManager::GetSingleton()->GetImg(ICON_LIFE)->Draw(m_backbufferDC, iconX, 60, 1, 1);
		}
	}
}


//Release
void GameManager::Release()
{
	m_Backgrd.Release();
	m_Enemy.Release();

	for (vector<Star*>::iterator it = m_Star.begin(); it != m_Star.end(); it++)

	{
		delete (*it);
	}
	m_Star.clear();

	BitMapManager::GetSingleton()->Release();
	BitMapManager::Release();
}

void GameManager::ObejectClear()
{
	m_Backgrd.Release();
	m_Enemy.Release();
}

GameManager::~GameManager()
{
}

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
}


void GameManager::StateReset()
{
	m_stat.Bonus = 5000;
	m_stat.Score = 0;
	m_stat.Stage = 1;
	m_stat.Hi = 20000;


	m_Backgrd.Init(0, 100);
	m_Player.Init();
}

//Update
void GameManager::Update()
{
	if (m_eGameState == GAME_MAIN || m_eGameState == GAME_INIT)
		MainUpdate();
	else if (m_eGameState == GAME_START)
		Stage();
	else if (m_eGameState == GAME_PLAY)
	{
		StatUpdate();

		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			if (m_Player.GetPlayX() < m_Backgrd.GetMitterPos(0))
			{
				return;
			}
		}
		if (m_Player.GetPlayX() > m_Backgrd.GetMitterPos(9))
		{
			m_Backgrd.Update();
			EndLine();
			Render();
			return;
		}
		m_Backgrd.Update();
		m_Player.Update(NOTEND);
		Render();
	}
	else if (m_eGameState == GAME_END)
	{
		m_Backgrd.EndMotion();
		m_Backgrd.Update();
		m_Player.Update(ENDPOS);
		StatWindow();
		EndScore();
	}
	Render();
}

void GameManager::MainUpdate()
{
	if (m_eGameState == GAME_INIT)
	{
		Release();
		PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS); // 지우기
	}
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
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
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	int tmp = m_stat.Bonus;
	if (tmp != 0)
	{
		tmp -= 10;
		m_stat.Score += 10;
	}
	if (m_stat.Bonus != 0)
		m_stat.Bonus -= 10;

	if (m_stat.Bonus == 0)
	{
		if (m_fDeltaTime > 15.0f)
		{
			m_eGameState = GAME_INIT;
			m_dwLastTime = m_dwCurTime;
		}
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

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	RECT SelectRct = { m_ClientRct.right * 0.3, m_ClientRct.bottom * 0.68, m_ClientRct.right * 0.7, m_ClientRct.bottom * 0.75 };

	BitMapManager::GetSingleton()->GetImg(ICON_TITLE)->
		Draw(m_backbufferDC, m_ClientRct.right*0.27, m_ClientRct.bottom*0.23, 1);


	m_Font.Text(m_ClientRct.right*0.4, m_ClientRct.bottom*0.6, L"PLAY SELECT", 0x00ffffff);

	if (m_eStartCheck == GAME_WAIT)
	{
		if (m_fDeltaTime > 0.2f)
		{
			PatBlt(m_backbufferDC, SelectRct.left, SelectRct.top, SelectRct.right, SelectRct.bottom, BLACKNESS);
		}
		if (m_fDeltaTime > 0.4f)
		{
			m_Font.Text(m_ClientRct.right*0.35, m_ClientRct.bottom*0.7, L"☞ 1 PLAYER A", 0x00ffffff);
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
		m_Font.Text(m_ClientRct.right*0.35, m_ClientRct.bottom*0.7, L"☞ 1 PLAYER A", 0x00ffffff);

}

void GameManager::Stage()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS); // 지우기

	StatWindow();

	TCHAR str[128];
	wsprintf(str, TEXT("STAGE-%02d"), m_stat.Stage);
	m_Font.Text(m_ClientRct.right*0.4, m_ClientRct.bottom*0.5, str, 0x00ffffff);

	if (m_fDeltaTime > 3.0f)
	{
		PatBlt(m_backbufferDC, 0, 0, m_ClientRct.right, m_ClientRct.bottom, BLACKNESS); //지우기
		m_eGameState = GAME_PLAY;
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
}

GameManager::~GameManager()
{
}

#include "Player.h"



Player::Player()
{
	m_fpositionX = 100.0f;
	m_fpositionY = 330.0f;

	m_eState = IDLE;
	m_eDirection = DIRECTION_NONE;

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fMotionTime = 0.0f;
}

void Player::Render(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	HDC hdc2 = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	BitMapManager::GetSingleton()->GetPlayer(m_ePlayImg).Draw(hdc2, m_fpositionX, m_fpositionY);
	BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc);
	ReleaseDC(hWnd, hdc);
}

void Player::Move()
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (m_eState == IDLE)
			m_eState = MOVE;

			m_eDirection = DIRECTION_LEFT;
			m_fpositionX -= 150 * m_fDeltaTime;

	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{	
		if (m_eState == IDLE)
			m_eState = MOVE;

			m_eDirection = DIRECTION_RIGHT;
			m_fpositionX += 150 * m_fDeltaTime;

	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (m_eState == IDLE || m_eState == MOVE)
		{
			// Å° ´Ù¿î
			m_eState = JUMP;
			m_fJumpY = m_fpositionY;
			m_fJumpX = m_fpositionX;
		}
	}

}

void Player::Motion()
{
	m_fMotionTime += m_fDeltaTime;
	if (m_eState == IDLE)
	{
		m_ePlayImg = PLAYER_MOVE0;
	}
	else	if (m_eState == MOVE)
	{
		if (m_fMotionTime >= 0.1f)
		{
			switch (m_ePlayImg)
			{
			case PLAYER_MOVE0:
				m_ePlayImg = PLAYER_MOVE1;
				break;
			case PLAYER_MOVE1:
				m_ePlayImg = PLAYER_MOVE2;
				break;
			case PLAYER_MOVE2:
				m_ePlayImg = PLAYER_MOVE0;
				break;
			case PLAYER_WIN0:
				m_ePlayImg = PLAYER_WIN1;
				break;
			case PLAYER_WIN1:
				m_ePlayImg = PLAYER_WIN0;
				break;
			case PLAYER_DIE:
				m_ePlayImg = PLAYER_DIE;
				break;
			default:
				break;
			}
			m_eState = IDLE;
			m_eDirection = DIRECTION_NONE;
			m_fMotionTime = 0.0f;
		}
	}
	else if (m_eState == JUMP)
	{
		m_ePlayImg = PLAYER_MOVE2;
	}

}
void Player::Update(HWND hWnd)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	Move();


	if (m_eState == JUMP)
	{
		m_fCurJumpTime += m_fDeltaTime;

		if (m_eDirection == DIRECTION_LEFT)
			m_fpositionX -= 150 * m_fDeltaTime;
		else if (m_eDirection == DIRECTION_RIGHT)
			m_fpositionX += 150 * m_fDeltaTime;

		 if(m_eDirection == DIRECTION_NONE)
			 m_fpositionY = m_fJumpY - sinf(m_fCurJumpTime * PI * 1) * 80;
		 else
			m_fpositionY = m_fJumpY - sinf(m_fCurJumpTime * PI) * 80;

		if (m_fCurJumpTime > 1.0f)
		{
			m_fCurJumpTime = 0.0f;
			m_fpositionY = 330.0f;
			m_eState = IDLE;
			m_eDirection = DIRECTION_NONE;
		}
	}

	Motion();

	Render(hWnd);
}


Player::~Player()
{
}

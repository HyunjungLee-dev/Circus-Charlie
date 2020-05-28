#include "Player.h"



Player::Player()
{
}

void Player::Init()
{
	m_Pos.m_fX = 100.0f;
	m_Pos.m_fY = 335.0f;

	m_eState = IDLE;
	m_eDirection = DIRECTION_NONE;
	m_iLife = 4;

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fMotionTime = 0.0f;
}

void Player::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	BitMapManager::GetSingleton()->GetPlayer(m_ePlayImg).Draw(hdc, m_Pos.m_fX, m_Pos.m_fY, 1, 1);
}

void Player::Move(END state)
{
	if (state == ENDPOS)
		return;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (state == ENDLINE)
		{
			m_Pos.m_fX -= 100 * m_fDeltaTime;
		}
		if (m_eState == IDLE )
			m_eState = MOVE;

			m_eDirection = DIRECTION_LEFT;

			

	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{	
		if (state == ENDLINE)
		{
			m_Pos.m_fX += 100 * m_fDeltaTime;
		}
		if (m_eState == IDLE)
			m_eState = MOVE;

			m_eDirection = DIRECTION_RIGHT;
	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (m_eState == IDLE || m_eState == MOVE)
		{
			m_eState = JUMP;
			m_fJumpX = m_Pos.m_fX;
			m_fJumpY = m_Pos.m_fY;
		}
	}

	UpdateRct();
}

void Player::Motion()
{
	m_fMotionTime += m_fDeltaTime;
	if (m_eState == IDLE)
	{
		m_ePlayImg = PLAYER_MOVE0;
	}
	else if (m_eState == P_END)
	{
		if (m_fMotionTime >= 0.09f)
		{
			switch (m_ePlayImg)
			{
			case PLAYER_WIN0:
				m_ePlayImg = PLAYER_WIN1;
				break;
			case PLAYER_WIN1:
				m_ePlayImg = PLAYER_WIN0;
				break;
			}
			m_fMotionTime = 0.0f;
		}
	}
	else if (m_eState == MOVE)
	{
		if (m_fMotionTime >= 0.08f)
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

void Player::UpdateRct()
{
	m_playerRect.left = m_Pos.m_fX + 20;
	m_playerRect.top = m_Pos.m_fY;
	m_playerRect.right = m_playerRect.left + BitMapManager::GetSingleton()->GetPlayer(PLAYER_MOVE0).GetSize().cx - 40;
	m_playerRect.bottom = m_playerRect.top + BitMapManager::GetSingleton()->GetPlayer(PLAYER_MOVE0).GetSize().cy - 5;
}


void Player::Update(END state)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	Move(state);


	if (m_eState == JUMP)
	{
		m_fCurJumpTime += m_fDeltaTime;

		if (state == ENDLINE)

		{
			if (m_eDirection == DIRECTION_LEFT)
				m_Pos.m_fX -= 100 * m_fDeltaTime;
			else if (m_eDirection == DIRECTION_RIGHT)
				m_Pos.m_fX += 100 * m_fDeltaTime;
		}




		if (m_fCurJumpTime > 1.0f)
		{
			m_eState = IDLE;
			m_eDirection = DIRECTION_NONE;
			m_fCurJumpTime = 0.0f;
			if (state == ENDPOS)
			{
				m_Pos.m_fX = 350.0f;
				m_Pos.m_fY = 290.0f;
				m_eState = P_END;
				m_ePlayImg = PLAYER_WIN0;
				Motion();
				return;
			}
			else
				m_Pos.m_fY = 330.0f;
		}

		if (state != ENDPOS)
		{
			if (m_eDirection == DIRECTION_NONE)
				m_Pos.m_fY = m_fJumpY - sinf(m_fCurJumpTime * PI * 1) * 110;
			else
				m_Pos.m_fY = m_fJumpY - sinf(m_fCurJumpTime * PI) * 110;
		}

		UpdateRct();
	}
	Motion();
}


Player::~Player()
{
}

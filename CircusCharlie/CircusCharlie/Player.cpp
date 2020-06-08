#include "Player.h"



Player::Player()
{
}

//Init
void Player::Init()
{
	m_Pos.m_fX = 100.0f;
	m_Pos.m_fY = 345.0f;

	m_iLife = 4;
	m_ePlayImg = PLAYER_MOVE0;

	m_eState = IDLE;
	m_eDirection = DIRECTION_NONE;


	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fCurJumpTime = 0.0f;
	m_fMotionTime = 0.0f;

	isJumping = false;

}

//Update
void Player::Update(LINE state)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	m_eEnd = state;

	Move();
	Motion();
	Render();
}

void Player::Move()
{
	if (m_eEnd == ENDPOS)
	{
		Jump();
		return;
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (m_eEnd == ENDLINE)
		{
			m_Pos.m_fX -= 100 * m_fDeltaTime;
		}
		if (m_eState == IDLE)
			m_eState = MOVE;


			m_eDirection = DIRECTION_LEFT;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		if (m_eEnd == ENDLINE)
		{
			m_Pos.m_fX += 100 * m_fDeltaTime;
		}
		if (m_eState == IDLE)
			m_eState = MOVE;

			m_eDirection = DIRECTION_LEFT;
	}


	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!isJumping)
		{
				isJumping = true;
				m_fJumpX = m_Pos.m_fX;
				m_fJumpY = m_Pos.m_fY;

		}
		else
		{
			if (m_Pos.m_fY < 345.0f)
			{
				Jump();
			}
		}

	}
	else
	{
		Jump();
	}

	UpdateRct();
}

void Player::Jump()	 
{
	if (isJumping)
	{
		m_fCurJumpTime += m_fDeltaTime;

		if (m_eEnd == ENDLINE)
		{
			if (m_eDirection == DIRECTION_LEFT)
				m_Pos.m_fX -= 20 * m_fDeltaTime;
			else if (m_eDirection == DIRECTION_RIGHT)
				m_Pos.m_fX += 20 * m_fDeltaTime;
		}

		if (m_eEnd != ENDPOS)
		{
			if (m_eDirection == DIRECTION_NONE)
				m_Pos.m_fY = m_fJumpY - sinf(m_fCurJumpTime * PI * 1) * 110;
			else
				m_Pos.m_fY = m_fJumpY - sinf(m_fCurJumpTime * PI) * 110;
		}

		if (m_fCurJumpTime > 1.0f)
		{
			m_eState = IDLE;
			isJumping = false;
			m_eDirection = DIRECTION_NONE;
			m_fCurJumpTime = 0.0f;
			if (m_eEnd == ENDPOS)
			{
				m_Pos.m_fX = 350.0f;
				m_Pos.m_fY = 300.0f;
				m_eState = P_END;
				m_ePlayImg = PLAYER_WIN0;
				return;
			}
			else
				m_Pos.m_fY = 345.0f;
		}

		UpdateRct();
	}
}

void Player::Motion()
{
	m_fMotionTime += m_fDeltaTime;
	if (m_eState == IDLE)
	{
		if(m_ePlayImg == PLAYER_DIE)
			m_ePlayImg = PLAYER_DIE;
		else
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
	if (isJumping && m_eEnd != ENDPOS)
	{
		if (m_Pos.m_fY < 345.0f)
		{
			if (m_ePlayImg == PLAYER_DIE)
				m_ePlayImg = PLAYER_DIE;
			else
				m_ePlayImg = PLAYER_MOVE2;
		}
	}

}

void Player::UpdateRct()
{
	m_playerRect.left = m_Pos.m_fX + 20;
	m_playerRect.top = m_Pos.m_fY;
	m_playerRect.right = m_playerRect.left + BitMapManager::GetSingleton()->GetImg(m_ePlayImg)->GetSize().cx - 40;
	m_playerRect.bottom = m_playerRect.top + BitMapManager::GetSingleton()->GetImg(m_ePlayImg)->GetSize().cy - 5;
}



//Render
void Player::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	BitMapManager::GetSingleton()->GetImg(m_ePlayImg)->Draw(hdc, m_Pos.m_fX, m_Pos.m_fY, 1, 1);
}

//Release
void Player::Release()
{
}

Player::~Player()
{
}

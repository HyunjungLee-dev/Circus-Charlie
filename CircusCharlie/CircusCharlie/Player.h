#pragma once
#include"BitMapManager.h"
class Player
{
private:
	
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	float	m_fCurJumpTime;
	float m_fMotionTime;

	float		m_fJumpX;
	float		m_fJumpY;


	float m_fpositionX, m_fpositionY;
	PLAYER_IMG m_ePlayImg;
	PLAYERSTATE m_eState;
	DIRECTION m_eDirection;
public:
	Player();
	~Player();

	void Move();
	void Motion();
	void Render();
	void Update(HWND hWnd);
};


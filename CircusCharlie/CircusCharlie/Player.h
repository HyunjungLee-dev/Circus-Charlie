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


	POS m_pos;
	RECT m_playerRect;
	PLAYER_IMG m_ePlayImg;
	PLAYERSTATE m_eState;
	DIRECTION m_eDirection;
public:
	Player();
	~Player();

	void Init();
	void Move();
	void Motion();
	void Render();
	void Update();
	RECT GetPlayerRct() { return m_playerRect; }
	PLAYERSTATE GetState() { return m_eState; }
	void SetPlayerMotion(PLAYER_IMG m) {  m_ePlayImg = m; }
};


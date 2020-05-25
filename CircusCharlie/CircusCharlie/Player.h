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

	int m_iLife;
	POS m_pos;
	POS m_ViewPos;
	RECT m_playerRect;
	PLAYER_IMG m_ePlayImg;
	PLAYERSTATE m_eState;
	DIRECTION m_eDirection;
public:
	Player();
	~Player();

	void Init();
	void Move(END state);
	void Motion();
	void Render();
	void Update(END state);

	RECT GetPlayerRct() { return m_playerRect; }
	PLAYERSTATE GetState() { return m_eState; }
	void SetPlayerMotion(PLAYER_IMG m) {  m_ePlayImg = m; }
	void SetLife() { m_iLife--; }
	int GetLife() {return m_iLife;}
	float GetPlayX() { return m_pos.m_fX; }
};


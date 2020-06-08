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
	bool isJumping;

	POS m_Pos;
	RECT m_playerRect;
	IMG m_ePlayImg;

	PLAYERSTATE m_eState;
	DIRECTION m_eDirection;
	LINE m_eEnd;
	int m_iLife;
public:
	Player();
	~Player();

	//Init
	void Init();

	//Update
	void Update(LINE state);
	void Move();
	void Jump();
	void Motion();
	void UpdateRct();

	//Render
	void Render();

	//Release
	void Release();

	//Getter
	int GetLife(){ return m_iLife; }
	float GetPlayX() { return m_Pos.m_fX; }
	RECT GetPlayerRct() { return m_playerRect; }
	PLAYERSTATE GetState() { return m_eState; }
	bool GetMove() {
		
		if (isJumping &&  m_eEnd != ENDPOS)
			return true;
		else
			return false;
	}

	//Setter
	void SetLife() { m_iLife--; }
	void SetPlayerMotion(IMG m) { m_ePlayImg = m; }
};


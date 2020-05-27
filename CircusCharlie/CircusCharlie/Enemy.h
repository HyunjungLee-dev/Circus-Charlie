#pragma once
#include"BitMapManager.h"

struct  Fire
{
	POS pos;
	FIRE_IMG type;
	ITEM Item;
	RECT FireRect;
};

class Enemy
{
private:
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	list<Fire*> Ring;
	list<Fire*> Jar;
public:
	Enemy();
	~Enemy();

	void Init();
	void SetJar();
	void SetRing();

	void Update(END end);
	void UpdateRectPos();
	void Motion(Fire* f);
	void backRing();
	bool Collision(RECT rect);
	void backEnemy(float back);
	void ResetItem();
	PASS PassCheck(float x);
	PASS RingPassCheck(float x);

	void Render();
	void HalfRender();

	void Clear(list<Fire*> list);
	void Release();
};


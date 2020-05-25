#pragma once
#include"BitMapManager.h"

struct  Fire
{
	POS pos;
	FIRE_IMG type;
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

	void Update();
	void UpdateRectPos();
	void Motion(Fire* f);
	bool Collision(RECT rect);

	void Render();
	void HalfRender();

	void Clear(list<Fire*> list);
	void Release();
};


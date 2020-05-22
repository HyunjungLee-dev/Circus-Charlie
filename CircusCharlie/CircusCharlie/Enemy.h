#pragma once
#include"BitMapManager.h"

struct  Fire
{
	POINT pos;
	FIRE_IMG type;
	RECT FireRect;
};

class Enemy
{
private:
	float m_fStartTime;
	float m_fNowTime;

	list<Fire*> Ring;
	list<Fire*> Jar;
public:
	Enemy();
	~Enemy();

	void Init();
	void SetJar();
	void SetRing();
	void Update();
	void Motion(Fire* f);
	void Render();
	void Clear(list<Fire*> list);
	void Release();
};


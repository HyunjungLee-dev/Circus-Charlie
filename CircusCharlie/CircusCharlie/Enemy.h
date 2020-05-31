#pragma once
#include"BitMapManager.h"

class Enemy
{
private:
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	list<Fire*> m_Ringlist;
	vector<Fire*> m_Jar;

	HDC m_backbufferDC;
	bool m_bPassCheck;
	LINE m_eLine;
public:
	Enemy();
	~Enemy();

	//Init
	void Init();
	void SetJar();
	void SetRing();

	//Update
	void Update(LINE line);
	void UpdateRectPos();
	void Move();
	void Motion(Fire* f);

	bool Collision(RECT rect);
	void RestoreEnemy(float back);

	PASS JarPassCheck(float x);
	PASS RingPassCheck(float x);

	//Render
	void Render();
	void HalfRender();

	//Release
	void Release();
	void RingClear();
	void JarClear();


	//Getter

};


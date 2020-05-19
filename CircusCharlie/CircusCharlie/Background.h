#pragma once
#include"BitMapManager.h"


class Background
{
private:
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	float m_fEndpoint;
	float m_fStartpoint;

	vector<POINT*> Field;
	vector<POINT*> Audience;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void Clear(vector<POINT*> v);
	void Release();
};


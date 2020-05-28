#pragma once
#include"BitMapManager.h"

class Background
{
private:
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	list<POS*> Field;
	list<POS*> Audience;
	vector<POS*> Mitter;
	vector<POS*> _Distance;
	int NowDistance;
	float backlength;

	POS podium;
	RECT Rctpodium;

	BACKGROUND_IMG m_eENDImg;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void UpdateRectPos();
	void backBgd(float pos);
	float GetMitterPos(int index);
	float GetBacklength() { return backlength; }
	void CheckDistacne(float pos);
	bool EndCheck(RECT rect);
	void EndMotion();
	void Release();
};


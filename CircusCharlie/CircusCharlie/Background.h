#pragma once
#include"Font.h"
class Background
{
private:
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	list<BACKGROUND*> m_Audiencelist;
	list<BACKGROUND*> m_Fieldlist;
	vector<MITTER*> m_Mitter;
	PODIUM m_Podium;

	DIRECTION m_eDirection;
	LINE m_eline;

	int NowDistance;
	float backlength;

	HDC m_backbufferDC;
	Font m_Font;
public:
	Background();
	~Background();

	//Init
	void Init(int x, int y);

	//Update
	void Update(LINE line, bool state);
	void Move(bool state);
	void LeftMove();
	void RightMove();

	bool PodiumCheck(RECT rect);
	void PodiumRctUpdate();

	void RestoreBckgrd(float Playerpos);
	void CheckDistacne(float Playerpos);

	void EndMotion();
	//Render
	void Render();

	//Release
	void Release();


	//Getter
	float GetMitterPos(int index);
	float GetBacklength() { return backlength; }
};


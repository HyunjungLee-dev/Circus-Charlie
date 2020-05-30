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

	HDC m_backbufferDC;
	Font m_Font;
public:
	Background();
	~Background();

	//Init
	void Init(int x, int y);

	//Update
	void Update();
	void Move();

	bool PodiumCheck(RECT rect);
	void PodiumRctUpdate();

	void EndMotion();
	//Render
	void Render();

	//Release
	void Release();
	void Clear(vector<MITTER*> v);
	void Clear(list<BACKGROUND*> list);

	//Getter
	float GetMitterPos(int index);
};


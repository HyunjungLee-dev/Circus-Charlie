#pragma once
#include"BitMapManager.h"


class Background
{
private:
	//vector<POINT*> Field;
	//vector<POINT*> Audience;

	list<POINT*> Field;
	list<POINT*> Audience;


	//float m_fEndPoint;
	//bool m_bStart;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void Clear(list<POINT*> v);
	void Release();
};


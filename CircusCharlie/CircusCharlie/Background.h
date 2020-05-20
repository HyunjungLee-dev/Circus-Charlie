#pragma once
#include"BitMapManager.h"


class Background
{
private:
	list<POINT*> Field;
	list<POINT*> Audience;
	list<POINT*> Mitter;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void Clear(list<POINT*> v);
	void Release();
};


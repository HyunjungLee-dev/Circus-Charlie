#pragma once
#include"BitMapManager.h"

class Background
{
private:
	list<POS*> Field;
	list<POS*> Audience;
	list<POS*> Mitter;
	list<POS*> _Distance;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void Clear(list<POS*> v);
	void Release();
};


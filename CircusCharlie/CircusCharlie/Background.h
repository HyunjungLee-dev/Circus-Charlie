#pragma once
#include"BitMapManager.h"

class Background
{
private:
	list<POS*> Field;
	list<POS*> Audience;
	vector<POS*> Mitter;
	list<POS*> _Distance;

	POS podium;
	RECT Rctpodium;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void backBgd();
	float GetMitterPos(int index);
	bool Collision();
	void Clear(list<POS*> lst);
	void Clear(vector<POS*> v);
	void Release();
};


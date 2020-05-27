#pragma once
#include"BitMapManager.h"

class Background
{
private:
	list<POS*> Field;
	list<POS*> Audience;
	vector<POS*> Mitter;
	vector<POS*> _Distance;
	int NowDistance;
	float backlength;

	POS podium;
	RECT Rctpodium;
public:
	Background();
	~Background();

	void Init(int x, int y);
	void Update();
	void Render();
	void backBgd(float pos);
	float GetMitterPos(int index);
	float GetBacklength() { return backlength; }
	void CheckDistacne(float pos);
	void Clear(list<POS*> lst);
	void Clear(vector<POS*> v);
	void Release();
};


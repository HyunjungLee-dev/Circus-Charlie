#pragma once
#include"BitMapManager.h"
class Font
{
private:
	HDC m_Fonthdc;
	HFONT hFont, OldFont;
public:
	void Init();
	void Text(int x, int y, LPCWSTR text, COLORREF color);
	Font();
	~Font();
};


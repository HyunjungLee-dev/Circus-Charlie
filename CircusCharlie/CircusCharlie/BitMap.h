#pragma once
#include "Mecro.h"

class BitMap
{
private:
	HDC     MemDC;
	HBITMAP m_BitMap;
	HBITMAP m_OldBitMap;
	SIZE    m_size;
public:
	void Init(HDC hdc);
	void ImgLoad(HDC hdc, LPCWSTR FileName);
	void Draw(HDC hdc, int nX, int nY, float sX, float sY);
	void Draw(HDC hdc, int nX, int nY, float stretch);
	void Draw(HDC hdc);

	inline SIZE GetSize() { return m_size; }
	HDC GetMemDC() { return MemDC; }
	void Release();
	BitMap();
	~BitMap();
};
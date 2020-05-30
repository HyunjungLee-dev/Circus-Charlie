#include "BitMap.h"

BitMap::BitMap()
{
}

void BitMap::Init(HDC hdc)
{
	MemDC = CreateCompatibleDC(hdc);
	m_BitMap = CreateCompatibleBitmap(hdc, 512, 448);
	m_OldBitMap = (HBITMAP)SelectObject(MemDC, m_BitMap);

	BITMAP bit;
	GetObject(m_BitMap, sizeof(BITMAP), &bit);
	m_size.cx = bit.bmWidth;
	m_size.cy = bit.bmHeight;
}

void BitMap::ImgLoad(HDC hdc, LPCWSTR FileName)
{
	MemDC = CreateCompatibleDC(hdc);
	m_BitMap = (HBITMAP)LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_OldBitMap = (HBITMAP)SelectObject(MemDC, m_BitMap);

	BITMAP bit;
	GetObject(m_BitMap, sizeof(BITMAP), &bit);
	m_size.cx = bit.bmWidth;
	m_size.cy = bit.bmHeight;
}

void BitMap::Draw(HDC hdc, int nX, int nY, float sX, float sY) // 색상 제거
{
	TransparentBlt(hdc, nX, nY, m_size.cx*sX, m_size.cy*sY,
		MemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}

void BitMap::Draw(HDC hdc, int nX, int nY, float stretch) //확대
{
	StretchBlt(hdc, nX, nY, m_size.cx*stretch, m_size.cy*stretch, MemDC, 0, 0, m_size.cx, m_size.cy, SRCCOPY);
}

void BitMap::Draw(HDC hdc) //전체
{
	BitBlt(hdc, 0, 0, m_size.cx, m_size.cy, MemDC, 0, 0, SRCCOPY);
}

void BitMap::Release()
{
	SelectObject(MemDC, m_OldBitMap);
	DeleteObject(m_BitMap);
	DeleteDC(MemDC);
}


BitMap::~BitMap()
{
}
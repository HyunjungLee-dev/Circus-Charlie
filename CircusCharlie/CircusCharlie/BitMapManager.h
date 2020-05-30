#pragma once
#include"BitMap.h"
#include"Singleton.h"


class BitMapManager : public Singleton< BitMapManager>
{
private:
	BitMap m_BackBuffer;
	vector<BitMap*> Imglist;
	HDC BufferDC;

public:
	void Init(HWND hWnd);
	void ImgLoad();
	void PlayerImgLoad();
	void FireImgLoad();
	void BkgImgLoad();
	void IconImgLoad();
	void Clear();

	HDC GetBufferDC() { return BufferDC; }
	BitMap GetBackBuffer() { return m_BackBuffer; }
	BitMap* GetImg(IMG index) { return Imglist.at(index); }
	BitMapManager();
	~BitMapManager();
};
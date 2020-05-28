#pragma once
#include"BitMap.h"
#include"Singleton.h"


class BitMapManager : public Singleton< BitMapManager>
{
private:
	BitMap m_pPlayer[PLAYER_END];
	BitMap m_pFire[FIRE_END];
	BitMap m_pBackground[BACK_END];
	BitMap m_pIcon[ICON_END];
	BitMap m_BackBuffer;

public:
	void Init(HWND hWnd);
	void PlayerImgLoad();
	void FireImgLoad();
	void BackImgLoad();
	void IconImgLoad();
	void BitRelease();

	BitMap GetBackBuffer() { return m_BackBuffer; }
	BitMap  GetBackgroud(BACKGROUND_IMG index) { return m_pBackground[index]; }
	BitMap  GetPlayer(PLAYER_IMG index) { return m_pPlayer[index]; }
	BitMap GetFire(FIRE_IMG index) { return m_pFire[index]; }
	BitMap GetIcon(ICON_IMG index) { return m_pIcon[index]; }

	BitMapManager();
	~BitMapManager();
};
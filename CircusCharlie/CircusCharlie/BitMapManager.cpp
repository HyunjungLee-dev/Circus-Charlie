#include "BitMapManager.h"

BitMapManager* BitMapManager::_Singleton = NULL;

BitMapManager::BitMapManager()
{
}

void BitMapManager::Init(HWND hWnd)
{

	HDC hdc = GetDC(hWnd);
	m_BackBuffer.Init(hdc);
	PlayerImgLoad();
	FireImgLoad();
	BackImgLoad();
	IconImgLoad();
	ReleaseDC(hWnd, hdc);
}

void BitMapManager::PlayerImgLoad() 
{
	m_pPlayer[PLAYER_MOVE0].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//player0.bmp");
	m_pPlayer[PLAYER_MOVE1].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//player1.bmp");
	m_pPlayer[PLAYER_MOVE2].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//player2.bmp");
	m_pPlayer[PLAYER_DIE].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//die.bmp");
	m_pPlayer[PLAYER_WIN0].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//win0.bmp");
	m_pPlayer[PLAYER_WIN1].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//win1.bmp");
}

void BitMapManager::FireImgLoad()
{
	m_pFire[FIRE_NOMAL].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//enemy.bmp");
	m_pFire[FIRE_NOMAL_B].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//enemy1.bmp");
	m_pFire[FIRE_HALF_LEFT].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//enemy_b.bmp");
	m_pFire[FIRE_HALF_LEFTB].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//enemy_1b.bmp");
	m_pFire[FIRE_HALF_RIGHT].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//enemy_f.bmp");
	m_pFire[FIRE_HALF_RIGHTB].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//enemy_1f.bmp");
	m_pFire[FIRE_ITEM].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//cash.bmp");
	m_pFire[FIRE_JAR].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//front.bmp");
	m_pFire[FIRE_JARB].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//front2.bmp");
}

void BitMapManager::BackImgLoad()
{
	m_pBackground[BACK_WAY].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//back.bmp");
	m_pBackground[BACK_DECO].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//back_deco.bmp");
	m_pBackground[BACK_NOMAL0].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//back_normal.bmp");
	m_pBackground[BACK_NOMAL1].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//back_normal2.bmp");
	m_pBackground[BACK_PODIUM].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//end.bmp");
	m_pBackground[BACK_MITER].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//miter.bmp");
}

void BitMapManager::IconImgLoad()
{
	m_pIcon[ICON_LIFE].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//icon.bmp");
	m_pIcon[ICON_STAR_B].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//star.bmp");
	m_pIcon[ICON_STAR_Y].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//star2.bmp");
	m_pIcon[ICON_STAR_R].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//star1.bmp");
	m_pIcon[ICON_TITLE].ImgLoad(m_BackBuffer.GetMemDC(), L"Res//title.bmp");
}

void BitMapManager::BitRelease()
{
	m_BackBuffer.Release();
}


BitMapManager::~BitMapManager()
{
}

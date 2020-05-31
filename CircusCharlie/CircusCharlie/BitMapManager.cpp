#include "BitmapManager.h"

BitMapManager* BitMapManager::_Singleton = NULL;

BitMapManager::BitMapManager()
{
}

void BitMapManager::Init(HWND hWnd)
{

	HDC hdc = GetDC(hWnd);
	m_BackBuffer.Init(hdc);
	BufferDC = m_BackBuffer.GetMemDC();
	ImgLoad();
	ReleaseDC(hWnd, hdc);
}

void BitMapManager::ImgLoad()
{
	for (int i = IMG_START; i < IMG_END; i++)
	{
		Imglist.push_back(new BitMap);
	}
	PlayerImgLoad();
	FireImgLoad();
	BkgImgLoad();
	IconImgLoad();

}

void BitMapManager::PlayerImgLoad()
{

	Imglist.at(PLAYER_MOVE0)->ImgLoad(BufferDC, L"Res//player0.bmp");
	Imglist.at(PLAYER_MOVE1)->ImgLoad(BufferDC, L"Res//player1.bmp");
	Imglist.at(PLAYER_MOVE2)->ImgLoad(BufferDC, L"Res//player2.bmp");
	Imglist.at(PLAYER_DIE)->ImgLoad(BufferDC, L"Res//die.bmp");
	Imglist.at(PLAYER_WIN0)->ImgLoad(BufferDC, L"Res//win0.bmp");
	Imglist.at(PLAYER_WIN1)->ImgLoad(BufferDC, L"Res//win1.bmp");

}

void BitMapManager::FireImgLoad()
{
	Imglist.at(FIRE_NOMAL)->ImgLoad(BufferDC, L"Res//enemy.bmp");
	Imglist.at(FIRE_NOMAL_B)->ImgLoad(BufferDC, L"Res//enemy1.bmp");
	Imglist.at(FIRE_HALF_LEFT)->ImgLoad(BufferDC, L"Res//enemy_b.bmp");
	Imglist.at(FIRE_HALF_LEFTB)->ImgLoad(BufferDC, L"Res//enemy_1b.bmp");
	Imglist.at(FIRE_HALF_RIGHT)->ImgLoad(BufferDC, L"Res//enemy_f.bmp");
	Imglist.at(FIRE_HALF_RIGHTB)->ImgLoad(BufferDC, L"Res//enemy_1f.bmp");
	Imglist.at(FIRE_ITEM)->ImgLoad(BufferDC, L"Res//cash.bmp");
	Imglist.at(FIRE_JAR)->ImgLoad(BufferDC, L"Res//front.bmp");
	Imglist.at(FIRE_JARB)->ImgLoad(BufferDC, L"Res//front2.bmp");
}

void BitMapManager::BkgImgLoad()
{
	Imglist.at(BACK_WAY)->ImgLoad(BufferDC, L"Res//back.bmp");
	Imglist.at(BACK_DECO)->ImgLoad(BufferDC, L"Res//back_deco.bmp");
	Imglist.at(BACK_NOMAL0)->ImgLoad(BufferDC, L"Res//back_normal.bmp");
	Imglist.at(BACK_NOMAL1)->ImgLoad(BufferDC, L"Res//back_normal2.bmp");
	Imglist.at(BACK_PODIUM)->ImgLoad(BufferDC, L"Res//end.bmp");
	Imglist.at(BACK_MITER)->ImgLoad(BufferDC, L"Res//miter.bmp");
}

void BitMapManager::IconImgLoad()
{
	Imglist.at(ICON_LIFE)->ImgLoad(BufferDC, L"Res//icon.bmp");
	Imglist.at(ICON_STAR_B)->ImgLoad(BufferDC, L"Res//star.bmp");
	Imglist.at(ICON_STAR_Y)->ImgLoad(BufferDC, L"Res//star2.bmp");
	Imglist.at(ICON_STAR_R)->ImgLoad(BufferDC, L"Res//star1.bmp");
	Imglist.at(ICON_TITLE)->ImgLoad(BufferDC, L"Res//title.bmp");
}

void BitMapManager::Clear()
{
	for (vector<BitMap*>::iterator it = Imglist.begin(); it != Imglist.end(); it++)
	{
		(*it)->Release();
		delete (*it);

	}
	Imglist.clear();
}

BitMapManager::~BitMapManager()
{
}

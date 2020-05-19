#include "Background.h"



Background::Background()
{
}

void Background::Init(int x, int y)
{
	for (int i = START_POINT; i < AUDIENCE_END; i++)
	{
		Audience.push_back(new POINT);
		Audience.back()->x = x + i * BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;
		Audience.back()->y = y;
	}

	for (int i = START_POINT; i < FIELD_END; i++)
	{
		Field.push_back(new POINT);
		Field.back()->x = x + i * BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).GetSize().cx;
		Field.back()->y = y+ BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cy*1.3;
	}

}

void Background::Update()
{
	int imgSizeX = BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		for (list<POINT*>::iterator  iter = Audience.begin(); iter != Audience.end(); iter++)
		{
			(*iter)->x += 1;
			if ((*iter)->x + imgSizeX > 512)
			{
				int x = Audience.front()->x;
				int y = Audience.front()->y;
				if (x > 0)
				{
					Audience.push_front(new POINT);
					Audience.front()->x = x - imgSizeX;
					Audience.front()->y = y;
				}

			}
		}
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		for (list<POINT*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
		{
			(*iter)->x -= 1;
			if ((*iter)->x + imgSizeX < 0)
			{
				int x = Audience.back()->x;
				int y = Audience.back()->y;
				if (x + imgSizeX < 512)
				{
					Audience.push_back(new POINT);
					Audience.back()->x = x + imgSizeX;
					Audience.back()->y = y;
				}

			}
		}
	}
	Render();
}

void Background::Render()	// 왼쪽으로 갈때 DECO 수정, 속도 체크 필요
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	int AudienceNum = 0;
	
	for (list<POINT*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
	{
		if (AudienceNum == DECO_POINT)
			BitMapManager::GetSingleton()->GetBackgroud(BACK_DECO).Draw(hdc, (*iter)->x, (*iter)->y, 1.3);
		else
			BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).Draw(hdc, (*iter)->x, (*iter)->y, 1.3);
		AudienceNum++;
		if (AudienceNum == AUDIENCE_END)
			AudienceNum = 0;
	}
	
	for (list<POINT*>::iterator iter = Field.begin(); iter != Field.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).Draw(hdc, (*iter)->x, (*iter)->y, 1.2);
	}
}

void Background::Clear(list<POINT*> v)
{
	for (list<POINT*>::iterator it = v.begin(); it != v.end(); it++)
	{
		delete (*it);

	}
	v.clear();
}

void Background::Release()
{
	Clear(Field);
	Clear(Audience);
}

Background::~Background()
{
	Release();
}

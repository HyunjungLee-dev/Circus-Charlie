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


	for (int i = 0; i < 11; i++)
	{
		int Distance = 600;

		Mitter.push_back(new POINT);
		_Distance.push_back(new POINT);
		if (i != 0)
			Mitter.back()->x = 20 + Distance*i;
		else
			Mitter.back()->x = 20;
		Mitter.back()->y = 405;
		_Distance.back()->x = Mitter.back()->x+30;
		_Distance.back()->y = Mitter.back()->y + 4;
	}

}

void Background::Update()
{
	int imgSizeX = BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		for (list<POINT*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
		{
			(*iter)->x += LENGTH;
		}

		for (list<POINT*>::iterator iter = Mitter.begin(); iter != Mitter.end(); iter++)
		{
			(*iter)->x += LENGTH;

		}

		for (list<POINT*>::iterator iter = _Distance.begin(); iter != _Distance.end(); iter++)
		{
			(*iter)->x += LENGTH;
		}
		
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		for (list<POINT*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
		{
			(*iter)->x -= LENGTH;
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

		for (list<POINT*>::iterator iter = Mitter.begin(); iter != Mitter.end(); iter++)
		{
			(*iter)->x -= LENGTH;
		}

		for (list<POINT*>::iterator iter = _Distance.begin(); iter != _Distance.end(); iter++)
		{
			(*iter)->x -= LENGTH;
		}
	}
}

void Background::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	TCHAR str[128];
	HFONT hFont, OldFont;
	hFont = CreateFont( 25,0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));
	OldFont = (HFONT)SelectObject(hdc, hFont);

	int AudienceNum = 0;
	int Distance = 100;
	
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

	for (list<POINT*>::iterator iter = Mitter.begin(); iter != Mitter.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetBackgroud(BACK_MITER).Draw(hdc, (*iter)->x, (*iter)->y, 1.2);
	}
	for (list<POINT*>::iterator iter = _Distance.begin(); iter != _Distance.end(); iter++)
	{
		if(Distance == 0)
			wsprintf(str, TEXT("00"));
		else
			wsprintf(str, TEXT("%d"), Distance);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, 0x00ffffff);
		TextOut(hdc, (*iter)->x , (*iter)->y , str, lstrlen(str));
		Distance -= 10;
	}
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
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
	Clear(Mitter);
}

Background::~Background()
{
	Release();
}

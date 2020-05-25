#include "Background.h"



Background::Background()
{
}

void Background::Init(int x, int y)
{

	for (int i = START_POINT; i < AUDIENCE_END; i++)
	{
		Audience.push_back(new POS);
		Audience.back()->m_fX = x + i * BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;
		Audience.back()->m_fY = y;
	}

	for (int i = START_POINT; i < FIELD_END; i++)
	{
		Field.push_back(new POS);
		Field.back()->m_fX= x + i * BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).GetSize().cx;
		Field.back()->m_fY = y+ BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cy*1.3;
	}


	for (int i = 0; i < 11; i++)
	{
		Mitter.push_back(new POS);
		_Distance.push_back(new POS);
		if (i != 0)
			Mitter.back()->m_fX = 20 + DISTANCE *i;
		else
			Mitter.back()->m_fX = 20;
		Mitter.back()->m_fY = 405;
		_Distance.back()->m_fX = Mitter.back()->m_fX+ 20;
		_Distance.back()->m_fY = Mitter.back()->m_fY + 8;
	}

	podium.m_fX = Mitter.back()->m_fX;
	podium.m_fY = 350;

}

void Background::Update()
{
	int imgSizeX = BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		for (list<POS*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
		{
			(*iter)->m_fX += LENGTH;
		}

		for (vector<POS*>::iterator iter = Mitter.begin(); iter != Mitter.end(); iter++)
		{
			(*iter)->m_fX += LENGTH;

		}

		for (list<POS*>::iterator iter = _Distance.begin(); iter != _Distance.end(); iter++)
		{
			(*iter)->m_fX += LENGTH;
		}
		podium.m_fX += LENGTH;
		
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		for (list<POS*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
		{
			(*iter)->m_fX -= LENGTH;
			if ((*iter)->m_fX + imgSizeX < 0)
			{
				int x = Audience.back()->m_fX;
				int y = Audience.back()->m_fY;
				if (x + imgSizeX < 512)
				{
					Audience.push_back(new POS);
					Audience.back()->m_fX = x + imgSizeX;
					Audience.back()->m_fY = y;
				}
			}
		}

		for (vector<POS*>::iterator iter = Mitter.begin(); iter != Mitter.end(); iter++)
		{
			(*iter)->m_fX -= LENGTH;
		}

		for (list<POS*>::iterator iter = _Distance.begin(); iter != _Distance.end(); iter++)
		{
			(*iter)->m_fX -= LENGTH;
		}

		podium.m_fX-= LENGTH;
	}
}

void Background::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();
	TCHAR str[128];
	HFONT hFont, OldFont;
	hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Terminal"));
	OldFont = (HFONT)SelectObject(hdc, hFont);

	int AudienceNum = 0;
	int Distance = 100;
	
	for (list<POS*>::iterator iter = Audience.begin(); iter != Audience.end(); iter++)
	{
		if (AudienceNum == DECO_POINT)
			BitMapManager::GetSingleton()->GetBackgroud(BACK_DECO).Draw(hdc, (*iter)->m_fX, (*iter)->m_fY, 1.3);
		else
			BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).Draw(hdc, (*iter)->m_fX, (*iter)->m_fY, 1.3);
		AudienceNum++;
		if (AudienceNum == AUDIENCE_END)
			AudienceNum = 0;
	}
	
	for (list<POS*>::iterator iter = Field.begin(); iter != Field.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).Draw(hdc, (*iter)->m_fX, (*iter)->m_fY, 1.2);
	}

	for (vector<POS*>::iterator iter = Mitter.begin(); iter != Mitter.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetBackgroud(BACK_MITER).Draw(hdc, (*iter)->m_fX, (*iter)->m_fY, 1);
	}
	for (list<POS*>::iterator iter = _Distance.begin(); iter != _Distance.end(); iter++)
	{
		if(Distance == 0)
			wsprintf(str, TEXT("00"));
		else
			wsprintf(str, TEXT("%d"), Distance);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, 0x00ffffff);
		TextOut(hdc, (*iter)->m_fX , (*iter)->m_fY , str, lstrlen(str));
		Distance -= 10;
	}
	BitMapManager::GetSingleton()->GetBackgroud(BACK_PODIUM).Draw(hdc, podium.m_fX, podium.m_fY, 1,1);
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}

float Background::GetMitterPos(int index)
{
	return Mitter[index]->m_fX + BitMapManager::GetSingleton()->GetBackgroud(BACK_MITER).GetSize().cx * 3;
}

void Background::Clear(list<POS*> lst)
{
	for (list<POS*>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		delete (*it);

	}
	lst.clear();
}

void Background::Clear(vector<POS*> v)
{
	for (vector<POS*>::iterator it = v.begin(); it != v.end(); it++)
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

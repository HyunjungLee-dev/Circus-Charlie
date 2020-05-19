#include "Background.h"



Background::Background()
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
}

void Background::Init(int x, int y)
{
	for (int i = START_POINT; i < AUDIENCE_END; i++)
	{
		Audience.push_back(new POINT);
		Audience[i]->x = x + i * BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;
		Audience[i]->y = y ;
	}

	for (int i = START_POINT; i < FIELD_END; i++)
	{
		Field.push_back(new POINT);
		Field[i]->x = x + i * BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).GetSize().cx;
		Field[i]->y = y+ BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cy*1.3;
	}
	m_fEndpoint = Field.back()->x;
}

void Background::Update()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	int sizetmp;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		for (vector<POINT*>::iterator  iter = Audience.begin(); iter < Audience.end(); iter++)
		{
			(*iter)->x += 1;
			if ((*iter)->x + BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25 > m_fEndpoint)
			{
				sizetmp = m_fEndpoint - (*iter)->x;
				if (sizetmp < 0)
					sizetmp = 0;
			}
			else
				sizetmp = BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;
		}
		for (int i = START_POINT; i < FIELD_END; i++)
		{
			Field[i]->x += 1;
			if (Field[i]->x + BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).GetSize().cx > m_fEndpoint)
			{
				sizetmp = m_fEndpoint - Field[i]->x;
				if (sizetmp < 0)
					sizetmp = 0;
			}
			else
				sizetmp = BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).GetSize().cx;
		}

	}
	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//{
	//	for (int i = START_POINT; i < AUDIENCE_END; i++)
	//	{
	//		Audience[i]->x -= 100 * m_fDeltaTime;
	//	/*	if (Audience[i]->x > Audience[AUDIENCE_END-1]->x + BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25)
	//			Audience[i]->x = Audience[i - 1]->x + BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).GetSize().cx*1.25;*/
	//	}
	//	for (int i = START_POINT; i < FIELD_END; i++)
	//	{
	//		Field[i]->x -= 100 * m_fDeltaTime;
	//	}
	//}
	Render();
}

void Background::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	for (int i = START_POINT; i < AUDIENCE_END; i++)
	{
		if(i == DECO_POINT)
			BitMapManager::GetSingleton()->GetBackgroud(BACK_DECO).Draw(hdc, Audience[i]->x, Audience[i]->y, 1.3);
		else
			BitMapManager::GetSingleton()->GetBackgroud(BACK_NOMAL0).Draw(hdc, Audience[i]->x, Audience[i]->y, 1.3);
	}

	for (int i = START_POINT; i < FIELD_END; i++)
	{
		BitMapManager::GetSingleton()->GetBackgroud(BACK_WAY).Draw(hdc, Field[i]->x, Field[i]->y,1.2);
	}

}

void Background::Clear(vector<POINT*> v)
{
	for (vector<POINT*>::iterator it = v.begin(); it != v.end(); it++)
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

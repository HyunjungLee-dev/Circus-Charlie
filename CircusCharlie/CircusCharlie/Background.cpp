#include "Background.h"



Background::Background()
{
}

//Init
void Background::Init(int x, int y)
{
	m_backbufferDC = BitMapManager::GetSingleton()->GetBufferDC();
	m_Font.Init();

	for (int i = START_POINT; i < AUDIENCE_END; i++)
	{
		m_Audiencelist.push_back(new BACKGROUND);
		m_Audiencelist.back()->pos.m_fX = x + i * BitMapManager::GetSingleton()->GetImg(BACK_NOMAL0)->GetSize().cx*1.25;
		m_Audiencelist.back()->pos.m_fY = y;
		m_Audiencelist.back()->m_eImg = BACK_NOMAL0;
	}

	for (int i = START_POINT; i < FIELD_END; i++)
	{
		m_Fieldlist.push_back(new BACKGROUND);
		m_Fieldlist.back()->pos.m_fX = x + i * BitMapManager::GetSingleton()->GetImg(BACK_WAY)->GetSize().cx;
		m_Fieldlist.back()->pos.m_fY = y + BitMapManager::GetSingleton()->GetImg(BACK_NOMAL0)->GetSize().cy*1.3;
		m_Fieldlist.back()->m_eImg = BACK_WAY;
	}

	int Distance = 100;

	for (int i = 0; i < 11; i++)
	{
		m_Mitter.push_back(new MITTER);
		if (i != 0)
			m_Mitter.at(i)->pos.m_fX = 20 + DISTANCE * i;
		else
			m_Mitter.at(i)->pos.m_fX = 20;
		m_Mitter.at(i)->pos.m_fY = 410;
		m_Mitter.at(i)->Distance = Distance;
		m_Mitter.at(i)->m_eImg = BACK_MITER;
		Distance -= 10;
	}

	m_Podium.pos.m_fX = m_Mitter.back()->pos.m_fX;
	m_Podium.pos.m_fY = 360;
	m_Podium.m_eImg = BACK_PODIUM;

}

//Update
void Background::Update(LINE line, bool state)
{
	m_eline = line;

	Move(state);
	PodiumRctUpdate();
	Render();
}


void Background::LeftMove()
{
	int imgSizeX = BitMapManager::GetSingleton()->GetImg(BACK_NOMAL0)->GetSize().cx*1.25;

	for (list<BACKGROUND*>::iterator iter = m_Fieldlist.begin(); iter != m_Fieldlist.end(); iter++)
	{
		(*iter)->pos.m_fX += LENGTH;
	}

	for (list<BACKGROUND*>::iterator iter = m_Audiencelist.begin(); iter != m_Audiencelist.end(); iter++)
	{
		(*iter)->pos.m_fX += LENGTH;
	}

	for (vector<MITTER*>::iterator iter = m_Mitter.begin(); iter != m_Mitter.end(); iter++)
	{
		(*iter)->pos.m_fX += LENGTH;

	}

	m_Podium.pos.m_fX += LENGTH;
}

void Background::RightMove()
{
	int imgSizeX = BitMapManager::GetSingleton()->GetImg(BACK_NOMAL0)->GetSize().cx*1.25;

	for (list<BACKGROUND*>::iterator iter = m_Audiencelist.begin(); iter != m_Audiencelist.end(); iter++)
	{
		(*iter)->pos.m_fX -= LENGTH;
		if ((*iter)->pos.m_fX + imgSizeX < 0)
		{
			int x = m_Audiencelist.back()->pos.m_fX;
			int y = m_Audiencelist.back()->pos.m_fY;

			if (x + imgSizeX < 512)
			{
				m_Audiencelist.push_back(new BACKGROUND);
				m_Audiencelist.back()->pos.m_fX = x + imgSizeX;
				m_Audiencelist.back()->pos.m_fY = y;
				m_Audiencelist.back()->m_eImg = BACK_NOMAL0;
			}
		}
	}


	for (list<BACKGROUND*>::iterator iter = m_Fieldlist.begin(); iter != m_Fieldlist.end(); iter++)
	{
		(*iter)->pos.m_fX -= LENGTH;
		if ((*iter)->pos.m_fX + imgSizeX < 0)
		{
			int x = m_Fieldlist.back()->pos.m_fX;
			int y = m_Fieldlist.back()->pos.m_fY;

			if (x + imgSizeX < 512)
			{
				m_Fieldlist.push_back(new BACKGROUND);
				m_Fieldlist.back()->pos.m_fX = x + imgSizeX;
				m_Fieldlist.back()->pos.m_fY = y;
				m_Fieldlist.back()->m_eImg = BACK_WAY;
			}

		}
	}

	for (vector<MITTER*>::iterator iter = m_Mitter.begin(); iter != m_Mitter.end(); iter++)
	{
		(*iter)->pos.m_fX -= LENGTH;
	}
	m_Podium.pos.m_fX -= LENGTH;
}

void Background::Move(bool state)
{
	
	if (state)
	{
		if (m_eDirection == DIRECTION_RIGHT)
		{
			RightMove();
		}
		else if (m_eDirection == DIRECTION_LEFT)
		{

			LeftMove();
		}
		else
			return;

		return;
	}
	
	if(m_eline != ENDPOS)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_eDirection = DIRECTION_LEFT;
			LeftMove();
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_eDirection = DIRECTION_RIGHT;
			RightMove();
		}
		else
		{
			m_eDirection = DIRECTION_NONE;
		}
	}
}


bool Background::PodiumCheck(RECT rect)
{
	RECT rcTemp = { 0 };

	if (IntersectRect(&rcTemp, &m_Podium.rct, &rect))
	{
		return true;
	}

	return false;
}

void Background::PodiumRctUpdate()
{
	m_Podium.rct.left = m_Podium.pos.m_fX;
	m_Podium.rct.top = m_Podium.pos.m_fY;
	m_Podium.rct.right = m_Podium.rct.left + BitMapManager::GetSingleton()->GetImg(m_Podium.m_eImg)->GetSize().cx;
	m_Podium.rct.bottom = m_Podium.rct.top + BitMapManager::GetSingleton()->GetImg(m_Podium.m_eImg)->GetSize().cy;
}

void Background::EndMotion()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_fDeltaTime > 0.08f)
	{
		for (list<BACKGROUND*>::iterator iter = m_Audiencelist.begin(); iter != m_Audiencelist.end(); iter++)
		{
			switch ((*iter)->m_eImg)
			{
			case BACK_NOMAL0:
				(*iter)->m_eImg = BACK_NOMAL1;
				break;
			case BACK_NOMAL1:
				(*iter)->m_eImg = BACK_NOMAL0;
				break;
			}
			m_dwLastTime = m_dwCurTime;
		}
	}
}

void Background::RestoreBckgrd(float Playerpos)
{
	backlength = Playerpos - m_Mitter[NowDistance]->pos.m_fX - 80;

	for (list<BACKGROUND*>::iterator iter = m_Audiencelist.begin(); iter != m_Audiencelist.end(); iter++)
	{
		(*iter)->pos.m_fX += backlength;
	}

	for (vector<MITTER*>::iterator iter = m_Mitter.begin(); iter != m_Mitter.end(); iter++)
	{
		(*iter)->pos.m_fX += backlength;

	}

	m_Podium.pos.m_fX += backlength;
}

void Background::CheckDistacne(float Playerpos)
{
	for (int i = 0; i < m_Mitter.size(); i++)
	{
		if (Playerpos > m_Mitter.at(i)->pos.m_fX + BitMapManager::GetSingleton()->GetImg(BACK_MITER)->GetSize().cx)
			NowDistance = i;
	}
}

//Render
void Background::Render()
{
	
	TCHAR str[128];
	int AudienceNum = 0;

	for (list<BACKGROUND*>::iterator iter = m_Audiencelist.begin(); iter != m_Audiencelist.end(); iter++)
	{
		if (AudienceNum == DECO_POINT)
			BitMapManager::GetSingleton()->GetImg(BACK_DECO)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1.3);
		else
			BitMapManager::GetSingleton()->GetImg((*iter)->m_eImg)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1.3);
		AudienceNum++;
		if (AudienceNum == AUDIENCE_END)
			AudienceNum = 0;
	}

	for (list<BACKGROUND*>::iterator iter = m_Fieldlist.begin(); iter != m_Fieldlist.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetImg((*iter)->m_eImg)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1.25);
	}

	for (vector<MITTER*>::iterator iter = m_Mitter.begin(); iter != m_Mitter.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetImg((*iter)->m_eImg)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1);
		if((*iter)->Distance == 0)
			m_Font.Text((*iter)->pos.m_fX+20, (*iter)->pos.m_fY+8, L"00", 0x00ffffff);
		else
		{
			wsprintf(str, TEXT("%d"), (*iter)->Distance);
			m_Font.Text((*iter)->pos.m_fX + 20, (*iter)->pos.m_fY + 8,str, 0x00ffffff);
		}
	}

	
	BitMapManager::GetSingleton()->GetImg(BACK_PODIUM)->Draw(m_backbufferDC, m_Podium.pos.m_fX, m_Podium.pos.m_fY, 1, 1);
}

//Getter
float Background::GetMitterPos(int index)
{
	return m_Mitter[index]->pos.m_fX + BitMapManager::GetSingleton()->GetImg(BACK_MITER)->GetSize().cx;
}

//Release
void Background::Release()
{

	for (vector<MITTER*>::iterator it = m_Mitter.begin(); it != m_Mitter.end(); it++)

	{
		delete (*it);
	}
	m_Mitter.clear();

	for (list<BACKGROUND*>::iterator it = m_Audiencelist.begin(); it != m_Audiencelist.end(); it++)

	{
		delete (*it);
	}
	m_Audiencelist.clear();



	for (list<BACKGROUND*>::iterator it = m_Fieldlist.begin(); it != m_Fieldlist.end(); it++)

	{
		delete (*it);
	}
	m_Fieldlist.clear();

}



Background::~Background()
{
}

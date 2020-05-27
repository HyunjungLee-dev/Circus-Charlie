#include "Enemy.h"



Enemy::Enemy()
{
}

void Enemy::Init()
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	SetJar();
}


void Enemy::SetJar()
{
	for (int i = 0; i < 9; i++)
	{
		Jar.push_back(new Fire);
		if (i != 0)
			Jar.back()->pos.m_fX = Jar.front()->pos.m_fX + DISTANCE * i;
		else
			Jar.back()->pos.m_fX = 960.0;
		Jar.back()->pos.m_fY= 350.0;

		Jar.back()->type = FIRE_JAR;
	}
	UpdateRectPos();
}

void Enemy::UpdateRectPos()
{
	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		(*iter)->FireRect.left = (*iter)->pos.m_fX * 1.2;
		(*iter)->FireRect.top = (*iter)->pos.m_fY;
		(*iter)->FireRect.right = ((*iter)->FireRect.left + (BitMapManager::GetSingleton()->GetFire(Jar.back()->type).GetSize().cx)*0.3);
		(*iter)->FireRect.bottom = (*iter)->FireRect.top + BitMapManager::GetSingleton()->GetFire(Jar.back()->type).GetSize().cy;
	}
	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		(*iter)->FireRect.left = (*iter)->pos.m_fX * 1.3;
		(*iter)->FireRect.top = (*iter)->pos.m_fY * 1.8 ;
		(*iter)->FireRect.right = (*iter)->FireRect.left + BitMapManager::GetSingleton()->GetFire(Ring.back()->type).GetSize().cx*0.8;
		(*iter)->FireRect.bottom = (*iter)->FireRect.top + BitMapManager::GetSingleton()->GetFire(Ring.back()->type).GetSize().cy*0.2;
	}
}

void Enemy::SetRing() 
{
	int MaxRing = 3;
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_fDeltaTime > 2.0f ) // 2√ 
	{
		for (int i = 0; i < 2; i++)
		{
			if (Ring.size() > MaxRing)
				break;
			Ring.push_back(new Fire);
			if (i == 0)
			{
				Ring.back()->pos.m_fX = 512;
				Ring.back()->pos.m_fY = 180;
				Ring.back()->type = FIRE_HALF_LEFT;
			}
			else
			{
				Ring.back()->pos.m_fX = 512 + BitMapManager::GetSingleton()->GetFire(FIRE_HALF_LEFT).GetSize().cx;
				Ring.back()->pos.m_fY = 180;
				Ring.back()->type = FIRE_HALF_RIGHT;
			}
			m_dwLastTime = m_dwCurTime;
		}
		UpdateRectPos();
	}

}

void Enemy::Update(END end) 
{

	SetRing();

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (end != ENDLINE)
		{
			for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
			{
				(*iter)->pos.m_fX += LENGTH;
			}
		}
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		if (end != ENDLINE)
		{
			for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
			{
				(*iter)->pos.m_fX -= LENGTH;
			}
		}
		for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
		{
			(*iter)->pos.m_fX -= LENGTH;
		}
	}



	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		Motion((*iter));
	}

	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		(*iter)->pos.m_fX -= LENGTH;
		Motion((*iter));
		if ((*iter)->pos.m_fX < 0.0f)
		{
			list<Fire*>::iterator tmp = iter;
			Ring.erase(++tmp);
			Ring.erase(iter);
			break;
		}
	}

	UpdateRectPos();
}

void Enemy::backRing()
{
	Ring.clear();
}


void Enemy::backEnemy(float back)
{

	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		(*iter)->pos.m_fX += back;
	}

	UpdateRectPos();
}



bool Enemy::Collision(RECT rect)
{
	RECT rcTemp = { 0 };

	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		if (IntersectRect(&rcTemp, &(*iter)->FireRect, &rect))
		{
			return true;
		}
	}

	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		if ((*iter)->type == FIRE_HALF_LEFT || (*iter)->type == FIRE_HALF_LEFTB)
		{
			if (IntersectRect(&rcTemp, &(*iter)->FireRect, &rect))
			{
				return true;
			}
		}
	}
	return false;
}

void Enemy::Motion(Fire* f)
{
	switch (f->type)
	{
	case FIRE_JAR:
		f->type = FIRE_JARB;
		break;
	case FIRE_JARB:
		f->type = FIRE_JAR;
		break;
	case FIRE_HALF_LEFT:
		f->type = FIRE_HALF_LEFTB;
		break;
	case FIRE_HALF_LEFTB:
		f->type = FIRE_HALF_LEFT;
		break;
	case FIRE_HALF_RIGHT:
		f->type = FIRE_HALF_RIGHTB;
		break;
	case FIRE_HALF_RIGHTB:
		f->type = FIRE_HALF_RIGHT;
		break;
	default:
		break;
	}
}

void Enemy::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetFire((*iter)->type).Draw(hdc, (*iter)->pos.m_fX, (*iter)->pos.m_fY,1,1);
	}

	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		//RECT rct = (*iter)->FireRect;
		//Rectangle(hdc, rct.left, rct.top, rct.right, rct.bottom);
		BitMapManager::GetSingleton()->GetFire((*iter)->type).Draw(hdc, (*iter)-> pos.m_fX, (*iter)-> pos.m_fY,1,1.2);
	}

}

void Enemy::HalfRender()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		if((*iter)->type == FIRE_HALF_RIGHT || (*iter)->type == FIRE_HALF_RIGHTB)
		BitMapManager::GetSingleton()->GetFire((*iter)->type).Draw(hdc, (*iter)->pos.m_fX, (*iter)->pos.m_fY,1,1.2);
	}
}

void Enemy::Clear(list<Fire*> l)
{
	for (list<Fire*>::iterator it = l.begin(); it != l.end(); it++)
	{
		delete (*it);

	}
	l.clear();
}

void Enemy::Release()
{
	Clear(Ring);
	Clear(Jar);
}


Enemy::~Enemy()
{
	Release();
}

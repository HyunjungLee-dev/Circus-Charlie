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
		int Distance = 600;

		Jar.push_back(new Fire);
		if (i != 0)
			Jar.back()->pos.m_fX = Jar.front()->pos.m_fX + Distance * i;
		else
			Jar.back()->pos.m_fX = 1150.0f;
		Jar.back()->pos.m_fY= 350.0;

		Jar.back()->type = FIRE_JAR;

		Jar.back()->FireRect.left = Jar.back()->pos.m_fX;
		Jar.back()->FireRect.top = Jar.back()->pos.m_fY;
		Jar.back()->FireRect.right = Jar.back()->FireRect.left + BitMapManager::GetSingleton()->GetFire(Jar.back()->type).GetSize().cx;
		Jar.back()->FireRect.right = Jar.back()->FireRect.top + BitMapManager::GetSingleton()->GetFire(Jar.back()->type).GetSize().cy;
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
				Ring.back()->pos.m_fX = 512 + BitMapManager::GetSingleton()->GetFire(FIRE_HALF_LEFT).GetSize().cx;;
				Ring.back()->pos.m_fY = 180;
				Ring.back()->type = FIRE_HALF_RIGHT;
			}
			Ring.back()->FireRect.left = Ring.back()->pos.m_fX;
			Ring.back()->FireRect.top = Ring.back()->pos.m_fY;
			Ring.back()->FireRect.right = Ring.back()->FireRect.left + BitMapManager::GetSingleton()->GetFire(Ring.back()->type).GetSize().cx;
			Ring.back()->FireRect.right = Ring.back()->FireRect.top + BitMapManager::GetSingleton()->GetFire(Ring.back()->type).GetSize().cy;

			m_dwLastTime = m_dwCurTime;
		}
	}

}

void Enemy::Update() 
{

	SetRing();

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
		{
			(*iter)->pos.m_fX += LENGTH;
		}
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
		{
			(*iter)->pos.m_fX -= LENGTH;
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

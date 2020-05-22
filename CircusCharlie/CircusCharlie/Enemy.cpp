#include "Enemy.h"



Enemy::Enemy()
{
}

void Enemy::Init()
{
	SetJar();
}


void Enemy::SetJar()
{
	for (int i = 0; i < 9; i++)
	{
		int Distance = 600;

		Jar.push_back(new Fire);
		if (i != 0)
			Jar.back()->pos.x = Jar.front()->pos.x + Distance * i;
		else
			Jar.back()->pos.x = 1150;
		Jar.back()->pos.y = 350;

		Jar.back()->type = FIRE_JAR;

		Jar.back()->FireRect.left = Jar.back()->pos.x;
		Jar.back()->FireRect.top = Jar.back()->pos.y;
		Jar.back()->FireRect.right = Jar.back()->FireRect.left + BitMapManager::GetSingleton()->GetFire(Jar.back()->type).GetSize().cx;
		Jar.back()->FireRect.right = Jar.back()->FireRect.top + BitMapManager::GetSingleton()->GetFire(Jar.back()->type).GetSize().cy;
	}
}

void Enemy::SetRing()
{
	m_fNowTime = (float)timeGetTime() * 0.001f;

	if (m_fNowTime - m_fStartTime >= 0.1f) // 0.1초마다 실행
	{
		Ring.push_back(new Fire);
		Ring.back()->pos.x = 512;
		Ring.back()->pos.y = 180;
		Ring.back()->type = FIRE_NOMAL;
		Ring.back()->FireRect.left = Ring.back()->pos.x;
		Ring.back()->FireRect.top = Ring.back()->pos.y;
		Ring.back()->FireRect.right = Ring.back()->FireRect.left + BitMapManager::GetSingleton()->GetFire(Ring.back()->type).GetSize().cx;
		Ring.back()->FireRect.right = Ring.back()->FireRect.top + BitMapManager::GetSingleton()->GetFire(Ring.back()->type).GetSize().cy;
	}


	m_fStartTime = m_fNowTime;
}

void Enemy::Update()
{
	SetRing();

	if (GetKeyState(VK_LEFT) & 0x8000)
	{		 
		for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
		{
			(*iter)->pos.x += LENGTH;
		}
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
		{
			(*iter)->pos.x -= LENGTH;
		}
	}


	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		Motion((*iter));
	}
	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		if (Ring.front()->pos.x < 0)
			Ring.pop_front();
		(*iter)->pos.x -= LENGTH;
		Motion((*iter));
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
	case FIRE_NOMAL :
		f->type = FIRE_NOMAL_B;
	case FIRE_NOMAL_B :
		f->type = FIRE_NOMAL;
	default:
		break;
	}
}

void Enemy::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBackBuffer().GetMemDC();

	for (list<Fire*>::iterator iter = Jar.begin(); iter != Jar.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetFire((*iter)->type).Draw(hdc, (*iter)->pos.x, (*iter)->pos.y);
	}

	for (list<Fire*>::iterator iter = Ring.begin(); iter != Ring.end(); iter++)
	{
		BitMapManager::GetSingleton()->GetFire((*iter)->type).Draw(hdc, (*iter)->pos.x, (*iter)->pos.y);
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

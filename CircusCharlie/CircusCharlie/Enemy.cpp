#include "Enemy.h"



Enemy::Enemy()
{
}

//Init

void Enemy::Init()
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_bPassCheck = false;
	m_backbufferDC = BitMapManager::GetSingleton()->GetBufferDC();
	SetJar();
}

void Enemy::SetJar()
{
	for (int i = 0; i < 10; i++)
	{
		m_Jar.push_back(new Fire);
		if (i != 0)
			m_Jar.back()->pos.m_fX = m_Jar.front()->pos.m_fX + DISTANCE * i;
		else
			m_Jar.back()->pos.m_fX = DISTANCE*1.95;
		m_Jar.back()->pos.m_fY = 360.0f;
		m_Jar.back()->type = FIRE_JAR;
		m_Jar.back()->Item = NONE;
	}
}

void Enemy::SetRing()
{
	int MaxRing = 3;
	int Random = rand() % 100;

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_fDeltaTime > 2.0f)
	{
		for (int i = 0; i < 2; i++)
		{
			if (m_Ringlist.size() > MaxRing)
				break;
			m_Ringlist.push_back(new Fire);
			if (i == 0)
			{
				m_Ringlist.back()->pos.m_fX = 512;
				m_Ringlist.back()->pos.m_fY = 180;
				m_Ringlist.back()->type = FIRE_HALF_LEFT;
				m_Ringlist.back()->Item = NONE;
			}
			else
			{
				m_Ringlist.back()->pos.m_fX = 512 + BitMapManager::GetSingleton()->GetImg(FIRE_HALF_LEFT)->GetSize().cx;
				m_Ringlist.back()->pos.m_fY = 180;
				m_Ringlist.back()->type = FIRE_HALF_RIGHT;
				if (Random > 70)
					m_Ringlist.back()->Item = HAVE;
				else
					m_Ringlist.back()->Item = NONE;
			}
			m_dwLastTime = m_dwCurTime;
		}
	}
}


//Update
void Enemy::Update(LINE line)
{
	m_eLine = line;

	SetRing();
	Move();
	UpdateRectPos();
	Render();
}

void Enemy::Move()
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
		{
			if ( m_eLine != STARTLINE)
			{
				(*iter)->pos.m_fX += LENGTH;
			}
		}
		
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
		{
			if ( m_eLine != STARTLINE)
			{
				(*iter)->pos.m_fX -= LENGTH;
			}
		}
		
		for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
		{
			(*iter)->pos.m_fX -= LENGTH;
		}
	}



	for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
	{
		Motion((*iter));
	}

	for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
	{
		(*iter)->pos.m_fX -= LENGTH;
		Motion((*iter));
		if ((*iter)->pos.m_fX < 0.0f)
		{
			list<Fire*>::iterator m_eStarColor = iter;
			m_Ringlist.erase(++m_eStarColor);
			m_Ringlist.erase(iter);
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

void Enemy::UpdateRectPos()
{
	for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
	{
		(*iter)->FireRect.left = (*iter)->pos.m_fX;
		(*iter)->FireRect.top = 350.0f;
		(*iter)->FireRect.right = ((*iter)->FireRect.left + (BitMapManager::GetSingleton()->GetImg(m_Jar.back()->type)->GetSize().cx));
		(*iter)->FireRect.bottom = (*iter)->FireRect.top + BitMapManager::GetSingleton()->GetImg(m_Jar.back()->type)->GetSize().cy;
	}

	for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
	{
		list<Fire*>::iterator tmp;
		tmp = iter;
		if ((*iter)->type == FIRE_HALF_LEFT || (*iter)->type == FIRE_HALF_LEFTB)
		{
			++tmp;
			(*iter)->FireRect.left = (*iter)->pos.m_fX + 15;
			if ((*tmp)->Item == HAVE || (*iter)->Item == CHECK)
			{
				(*iter)->FireRect.top = (*iter)->pos.m_fY * 1.7f;
			}
			else
			{
				(*iter)->FireRect.top = (*iter)->pos.m_fY * 1.85f;
			}
		}
		else
		{
			(*iter)->FireRect.left = (*iter)->pos.m_fX;
			if ((*iter)->Item == HAVE || (*iter)->Item == CHECK)
			{
				(*iter)->FireRect.top = (*iter)->pos.m_fY * 1.7f;
			}
			else
			{
				(*iter)->FireRect.top = (*iter)->pos.m_fY * 1.85f;
			}

		}
		(*iter)->FireRect.bottom = (*iter)->FireRect.top + BitMapManager::GetSingleton()->GetImg(m_Ringlist.back()->type)->GetSize().cy*0.15f;
		(*iter)->FireRect.right = (*iter)->FireRect.left + BitMapManager::GetSingleton()->GetImg(m_Ringlist.back()->type)->GetSize().cx - 15;
	}
}

bool Enemy::Collision(RECT rect)
{
	RECT rcTemp = { 0 };

	for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
	{
		if (IntersectRect(&rcTemp, &(*iter)->FireRect, &rect))
		{
			return true;
		}
	}

	for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
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

void Enemy::RestoreEnemy(float back)
{

	for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
	{
		(*iter)->pos.m_fX += back;
	}

	UpdateRectPos();
}

PASS Enemy::JarPassCheck(float x)
{
	for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
	{
		if (x > (*iter)->pos.m_fX && x < (*iter)->pos.m_fX + BitMapManager::GetSingleton()->GetImg(FIRE_JAR)->GetSize().cx)
			return PASS_JAR;
	}
	return PASS_NOT;
}

PASS Enemy::RingPassCheck(float x)
{
	for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
	{
		if (x > (*iter)->pos.m_fX && x <= (*iter)->pos.m_fX + BitMapManager::GetSingleton()->GetImg(FIRE_JAR)->GetSize().cx)
		{
			if ((*iter)->Item == HAVE || (*iter)->Item == CHECK)
			{
				(*iter)->Item = CHECK;
				return PASS_ITEMRING;
			}
			return PASS_RING;
		}
	}
	return PASS_NOT;
}


//Render
void Enemy::Render()
{
	for (vector<Fire*>::iterator iter = m_Jar.begin(); iter != m_Jar.end(); iter++)
	{

		BitMapManager::GetSingleton()->GetImg((*iter)->type)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1, 1);
	}

	for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
	{
		list<Fire*>::iterator tmp;
		tmp = iter;
		if ((*iter)->type == FIRE_HALF_LEFT || (*iter)->type == FIRE_HALF_LEFTB)
		{
			++tmp;
			if ((*tmp)->Item == HAVE || (*tmp)->Item == CHECK)
			{
				BitMapManager::GetSingleton()->GetImg((*iter)->type)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1, 1.1);
			}
			else
				BitMapManager::GetSingleton()->GetImg((*iter)->type)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1, 1.3);
		}
	}
}

void Enemy::HalfRender()
{
	for (list<Fire*>::iterator iter = m_Ringlist.begin(); iter != m_Ringlist.end(); iter++)
	{
		if ((*iter)->type == FIRE_HALF_RIGHT || (*iter)->type == FIRE_HALF_RIGHTB)
		{
			if ((*iter)->Item == HAVE || (*iter)->Item == CHECK )
			{
				if((*iter)->Item == HAVE)
					BitMapManager::GetSingleton()->GetImg(FIRE_ITEM)->Draw(m_backbufferDC, (*iter)->pos.m_fX - BitMapManager::GetSingleton()->GetImg(FIRE_ITEM)->GetSize().cx*0.5, (*iter)->pos.m_fY + BitMapManager::GetSingleton()->GetImg(FIRE_ITEM)->GetSize().cy, 1, 1);
				BitMapManager::GetSingleton()->GetImg((*iter)->type)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1, 1.1);
			}
			else
				BitMapManager::GetSingleton()->GetImg((*iter)->type)->Draw(m_backbufferDC, (*iter)->pos.m_fX, (*iter)->pos.m_fY, 1, 1.3);
		}
		
	}
}


//Release
void Enemy::Release()
{
	JarClear();
	RingClear();
}

void Enemy::JarClear()
{
	for (vector<Fire*>::iterator it = m_Jar.begin(); it != m_Jar.end(); it++)

	{
		delete (*it);
	}
	m_Jar.clear();
}

void Enemy::RingClear()
{
	for (list<Fire*>::iterator it = m_Ringlist.begin(); it != m_Ringlist.end(); it++)

	{
		delete (*it);
	}
	m_Ringlist.clear();
}

Enemy::~Enemy()
{
}

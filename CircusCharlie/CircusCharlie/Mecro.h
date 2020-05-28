#pragma once
#include<windows.h>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<list>
#include<math.h>
using namespace std;

#define PI	3.141592f
#define LENGTH 1.2f
#define DISTANCE 500

struct POS
{
	float m_fX;
	float m_fY;
};



enum END
{
	ENDLINE,
	ENDPOS,
	NOTEND,
};

enum ITEM
{
	NONE,
	HAVE,
};

enum TYPE
{
	TYPE_PLAYER,
	TYPE_BACKGROUND,
	TYPE_FIRE,
	TYPE_ICON
};

enum PLAYER_IMG
{
	PLAYER_START,
	PLAYER_MOVE0 = 0,
	PLAYER_MOVE1,
	PLAYER_MOVE2,
	PLAYER_WIN0,
	PLAYER_WIN1,
	PLAYER_DIE,
	PLAYER_END
};

enum DIRECTION
{
	DIRECTION_NONE,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

enum PLAYERSTATE
{
	IDLE,
	MOVE,
	JUMP,
	P_END,
};


enum PASS
{
	PASS_NOT,
	PASS_JAR = 200,
	PASS_RING = 100,
	PASS_DOUBLE,
	PASS_BONOUS = 100,
	PASS_ITEMRING = 1000,
};

enum FIRE_IMG
{
	FIRE_START,
	FIRE_NOMAL = 0,
	FIRE_NOMAL_B,
	FIRE_HALF_LEFT,
	FIRE_HALF_LEFTB,
	FIRE_HALF_RIGHT,
	FIRE_HALF_RIGHTB,
	FIRE_ITEM,
	FIRE_JAR,
	FIRE_JARB,
	FIRE_END 
};

enum BACKGROUND_IMG
{
	BACK_START,
	BACK_WAY = 0,
	BACK_DECO,
	BACK_NOMAL0,
	BACK_NOMAL1,
	BACK_PODIUM,
	BACK_MITER,
	BACK_END 
};

enum ICON_IMG
{
	ICON_START,
	ICON_LIFE,
	ICON_STAR_B,
	ICON_STAR_Y,
	ICON_STAR_R,
	ICON_TITLE,
	ICON_END,
};

enum GAMESTATE
{
	GAME_MAIN,
	GAME_START,
	GAME_STOP,
	GAME_DIE,
	GAME_RE,
	GAME_INIT,
	GAME_PLAY,
	GAME_END
};

enum BACKPOINT
{
	START_POINT = 0,
	DECO_POINT = 2,
	FIELD_END = 8,
	AUDIENCE_END = 8,
};
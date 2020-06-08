#pragma once
#include<windows.h>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<list>
#include<math.h>
using namespace std;

#define PI	3.141592f
#define LENGTH 0.5f
#define DISTANCE 550


struct POS
{
	float m_fX;
	float m_fY;
};


//BitmapManager

enum IMG
{
	IMG_START,

	PLAYER_MOVE0 = 0,
	PLAYER_MOVE1,
	PLAYER_MOVE2,
	PLAYER_WIN0,
	PLAYER_WIN1,
	PLAYER_DIE,
	PLAYER_END = 5,

	FIRE_NOMAL,
	FIRE_NOMAL_B,
	FIRE_HALF_LEFT,
	FIRE_HALF_LEFTB,
	FIRE_HALF_RIGHT,
	FIRE_HALF_RIGHTB,
	FIRE_ITEM,
	FIRE_JAR,
	FIRE_JARB,


	BACK_WAY,
	BACK_DECO,
	BACK_NOMAL0,
	BACK_NOMAL1,
	BACK_PODIUM,
	BACK_MITER,


	ICON_LIFE,
	ICON_STAR_B,
	ICON_STAR_Y,
	ICON_STAR_R,
	ICON_TITLE,


	IMG_END = 26,
};

// GameManager

enum GAMESTATE
{
	GAME_NONE,
	GAME_MAIN,
	GAME_WAIT,
	GAME_INIT,
	GAME_START,
	GAME_STOP,
	GAME_DIE,
	GAME_RE,
	GAME_PLAY,
	GAME_END
};

struct Stat
{
	int Bonus;
	int Stage;
	int Score;
	int Hi;
};

enum LINE
{
	ENDLINE,
	ENDPOS,
	NOTEND,
	STARTLINE
};

struct Star
{
	IMG starColor;
	POS pos;
};



//Player

enum PLAYERSTATE
{
	IDLE,
	MOVE,
	JUMP,
	P_END,
};

enum DIRECTION
{
	DIRECTION_NONE,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

//Background

struct BACKGROUND
{
	POS pos;
	IMG m_eImg;
};

struct MITTER
{
	int Distance;
	POS pos;
	IMG m_eImg;
};

struct PODIUM
{
	POS pos;
	RECT rct;
	IMG m_eImg;
};

enum BACKPOINT
{
	START_POINT = 0,
	DECO_POINT = 2,
	FIELD_END = 8,
	AUDIENCE_END = 7,
};

//Enemy
enum ITEM
{
	NONE,
	HAVE,
	CHECK,
};

struct  Fire
{
	POS pos;
	IMG type;
	ITEM Item;
	RECT FireRect;
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


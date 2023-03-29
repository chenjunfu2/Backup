#pragma once
#include <stddef.h>


/*
//地图符号
0：人♀
1：人在目的地♀
*/

class Player
{
public:
	enum Block :unsigned char
	{
		PlayerInBlank = 0,	//0：人♀
		PlayerInDestn = 1,	//1：人在目的地♀
	};
public:
	//在地图中的位置
	long x;
	long y;
	Block enPlayerStatus;//状态

	Player(long _x, long _y, Block _enPlayerStatus = PlayerInBlank) :x(_x), y(_y), enPlayerStatus(_enPlayerStatus)
	{}
	~Player(void) = default;
};
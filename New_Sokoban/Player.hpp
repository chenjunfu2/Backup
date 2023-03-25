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
	enum Block
	{
		PlayerInBlank = 0,	//0：人♀
		PlayerInDestn = 1,	//1：人在目的地♀
	};
public:
	//在地图中的位置
	long x;
	long y;
	Block enPlayer;//状态
};
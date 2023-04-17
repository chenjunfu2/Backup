#pragma once
#include <stddef.h>
#include <stdint.h>

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

	struct File
	{
		uint64_t u64X;//玩家坐标x
		uint64_t u64Y;//玩家坐标y
		Block enPlayerStatus;//玩家状态
	};
public:
	//在地图中的位置
	long x;
	long y;
	Block enPlayerStatus;//状态
public:
	Player(const File &_File) :
		x(_File.u64X), y(_File.u64Y), enPlayerStatus(_File.enPlayerStatus)
	{}

	const File GetFile(void)
	{
		return File(x, y, enPlayerStatus);
	}
public:
	Player(long _x, long _y, Block _enPlayerStatus = PlayerInBlank) :x(_x), y(_y), enPlayerStatus(_enPlayerStatus)
	{}
	~Player(void) = default;
};
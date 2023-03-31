#pragma once
#include <stddef.h>
#include <new>
#include <string.h>

/*
//地图符号
0：空地"  "
1：墙壁"■"
2：目的地"☆" "○"
3：箱子"□"
4：箱子在目的地"★" "●"
*/

class Map//仅用于管理，不做内存分配与释放
{
public:
	enum Block :unsigned char
	{
		Blank = 0,		//0：空地"  "
		Wall = 1,		//1：墙壁"■"
		Destn = 2,		//2：目的地"☆" "○"
		BoxInBlank = 3,	//3：箱子"□"
		BoxInDestn = 4,	//4：箱子在目的地"★" "●"
	};

	struct File
	{
		uint64_t u64MapWidth;//地图宽
		uint64_t u64MapHight;//地图高
		uint64_t u64AllBoxNum;//箱子总数
		uint64_t u64DestnBoxNum;//在终点的箱子数
		Block *enpMapData;//变长地图数据集
	};
private:
	Block *enpMap;//地图数组
	long lMapWidth;//地图宽高
	long lMapHight;

	long lAllBoxNum;//箱子总数
	long lDestnBoxNum;//在终点的箱子数
public:
	Map(const File &_File) :
		enpMap(_File.enpMapData), lMapWidth(_File.u64MapWidth), lMapHight(_File.u64MapHight), 
		lAllBoxNum(_File.u64AllBoxNum), lDestnBoxNum(_File.u64DestnBoxNum)
	{}

	const File GetFile(void) const
	{
		return File(lMapWidth, lMapHight, lAllBoxNum, lDestnBoxNum, enpMap);
	}
public:
	Map(Block *_pMap, long _lMapWidth, long _lMapHight) :
		enpMap(nullptr), lMapWidth(_lMapWidth), lMapHight(_lMapHight), lAllBoxNum(0), lDestnBoxNum(0)
	{
		enpMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (enpMap == nullptr)
		{
			return;
		}
		//拷贝地图
		memcpy(enpMap, _pMap, sizeof(Block) * lMapWidth * lMapHight);

		//计算箱子数目
		for (long long i = 0; i < lMapWidth * lMapHight; ++i)
		{
			if (enpMap[i] == BoxInBlank)
			{
				++lAllBoxNum;//递增总数
			}
			else if (enpMap[i] == BoxInDestn)
			{
				++lAllBoxNum;
				++lDestnBoxNum;//递增在终点的箱子数
			}
		}
	}

	Map(const Map &_Map) :
		enpMap(nullptr), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		enpMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (enpMap == nullptr)
		{
			return;
		}
		//拷贝地图
		memcpy(enpMap, _Map.enpMap, sizeof(Block) * lMapWidth * lMapHight);
	}

	Map(Map &&_Map) :
		enpMap(_Map.enpMap), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		_Map.enpMap = nullptr;
		_Map.lMapWidth = 0;
		_Map.lMapHight = 0;
		_Map.lAllBoxNum = 0;
		_Map.lDestnBoxNum = 0;
	}

	~Map(void)
	{
		delete[] enpMap;
		enpMap = nullptr;
	}

	Map &operator=(Map &&_Map)
	{
		enpMap = _Map.enpMap;
		lMapWidth = _Map.lMapWidth;
		lMapHight = _Map.lMapHight;
		lAllBoxNum = _Map.lAllBoxNum;
		lDestnBoxNum = _Map.lDestnBoxNum;

		_Map.enpMap = nullptr;
		_Map.lMapWidth = 0;
		_Map.lMapHight = 0;
		_Map.lAllBoxNum = 0;
		_Map.lDestnBoxNum = 0;

		return *this;
	}

	Map &operator=(const Map &_Map)
	{
		lMapWidth = _Map.lMapWidth;
		lMapHight = _Map.lMapHight;
		lAllBoxNum = _Map.lAllBoxNum;
		lDestnBoxNum = _Map.lDestnBoxNum;

		delete[] enpMap;
		enpMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (enpMap == nullptr)
		{
			return *this;
		}
		//拷贝地图
		memcpy(enpMap, _Map.enpMap, sizeof(Block) * lMapWidth * lMapHight);

		return *this;
	}

	Block &operator()(long x, long y)//访问多维数组
	{
		return enpMap[y * lMapWidth + x];
	}

	const Block &operator()(long x, long y) const//访问多维数组
	{
		return enpMap[y * lMapWidth + x];
	}

	long Width(void) const
	{
		return lMapWidth;
	}

	long Hight(void) const
	{
		return lMapHight;
	}

	long AllBoxNum(void) const
	{
		return lAllBoxNum;
	}

	long DestnBoxNum(void) const
	{
		return lDestnBoxNum;
	}

	void IncDestnBoxNum(void)
	{
		++lDestnBoxNum;
	}

	void DecDestnBoxNum(void)
	{
		--lDestnBoxNum;
	}
};
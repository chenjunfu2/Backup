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
private:
	Block *pMap;//地图数组
	long lMapWidth;//地图宽高
	long lMapHight;

	long lAllBoxNum;//箱子总数
	long lDestnBoxNum;//在终点的箱子数
public:
	Map(Block *_pMap, long _lMapWidth, long _lMapHight, bool bMovePointer = false) :
		pMap(_pMap), lMapWidth(_lMapWidth), lMapHight(_lMapHight), lAllBoxNum(0), lDestnBoxNum(0)
	{
		//如果地图指针不是移动进来的，则进行内存分配和拷贝
		if (!bMovePointer)
		{
			pMap = new(std::nothrow) Block[lMapWidth * lMapHight];
			if (pMap == nullptr)
			{
				return;
			}
			//拷贝地图
			memcpy(pMap, _pMap, sizeof(Block) * lMapWidth * lMapHight);
		}

		//计算箱子数目
		for (long long i = 0; i < lMapWidth * lMapHight; ++i)
		{
			if (pMap[i] == BoxInBlank)
			{
				++lAllBoxNum;//递增总数
			}
			else if (pMap[i] == BoxInDestn)
			{
				++lAllBoxNum;
				++lDestnBoxNum;//递增在终点的箱子数
			}
		}
	}

	Map(const Map &_Map) :
		pMap(nullptr), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		pMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (pMap == nullptr)
		{
			return;
		}
		//拷贝地图
		memcpy(pMap, _Map.pMap, sizeof(Block) * lMapWidth * lMapHight);
	}

	Map(Map &&_Map) :
		pMap(_Map.pMap), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		_Map.pMap = nullptr;
		_Map.lMapWidth = 0;
		_Map.lMapHight = 0;
		_Map.lAllBoxNum = 0;
		_Map.lDestnBoxNum = 0;
	}

	~Map(void)
	{
		delete[] pMap;
		pMap = nullptr;
	}

	Map &operator=(Map &&_Map)
	{
		pMap = _Map.pMap;
		lMapWidth = _Map.lMapWidth;
		lMapHight = _Map.lMapHight;
		lAllBoxNum = _Map.lAllBoxNum;
		lDestnBoxNum = _Map.lDestnBoxNum;

		_Map.pMap = nullptr;
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

		delete[] pMap;
		pMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (pMap == nullptr)
		{
			return *this;
		}
		//拷贝地图
		memcpy(pMap, _Map.pMap, sizeof(Block) * lMapWidth * lMapHight);

		return *this;
	}

	Block &operator()(long x, long y)//访问多维数组
	{
		return pMap[y * lMapWidth + x];
	}

	const Block &operator()(long x, long y) const//访问多维数组
	{
		return pMap[y * lMapWidth + x];
	}

	Block *GetMapPoint(void)
	{
		return pMap;
	}

	const Block *GetMap(void) const
	{
		return pMap;
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
#pragma once
#include "List.hpp"
#include <string.h>
#include <new>

#define SYMBOL_COUNT 7
#define SYMBOL_STR_LEN 3

class Map
{
private:
	unsigned char *pMap;//地图数组
	char cSymbol[SYMBOL_COUNT][SYMBOL_STR_LEN];//7个字符，每种字符占用3个字节，前两个字节是符号，最后一个是空字符
	const size_t szMapWidth;
	const size_t szMapHight;
public:
	Map(
		const unsigned char *_pMap,
		size_t _szMapWidth,
		size_t _szMapHight,
		const char*_cSymbol) :
		pMap(nullptr),
		cSymbol{0},
		szMapWidth(_szMapWidth),
		szMapHight(_szMapHight)
	{
		pMap = new(std::nothrow) unsigned char[szMapWidth * szMapHight];
		if (pMap == nullptr)
		{
			return;
		}

		memcpy(pMap, _pMap, szMapWidth *szMapHight);
		memcpy(cSymbol, _cSymbol, sizeof(cSymbol));
	}

	Map(const Map &_Map) :pMap(nullptr), cSymbol{0}, szMapWidth(_Map.szMapWidth), szMapHight(_Map.szMapHight)
	{
		pMap = new(std::nothrow) unsigned char[szMapWidth * szMapHight];
		if (pMap == nullptr)
		{
			return;
		}

		memcpy(pMap, _Map.pMap, szMapWidth * szMapHight);
		memcpy(cSymbol, _Map.cSymbol, sizeof(cSymbol));
	}

	Map(Map &&_Map) :pMap(_Map.pMap), cSymbol{0}, szMapWidth(_Map.szMapWidth), szMapHight(_Map.szMapHight)
	{
		memcpy(cSymbol, _Map.cSymbol, sizeof(cSymbol));
		_Map.pMap = nullptr;
	}

	~Map(void)
	{
		delete[] pMap;
	}

	void SetMap(const unsigned char *_pMap)
	{
		memcpy(pMap, _pMap, szMapWidth * szMapHight);
	}

	void SetSymbol(const char *_cSymbol)
	{
		memcpy(cSymbol, _cSymbol, sizeof(cSymbol));
	}

	const char(*GetSymbol(void) const)[SYMBOL_COUNT][SYMBOL_STR_LEN]//返回数组指针
	{
		return &cSymbol;
	}

	size_t GetWidth(void)
	{
		return szMapWidth;
	}

	size_t GetHight(void)
	{
		return szMapHight;
	}

	unsigned char &operator()(size_t x, size_t y)//访问多维数组
	{
		return pMap[y * szMapWidth + x];
	}
};
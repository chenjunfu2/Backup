#pragma once
#include <stddef.h>
#include <algorithm>
//统计分数，最高记录

#define HISTROY_COUNT 10

class Record//记录每一局的分数和最高分数的前十名  分数就是操作步数，越大排名越低
{
public:

	struct File
	{
		uint64_t u64Current;
		uint64_t u64Histroy[HISTROY_COUNT];
	};
private:
	size_t szCurrent;//本局当前分数
	size_t szHistroy[HISTROY_COUNT];//历史分数

public:
	Record(const File &_File) :
		szCurrent(_File.u64Current), szHistroy{0}
	{
		for (long i = 0; i < HISTROY_COUNT; ++i)
		{
			szHistroy[i] = _File.u64Histroy[i];
		}
	}

	const File GetFile(void)
	{
		File RetFile(szCurrent, {0});
		for (long i = 0; i < HISTROY_COUNT; ++i)
		{
			RetFile.u64Histroy[i] = szHistroy[i];
		}
		return RetFile;
	}
public:
	Record(size_t _szCurrent, const size_t *_szpHistroy) :szCurrent(_szCurrent), szHistroy{0}
	{
		if (_szpHistroy != nullptr)
		{
			memcpy(szHistroy, _szpHistroy, sizeof(szHistroy));
			sort();
		}
	}

	~Record(void) = default;

	operator size_t &(void)
	{
		return szCurrent;
	}

	operator size_t(void) const
	{
		return szCurrent;
	}

	size_t &operator[](long lPos)
	{
		return szHistroy[lPos];
	}

	size_t operator[](long lPos) const
	{
		return szHistroy[lPos];
	}

	void sort(void)//0~9 升序排序
	{
		std::sort(std::begin(szHistroy), std::end(szHistroy), 
			std::less<std::remove_reference_t<decltype(*szHistroy)>>());//std::greater降序 std::less升序
	}
};
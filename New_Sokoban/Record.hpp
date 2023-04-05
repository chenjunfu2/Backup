#pragma once
#include <stddef.h>
#include <algorithm>
//统计分数，最高记录

class Record//记录每一局的分数和最高分数的前十名  分数就是操作步数，越大排名越低
{
public:
	static constexpr size_t HISTROY_COUNT = 10;
	static constexpr size_t szUnvalid = (size_t)-1;//用于判断分数是否有效

	struct File
	{
		uint64_t u64Current;
		uint64_t u64Histroy[HISTROY_COUNT];
	};
private:
	size_t szCurrent;//本局当前分数
	size_t szRanking;//当前排名 1 ~ HISTROY_COUNT
	size_t szHistroy[HISTROY_COUNT];//历史分数

	//名次前移
	inline void ForwardRanking(void)
	{
		while (szRanking > 1 && szCurrent < szHistroy[szRanking - 1])
		{
			--szRanking;
		}
	}

	//名次后移
	inline void BackwardRanking(void)
	{
		while (szRanking < HISTROY_COUNT && szCurrent > szHistroy[szRanking - 1])
		{
			++szRanking;
		}
	}

	//升序排序
	void sort(void)
	{
		std::sort(std::begin(szHistroy), std::end(szHistroy),
			std::less<std::remove_reference_t<decltype(*szHistroy)>>());//std::greater降序 std::less升序
	}
public:
	Record(const File &_File) :
		szCurrent(_File.u64Current), szRanking(0), szHistroy{0}
	{
		for (size_t i = 0; i < HISTROY_COUNT; ++i)
		{
			szHistroy[i] = _File.u64Histroy[i];
		}
		//排序
		sort();
		//计算排名
		BackwardRanking();
	}

	const File GetFile(void)
	{
		File RetFile(szCurrent, {0});
		for (size_t i = 0; i < HISTROY_COUNT; ++i)
		{
			RetFile.u64Histroy[i] = szHistroy[i];
		}
		return RetFile;
	}
public:
	Record(size_t _szCurrent, const size_t *_szpHistroy) :szCurrent(_szCurrent), szRanking(0), szHistroy{0}
	{
		if (_szpHistroy != nullptr)
		{
			//拷贝
			memcpy(szHistroy, _szpHistroy, sizeof(szHistroy));
			//排序
			sort();
		}
		else
		{
			std::fill(std::begin(szHistroy), std::end(szHistroy), szUnvalid);//无效分数，初始化为szUnvalid
		}

		//计算排名
		BackwardRanking();
	}

	~Record(void) = default;

	Record &operator++(void)
	{
		//递增
		++szCurrent;
		BackwardRanking();
		return *this;
	}

	Record &operator--(void)
	{
		//递减
		--szCurrent;
		ForwardRanking();
		return *this;
	}

	size_t Histroy(size_t szPos) const
	{
		return szHistroy[szPos];
	}

	size_t CurrentInHistroy(size_t szPos) const
	{
		if (szPos < szRanking - 1)
		{
			return szHistroy[szPos];
		}
		else if (szPos > szRanking - 1)
		{
			return szHistroy[szPos + 1];
		}
		else
		{
			return szCurrent;
		}
	}

	size_t Ranking(void) const
	{
		return szRanking;
	}

	size_t Current(void) const
	{
		return szCurrent;
	}
};
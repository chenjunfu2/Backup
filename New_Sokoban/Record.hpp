#pragma once
#include <stddef.h>
#include <algorithm>
//ͳ�Ʒ�������߼�¼

#define HISTROY_COUNT 10

class Record//��¼ÿһ�ֵķ�������߷�����ǰʮ��  �������ǲ���������Խ������Խ��
{
public:

	struct File
	{
		uint64_t u64Current;
		uint64_t u64Histroy[HISTROY_COUNT];
	};
private:
	size_t szCurrent;//���ֵ�ǰ����
	size_t szHistroy[HISTROY_COUNT];//��ʷ����

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

	void sort(void)//0~9 ��������
	{
		std::sort(std::begin(szHistroy), std::end(szHistroy), 
			std::less<std::remove_reference_t<decltype(*szHistroy)>>());//std::greater���� std::less����
	}
};
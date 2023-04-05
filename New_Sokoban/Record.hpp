#pragma once
#include <stddef.h>
#include <algorithm>
//ͳ�Ʒ�������߼�¼

class Record//��¼ÿһ�ֵķ�������߷�����ǰʮ��  �������ǲ���������Խ������Խ��
{
public:
	static constexpr size_t HISTROY_COUNT = 10;
	static constexpr size_t szUnvalid = (size_t)-1;//�����жϷ����Ƿ���Ч

	struct File
	{
		uint64_t u64Current;
		uint64_t u64Histroy[HISTROY_COUNT];
	};
private:
	size_t szCurrent;//���ֵ�ǰ����
	size_t szRanking;//��ǰ���� 1 ~ HISTROY_COUNT
	size_t szHistroy[HISTROY_COUNT];//��ʷ����

	//����ǰ��
	inline void ForwardRanking(void)
	{
		while (szRanking > 1 && szCurrent < szHistroy[szRanking - 1])
		{
			--szRanking;
		}
	}

	//���κ���
	inline void BackwardRanking(void)
	{
		while (szRanking < HISTROY_COUNT && szCurrent > szHistroy[szRanking - 1])
		{
			++szRanking;
		}
	}

	//��������
	void sort(void)
	{
		std::sort(std::begin(szHistroy), std::end(szHistroy),
			std::less<std::remove_reference_t<decltype(*szHistroy)>>());//std::greater���� std::less����
	}
public:
	Record(const File &_File) :
		szCurrent(_File.u64Current), szRanking(0), szHistroy{0}
	{
		for (size_t i = 0; i < HISTROY_COUNT; ++i)
		{
			szHistroy[i] = _File.u64Histroy[i];
		}
		//����
		sort();
		//��������
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
			//����
			memcpy(szHistroy, _szpHistroy, sizeof(szHistroy));
			//����
			sort();
		}
		else
		{
			std::fill(std::begin(szHistroy), std::end(szHistroy), szUnvalid);//��Ч��������ʼ��ΪszUnvalid
		}

		//��������
		BackwardRanking();
	}

	~Record(void) = default;

	Record &operator++(void)
	{
		//����
		++szCurrent;
		BackwardRanking();
		return *this;
	}

	Record &operator--(void)
	{
		//�ݼ�
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
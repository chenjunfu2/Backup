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
		while (szRanking > 1 && szCurrent < szHistroy[szRanking - 2])
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
		szCurrent(_File.u64Current), szRanking(1), szHistroy{0}
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
	Record(size_t _szCurrent, const size_t *_szpHistroy, size_t szHistroyCount = HISTROY_COUNT) : szCurrent(_szCurrent), szRanking(1), szHistroy{0}
	{
		if (_szpHistroy == nullptr)//ȫ����Ч
		{
			std::fill(std::begin(szHistroy), std::end(szHistroy), szUnvalid);//��ʼ��ΪszUnvalid
			return;
		}
		
		//������Ч����
		size_t szCount = szHistroyCount >= HISTROY_COUNT ? HISTROY_COUNT : szHistroyCount;
		for (size_t i = 0; i < szCount; ++i)
		{
			szHistroy[i] = _szpHistroy[i];
		}
		//��Ч���֣���ʼ��ΪszUnvalid
		std::fill(std::begin(szHistroy) + szCount, std::end(szHistroy), szUnvalid);

		//����
		sort();

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

	//��ʷ��¼
	size_t HistroyList(size_t szPos) const
	{
		return szHistroy[szPos];
	}

	//���а�����1 ~ HISTROY_COUNT
	size_t RankingList(size_t szPos) const
	{
		if (szPos < szRanking)
		{
			return szHistroy[szPos - 1];
		}
		else if (szPos > szRanking)
		{
			return szHistroy[szPos - 2];
		}
		else
		{
			return szCurrent;
		}
	}

	//��ǰ���� 1 ~ HISTROY_COUNT
	size_t Ranking(void) const
	{
		return szRanking;
	}

	size_t Current(void) const
	{
		return szCurrent;
	}
};
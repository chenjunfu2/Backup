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
	size_t szRanking;//��ǰ���� 1 ~ HISTROY_COUNT
	size_t szHistroy[HISTROY_COUNT];//��ʷ����

	//��������
	void CalculateRanking(void)
	{
		while (szRanking >= 1 && szRanking <= HISTROY_COUNT)//1 ~ HISTROY_COUNT��
		{
			if (szCurrent > szHistroy[szRanking - 1])//���κ���
			{
				++szRanking;
			}
			else if (szCurrent < szHistroy[szRanking - 1])//����ǰ��
			{
				--szRanking;
			}
			else//������ȣ�����
			{
				break;
			}
		}

		if (szRanking < 1)//�ȵ�һ�����;��ǵ�һ��
		{
			szRanking = 1;
		}
		else if (szRanking > HISTROY_COUNT)//�����һ�����߾������һ��
		{
			szRanking = HISTROY_COUNT;
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
		CalculateRanking();
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
			std::fill(std::begin(szHistroy), std::end(szHistroy), (size_t)-1);//��Ч��������ʼ��Ϊ-1
		}

		//��������
		CalculateRanking();
	}

	~Record(void) = default;

	Record &operator++(void)
	{
		//����
		++szCurrent;
		//��������
		CalculateRanking();

		return *this;
	}

	Record &operator--(void)
	{
		//�ݼ�
		--szCurrent;
		//��������
		CalculateRanking();

		return *this;
	}

	bool IsValidHistroy(size_t szPos) const
	{
		return szHistroy[szPos] != (size_t)-1;//�жϷ����Ƿ���Ч
	}

	size_t Histroy(size_t szPos) const
	{
		return szHistroy[szPos];
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
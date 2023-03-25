#pragma once
#include <stddef.h>

/*
//��ͼ����
0���յ�"  "
1��ǽ��"��"
2��Ŀ�ĵ�"��" "��"
3������"��"
4��������Ŀ�ĵ�"��" "��"
*/

class Map//�����ڹ��������ڴ�������ͷ�
{
public:
	enum Block
	{
		Blank = 0,		//0���յ�"  "
		Wall = 1,		//1��ǽ��"��"
		Destn = 2,		//2��Ŀ�ĵ�"��" "��"
		BoxInBlank = 3,	//3������"��"
		BoxInDestn = 4,	//4��������Ŀ�ĵ�"��" "��"
	};
private:
	Block *pMap;//��ͼ����
	long lMapWidth;//��ͼ���
	long lMapHight;

	long lAllBoxNum;//��������
	long lDestnBoxNum;//���յ��������
public:
	Map(Block *_pMap, long _szMapWidth, long _szMapHight) :
		pMap(_pMap), lMapWidth(_szMapWidth), lMapHight(_szMapHight)
	{
		for (long long i = 0; i < lMapWidth * lMapHight; ++i)
		{
			if (pMap[i] == BoxInBlank)
			{
				++lAllBoxNum;//��������
			}
			else if (pMap[i] == BoxInDestn)
			{
				++lAllBoxNum;
				++lDestnBoxNum;//�������յ��������
			}
		}
	}

	Map(const Map &_Map) :
		pMap(_Map.pMap), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{}

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

	~Map(void) = default;

	Block &operator()(long x, long y) const//���ʶ�ά����
	{
		return pMap[y * lMapWidth + x];
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

	long &DestnBoxNum(void)
	{
		return lDestnBoxNum;
	}
};
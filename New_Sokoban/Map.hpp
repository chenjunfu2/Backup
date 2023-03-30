#pragma once
#include <stddef.h>
#include <new>
#include <string.h>

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
	enum Block :unsigned char
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
	Map(Block *_pMap, long _lMapWidth, long _lMapHight, bool bMovePointer = false) :
		pMap(_pMap), lMapWidth(_lMapWidth), lMapHight(_lMapHight), lAllBoxNum(0), lDestnBoxNum(0)
	{
		//�����ͼָ�벻���ƶ������ģ�������ڴ����Ϳ���
		if (!bMovePointer)
		{
			pMap = new(std::nothrow) Block[lMapWidth * lMapHight];
			if (pMap == nullptr)
			{
				return;
			}
			//������ͼ
			memcpy(pMap, _pMap, sizeof(Block) * lMapWidth * lMapHight);
		}

		//����������Ŀ
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
		pMap(nullptr), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		pMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (pMap == nullptr)
		{
			return;
		}
		//������ͼ
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
		//������ͼ
		memcpy(pMap, _Map.pMap, sizeof(Block) * lMapWidth * lMapHight);

		return *this;
	}

	Block &operator()(long x, long y)//���ʶ�ά����
	{
		return pMap[y * lMapWidth + x];
	}

	const Block &operator()(long x, long y) const//���ʶ�ά����
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
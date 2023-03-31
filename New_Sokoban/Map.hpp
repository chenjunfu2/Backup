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

	struct File
	{
		uint64_t u64MapWidth;//��ͼ��
		uint64_t u64MapHight;//��ͼ��
		uint64_t u64AllBoxNum;//��������
		uint64_t u64DestnBoxNum;//���յ��������
		Block *enpMapData;//�䳤��ͼ���ݼ�
	};
private:
	Block *enpMap;//��ͼ����
	long lMapWidth;//��ͼ���
	long lMapHight;

	long lAllBoxNum;//��������
	long lDestnBoxNum;//���յ��������
public:
	Map(const File &_File) :
		enpMap(_File.enpMapData), lMapWidth(_File.u64MapWidth), lMapHight(_File.u64MapHight), 
		lAllBoxNum(_File.u64AllBoxNum), lDestnBoxNum(_File.u64DestnBoxNum)
	{}

	const File GetFile(void) const
	{
		return File(lMapWidth, lMapHight, lAllBoxNum, lDestnBoxNum, enpMap);
	}
public:
	Map(Block *_pMap, long _lMapWidth, long _lMapHight) :
		enpMap(nullptr), lMapWidth(_lMapWidth), lMapHight(_lMapHight), lAllBoxNum(0), lDestnBoxNum(0)
	{
		enpMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (enpMap == nullptr)
		{
			return;
		}
		//������ͼ
		memcpy(enpMap, _pMap, sizeof(Block) * lMapWidth * lMapHight);

		//����������Ŀ
		for (long long i = 0; i < lMapWidth * lMapHight; ++i)
		{
			if (enpMap[i] == BoxInBlank)
			{
				++lAllBoxNum;//��������
			}
			else if (enpMap[i] == BoxInDestn)
			{
				++lAllBoxNum;
				++lDestnBoxNum;//�������յ��������
			}
		}
	}

	Map(const Map &_Map) :
		enpMap(nullptr), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		enpMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (enpMap == nullptr)
		{
			return;
		}
		//������ͼ
		memcpy(enpMap, _Map.enpMap, sizeof(Block) * lMapWidth * lMapHight);
	}

	Map(Map &&_Map) :
		enpMap(_Map.enpMap), lMapWidth(_Map.lMapWidth), lMapHight(_Map.lMapHight),
		lAllBoxNum(_Map.lAllBoxNum), lDestnBoxNum(_Map.lDestnBoxNum)
	{
		_Map.enpMap = nullptr;
		_Map.lMapWidth = 0;
		_Map.lMapHight = 0;
		_Map.lAllBoxNum = 0;
		_Map.lDestnBoxNum = 0;
	}

	~Map(void)
	{
		delete[] enpMap;
		enpMap = nullptr;
	}

	Map &operator=(Map &&_Map)
	{
		enpMap = _Map.enpMap;
		lMapWidth = _Map.lMapWidth;
		lMapHight = _Map.lMapHight;
		lAllBoxNum = _Map.lAllBoxNum;
		lDestnBoxNum = _Map.lDestnBoxNum;

		_Map.enpMap = nullptr;
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

		delete[] enpMap;
		enpMap = new(std::nothrow) Block[lMapWidth * lMapHight];
		if (enpMap == nullptr)
		{
			return *this;
		}
		//������ͼ
		memcpy(enpMap, _Map.enpMap, sizeof(Block) * lMapWidth * lMapHight);

		return *this;
	}

	Block &operator()(long x, long y)//���ʶ�ά����
	{
		return enpMap[y * lMapWidth + x];
	}

	const Block &operator()(long x, long y) const//���ʶ�ά����
	{
		return enpMap[y * lMapWidth + x];
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
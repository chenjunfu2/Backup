#pragma once
#include <stddef.h>
#include <stdint.h>

/*
//��ͼ����
0���ˡ�
1������Ŀ�ĵء�
*/

class Player
{
public:
	enum Block :unsigned char
	{
		PlayerInBlank = 0,	//0���ˡ�
		PlayerInDestn = 1,	//1������Ŀ�ĵء�
	};

	struct File
	{
		uint64_t u64X;//�������x
		uint64_t u64Y;//�������y
		Block enPlayerStatus;//���״̬
	};
public:
	//�ڵ�ͼ�е�λ��
	long x;
	long y;
	Block enPlayerStatus;//״̬
public:
	Player(const File &_File) :
		x(_File.u64X), y(_File.u64Y), enPlayerStatus(_File.enPlayerStatus)
	{}

	const File GetFile(void)
	{
		return File(x, y, enPlayerStatus);
	}
public:
	Player(long _x, long _y, Block _enPlayerStatus = PlayerInBlank) :x(_x), y(_y), enPlayerStatus(_enPlayerStatus)
	{}
	~Player(void) = default;
};
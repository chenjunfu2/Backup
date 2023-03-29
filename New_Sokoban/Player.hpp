#pragma once
#include <stddef.h>


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
public:
	//�ڵ�ͼ�е�λ��
	long x;
	long y;
	Block enPlayerStatus;//״̬

	Player(long _x, long _y, Block _enPlayerStatus = PlayerInBlank) :x(_x), y(_y), enPlayerStatus(_enPlayerStatus)
	{}
	~Player(void) = default;
};
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
	enum Block
	{
		PlayerInBlank = 0,	//0���ˡ�
		PlayerInDestn = 1,	//1������Ŀ�ĵء�
	};
public:
	//�ڵ�ͼ�е�λ��
	long x;
	long y;
	Block enPlayer;//״̬
};
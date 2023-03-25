#pragma once
//#include "Console.hpp"


/*
�������ҷ����
0xE0��ʼ���
	 0x48
0x4B 0x50 0x4D

���ϰ�������0x00��ͷ����0xE0��ͷ

#include <conio.h>
#include <stdio.h>

int main(void)
{
	while (true)
	{
		printf("0x%02X ", _getch());
	}

	return 0;
}
*/

#include <conio.h>
#include <string.h>
#include <limits.h>

class Interaction//�û�����
{
public:
	struct Func
	{
		long (*pFunc)(void *);
		void *pParam;
	};

	enum LeadCode
	{
		Code_00 = 0,
		Code_E0 = 1,
		Code_NL = 2,
	};
private:
	Func fRegisterTable[3][UCHAR_MAX + 1];//ǰ���ֽڣ�������
public:
	Interaction(void)
	{
		memset(fRegisterTable, 0, sizeof(fRegisterTable));
	}
	~Interaction(void) = default;

	void RegisterKey(unsigned char lKeyCode, const Func &fFunc)
	{
		RegisterKey(Code_NL, lKeyCode, fFunc);//ת������
	}

	void RegisterKey(LeadCode enLeadCode, unsigned char lKeyCode, const Func &fFunc)
	{
		fRegisterTable[enLeadCode][lKeyCode] = fFunc;
	}

	void UnRegisterKey(unsigned char lKeyCode)
	{
		UnRegisterKey(Code_NL, lKeyCode);//ת������
	}

	void UnRegisterKey(LeadCode enLeadCode, unsigned char lKeyCode)
	{
		fRegisterTable[enLeadCode][lKeyCode] = {0};
	}

	void Reset(void)
	{
		memset(fRegisterTable, 0, sizeof(fRegisterTable));
	}

	long Loop(void)
	{
		int iInput;
		LeadCode enCode;
		while (true)
		{
			iInput = _getch();
			
			if (iInput == 0x00)//ת��
			{
				enCode = Code_00;
				iInput = _getch();
			}
			else if (iInput == 0xE0)//ת��
			{
				enCode = Code_E0;
				iInput = _getch();
			}
			else//��������
			{
				enCode = Code_NL;
			}

			//��ȡ����
			Func fFun = fRegisterTable[enCode][iInput];
			if (fFun.pFunc == nullptr)
			{
				continue;
			}

			//��Ϊ�������
			long lRet = fFun.pFunc(fFun.pParam);//��Ϊ0��Ϊ�쳣����
			if (lRet != 0)
			{
				return lRet;
			}
		}
	}

};
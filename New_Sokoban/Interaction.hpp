#pragma once
//#include "Console.hpp"


/*
上下左右方向键
0xE0开始后跟
	 0x48
0x4B 0x50 0x4D

复合按键不是0x00开头就是0xE0开头

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

class Interaction//用户交互
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
	Func fRegisterTable[3][UCHAR_MAX + 1];//前导字节，按键码
public:
	Interaction(void)
	{
		memset(fRegisterTable, 0, sizeof(fRegisterTable));
	}
	~Interaction(void) = default;

	void RegisterKey(unsigned char lKeyCode, const Func &fFunc)
	{
		RegisterKey(Code_NL, lKeyCode, fFunc);//转发调用
	}

	void RegisterKey(LeadCode enLeadCode, unsigned char lKeyCode, const Func &fFunc)
	{
		fRegisterTable[enLeadCode][lKeyCode] = fFunc;
	}

	void UnRegisterKey(unsigned char lKeyCode)
	{
		UnRegisterKey(Code_NL, lKeyCode);//转发调用
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
			
			if (iInput == 0x00)//转义
			{
				enCode = Code_00;
				iInput = _getch();
			}
			else if (iInput == 0xE0)//转义
			{
				enCode = Code_E0;
				iInput = _getch();
			}
			else//正常按键
			{
				enCode = Code_NL;
			}

			//获取函数
			Func fFun = fRegisterTable[enCode][iInput];
			if (fFun.pFunc == nullptr)
			{
				continue;
			}

			//不为空则调用
			long lRet = fFun.pFunc(fFun.pParam);//不为0即为异常返回
			if (lRet != 0)
			{
				return lRet;
			}
		}
	}

};
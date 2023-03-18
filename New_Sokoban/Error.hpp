#pragma once

#include <stdio.h>


class Error
{
private:
	bool bIsError;
	unsigned long ulErrCode;
	const char *pErrFuncName;
	const char *pErrReason;
public:
	Error(
		bool _bIsError,
		unsigned long _ulErrCode = 0,
		const char *_pErrFuncName = nullptr,
		const char *_pErrReason = nullptr)://注意！设置的错误函数名称和原因应该为常量字符串
		bIsError(_bIsError),
		ulErrCode(_ulErrCode),
		pErrFuncName(_pErrFuncName),
		pErrReason(_pErrReason)
	{
		if (pErrFuncName == nullptr)
		{
			pErrFuncName = "未知函数名";
		}

		if (pErrReason == nullptr)
		{
			pErrReason = "未知错误原因";
		}
	}
	~Error(void) = default;

	unsigned long GetErrCode(void)
	{
		return ulErrCode;
	}

	void SetErrCode(unsigned long _ulErrCode)
	{
		ulErrCode = _ulErrCode;
	}

	const char *GetErrFuncName(void)
	{
		return pErrFuncName;
	}

	void SetErrFuncName(const char *_pErrFuncName)//注意！设置的错误函数名称应该为常量字符串
	{
		pErrFuncName = _pErrFuncName;
	}

	const char *GetErrReason(void)
	{
		return pErrReason;
	}

	void SetErrReason(const char *_pErrReason)//注意！设置的错误原因应该为常量字符串
	{
		pErrReason = _pErrReason;
	}

	operator bool(void)//默认转换以便直接通过if判断
	{
		return bIsError;
	}

	int PrintError(FILE *fOut)
	{
		return fprintf(fOut, "出现错误! [错误函数名:%s]\n[错误码:%ld 错误原因%s]", pErrFuncName, ulErrCode, pErrReason);
	}

};
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
		const char *_pErrReason = nullptr)://ע�⣡���õĴ��������ƺ�ԭ��Ӧ��Ϊ�����ַ���
		bIsError(_bIsError),
		ulErrCode(_ulErrCode),
		pErrFuncName(_pErrFuncName),
		pErrReason(_pErrReason)
	{
		if (pErrFuncName == nullptr)
		{
			pErrFuncName = "δ֪������";
		}

		if (pErrReason == nullptr)
		{
			pErrReason = "δ֪����ԭ��";
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

	void SetErrFuncName(const char *_pErrFuncName)//ע�⣡���õĴ���������Ӧ��Ϊ�����ַ���
	{
		pErrFuncName = _pErrFuncName;
	}

	const char *GetErrReason(void)
	{
		return pErrReason;
	}

	void SetErrReason(const char *_pErrReason)//ע�⣡���õĴ���ԭ��Ӧ��Ϊ�����ַ���
	{
		pErrReason = _pErrReason;
	}

	operator bool(void)//Ĭ��ת���Ա�ֱ��ͨ��if�ж�
	{
		return bIsError;
	}

	int PrintError(FILE *fOut)
	{
		return fprintf(fOut, "���ִ���! [��������:%s]\n[������:%ld ����ԭ��%s]", pErrFuncName, ulErrCode, pErrReason);
	}

};
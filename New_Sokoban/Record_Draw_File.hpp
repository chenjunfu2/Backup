#pragma once
#include "Endian.hpp"

#include "Draw.hpp"
//��߼�¼�������ݶ�д��
class Record_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record_Draw::File &stControlFile)
	{
		for (size_t i = 0; i < Record_Draw::RECORD_SYMBOL_COUNT; ++i)
		{
			size_t szStrLength = stControlFile.stSymbol[i].strOutput.length();//��ȡ�ַ������ȣ�������ĩβ\0

			if (!WriteFileWithGeneralEndian(fpWrite, szStrLength) ||//д���ַ������ȣ�������ĩβ\0
				!WriteFileWithGeneralEndian(fpWrite, stControlFile.stSymbol[i].strOutput.data(), szStrLength) ||//д���ַ�����������ĩβ\0
				!WriteFileWithGeneralEndian(fpWrite, stControlFile.stSymbol[i].ucNumbColor) ||
				!WriteFileWithGeneralEndian(fpWrite, stControlFile.stSymbol[i].ucTextColor))
			{
				return false;
			}
		}
		
		return true;
	}

	static bool ReadFile(FILE *fpRead, Record_Draw::File &stControlFile)
	{
		for (size_t i = 0; i < Record_Draw::RECORD_SYMBOL_COUNT; ++i)
		{
			size_t szStrLength;
			if (!ReadFileWithGeneralEndian(fpRead, szStrLength))//��ȡ�ַ������ȣ�������ĩβ\0
			{
				return false;
			}
			stControlFile.stSymbol[i].strOutput.resize(szStrLength);//Ԥ�����ڴ沢�ı�string��С���������ֱ�Ӷ���

			if (!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].strOutput.data(), szStrLength) ||//�����ַ�����������ĩβ\0
				!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].ucNumbColor) ||
				!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].ucTextColor))
			{
				return false;
			}
		}

		return true;
	}
};
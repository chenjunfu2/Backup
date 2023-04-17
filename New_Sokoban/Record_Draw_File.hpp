#pragma once
#include "Endian.hpp"

#include "Draw.hpp"
//最高纪录符号数据读写类
class Record_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record_Draw::File &stControlFile)
	{
		for (size_t i = 0; i < Record_Draw::RECORD_SYMBOL_COUNT; ++i)
		{
			size_t szStrLength = stControlFile.stSymbol[i].strOutput.length();//获取字符串长度，不包括末尾\0

			if (!WriteFileWithGeneralEndian(fpWrite, szStrLength) ||//写入字符串长度，不包括末尾\0
				!WriteFileWithGeneralEndian(fpWrite, stControlFile.stSymbol[i].strOutput.data(), szStrLength) ||//写入字符串，不包括末尾\0
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
			if (!ReadFileWithGeneralEndian(fpRead, szStrLength))//读取字符串长度，不包括末尾\0
			{
				return false;
			}
			stControlFile.stSymbol[i].strOutput.resize(szStrLength);//预分配内存并改变string大小，方便后续直接读入

			if (!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].strOutput.data(), szStrLength) ||//读入字符串，不包括末尾\0
				!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].ucNumbColor) ||
				!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].ucTextColor))
			{
				return false;
			}
		}

		return true;
	}
};
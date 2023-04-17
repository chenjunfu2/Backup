#pragma once
#include "Endian.hpp"

//地图符号数据读写类
#include "Draw.hpp"
class Map_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Map_Draw::File &stMapDrawFile)
	{
		for (size_t i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
		{
			//写入符号类的所有成员
			if (!WriteFileWithGeneralEndian(fpWrite, stMapDrawFile.stSymbol[i].cStr) ||
				!WriteFileWithGeneralEndian(fpWrite, stMapDrawFile.stSymbol[i].ucColor))
			{
				return false;
			}
		}

		return true;
	}

	static bool ReadFile(FILE *fpRead, Map_Draw::File &stMapDrawFile)
	{
		for (size_t i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
		{
			//读取符号类的所有成员
			if (!ReadFileWithGeneralEndian(fpRead, stMapDrawFile.stSymbol[i].cStr) ||
				!ReadFileWithGeneralEndian(fpRead, stMapDrawFile.stSymbol[i].ucColor))
			{
				return false;
			}
		}

		return true;
	}
};
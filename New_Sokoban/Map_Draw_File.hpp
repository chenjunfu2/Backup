#pragma once
#include "Endian.hpp"

//��ͼ�������ݶ�д��
#include "Draw.hpp"
class Map_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Map_Draw::File &stMapDrawFile)
	{
		for (size_t i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
		{
			//д�����������г�Ա
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
			//��ȡ����������г�Ա
			if (!ReadFileWithGeneralEndian(fpRead, stMapDrawFile.stSymbol[i].cStr) ||
				!ReadFileWithGeneralEndian(fpRead, stMapDrawFile.stSymbol[i].ucColor))
			{
				return false;
			}
		}

		return true;
	}
};
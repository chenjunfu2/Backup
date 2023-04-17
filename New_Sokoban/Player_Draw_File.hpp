#pragma once
#include "Endian.hpp"

//��ҷ������ݶ�д��
#include "Draw.hpp"
class Player_Draw_File
{
	static bool WriteFile(FILE *fpWrite, const Player_Draw::File &stPlayerDrawFile)
	{
		for (size_t i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
		{
			//д�����������г�Ա
			if (!WriteFileWithGeneralEndian(fpWrite, stPlayerDrawFile.stSymbol[i].cStr) ||
				!WriteFileWithGeneralEndian(fpWrite, stPlayerDrawFile.stSymbol[i].ucColor))
			{
				return false;
			}
		}

		return true;
	}

	static bool ReadFile(FILE *fpRead, Player_Draw::File &stPlayerDrawFile)
	{
		for (size_t i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
		{
			//��ȡ����������г�Ա
			if (!ReadFileWithGeneralEndian(fpRead, stPlayerDrawFile.stSymbol[i].cStr) ||
				!ReadFileWithGeneralEndian(fpRead, stPlayerDrawFile.stSymbol[i].ucColor))
			{
				return false;
			}
		}

		return true;
	}
};
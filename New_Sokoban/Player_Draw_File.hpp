#pragma once
#include "Endian.hpp"

//玩家符号数据读写类
#include "Draw.hpp"
class Player_Draw_File
{
	static bool WriteFile(FILE *fpWrite, const Player_Draw::File &stPlayerDrawFile)
	{
		for (size_t i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
		{
			//写入符号类的所有成员
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
			//读取符号类的所有成员
			if (!ReadFileWithGeneralEndian(fpRead, stPlayerDrawFile.stSymbol[i].cStr) ||
				!ReadFileWithGeneralEndian(fpRead, stPlayerDrawFile.stSymbol[i].ucColor))
			{
				return false;
			}
		}

		return true;
	}
};
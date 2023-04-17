#pragma once
#include "Endian.hpp"

//游戏控制数据读写类
#include "Game_Control.hpp"
class Game_Control_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Game_Control::File &stControlFile)
	{
		if (!WriteFileWithGeneralEndian(fpWrite, (uint64_t)stControlFile.csOperate.GetNodeNum()))
		{
			return false;
		}

		for (auto const &i : stControlFile.csOperate)
		{
			if (!WriteFileWithGeneralEndian(fpWrite, i.cXMove) ||
				!WriteFileWithGeneralEndian(fpWrite, i.cYMove) ||
				!WriteFileWithGeneralEndian(fpWrite, i.ucCode))
			{
				return false;
			}
		}

		if (!WriteFileWithGeneralEndian(fpWrite, (uint64_t)stControlFile.csUndo.GetNodeNum()))
		{
			return false;
		}

		for (auto const &i : stControlFile.csUndo)
		{
			if (!WriteFileWithGeneralEndian(fpWrite, i.cXMove) ||
				!WriteFileWithGeneralEndian(fpWrite, i.cYMove) ||
				!WriteFileWithGeneralEndian(fpWrite, i.ucCode))
			{
				return false;
			}
		}

		return true;
	}

	static bool ReadFile(FILE *fpRead, Game_Control::File &stControlFile)
	{
		uint64_t u64OperateNodeNum;
		if (!ReadFileWithGeneralEndian(fpRead, u64OperateNodeNum))
		{
			return false;
		}

		for (uint64_t i = 0; i < u64OperateNodeNum; ++i)
		{
			Game_Control::Move_Data stMoveData;
			if (!ReadFileWithGeneralEndian(fpRead, stMoveData.cXMove) ||
				!ReadFileWithGeneralEndian(fpRead, stMoveData.cYMove) ||
				!ReadFileWithGeneralEndian(fpRead, stMoveData.ucCode) ||
				!stControlFile.csOperate.InsertTail(stMoveData))
			{
				return false;
			}
		}


		uint64_t u64UndoNodeNum;
		if (!ReadFileWithGeneralEndian(fpRead, u64UndoNodeNum))
		{
			return false;
		}

		for (uint64_t i = 0; i < u64UndoNodeNum; ++i)
		{
			Game_Control::Move_Data stMoveData;
			if (!ReadFileWithGeneralEndian(fpRead, stMoveData.cXMove) ||
				!ReadFileWithGeneralEndian(fpRead, stMoveData.cYMove) ||
				!ReadFileWithGeneralEndian(fpRead, stMoveData.ucCode) ||
				!stControlFile.csUndo.InsertTail(stMoveData))
			{
				return false;
			}
		}

		return true;
	}
};
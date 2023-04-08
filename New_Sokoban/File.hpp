#pragma once
#include <stdint.h>
//配置文件读写、地图文件读写、关卡回放与恢复文件读写

#include "Endian.hpp"

#include <new>

//文件头验证类
class File_Head_Verify
{
public:
	enum Type
	{
		Unknow = 0,
		Sokoban_Level,
		Sokoban_Config,
		Sokoban_PlayBack,
		Sokoban_Progress,
	};
public:
	static Type GetFileType(FILE * fpRead)
	{
		char cReadStr[32];//0~30 输入字符 31 '\0'
		for (long i = 0; i < 31; ++i)
		{
			cReadStr[i] = fgetc(fpRead);
			if (cReadStr[i] == '\0')
			{
				break;
			}
		}
		cReadStr[31] = '\0';

		//比较开头
		constexpr char cBaseStr[] = "Sokoban_";
		constexpr size_t cBaseStrLen = sizeof(cBaseStr) - 1;
		static_assert(sizeof(cBaseStr) < sizeof(cReadStr) - 1);

		if (strncmp(cReadStr, cBaseStr, cBaseStrLen) != 0)
		{
			return Unknow;
		}
		
		//比较接下来的内容
		switch (cReadStr[cBaseStrLen])
		{
		case 'L':
			if (strcmp("Level" + 1, &cReadStr[cBaseStrLen + 1]) == 0)
			{
				return Sokoban_Level;
			}
			break;
		case 'C':
			if (strcmp("Config" + 1, &cReadStr[cBaseStrLen + 1]) == 0)
			{
				return Sokoban_Config;
			}
			break;
		case 'P':
			if (strcmp("PlayBack" + 1, &cReadStr[cBaseStrLen + 1]) == 0)
			{
				return Sokoban_PlayBack;
			}
			else if (strcmp("Progress" + 1, &cReadStr[cBaseStrLen + 1]) == 0)
			{
				return Sokoban_Progress;
			}
			break;
		default:
			return Unknow;
			break;
		}

		return Unknow;
	}
};

//玩家数据读写类
#include "Player.hpp"
class Player_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Player::File &stPlayerFile)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, stPlayerFile.u64X);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, stPlayerFile.u64Y);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (unsigned char &)stPlayerFile.enPlayerStatus);
		return bSuccess;

		static_assert(sizeof(unsigned char &) == sizeof(stPlayerFile.enPlayerStatus));//确保类型大小一致，不会出现UB
	}

	static bool ReadFile(FILE *fpRead, Player::File &stPlayerFile)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stPlayerFile.u64X);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stPlayerFile.u64Y);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, (unsigned char &)stPlayerFile.enPlayerStatus);
		return bSuccess;

		static_assert(sizeof(unsigned char &) == sizeof(stPlayerFile.enPlayerStatus));//确保类型大小一致，不会出现UB
	}

};

//玩家符号数据读写类
#include "Draw.hpp"
class Player_Draw_File
{
	static bool WriteFile(FILE *fpWrite, const Player_Draw::File &stPlayerDrawFile)
	{
		for (long i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
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
		for (long i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
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

//地图数据读写类
#include "Map.hpp"
class Map_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Map::File &stMapFile)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, stMapFile.u64MapWidth);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, stMapFile.u64MapHight);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, stMapFile.u64AllBoxNum);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, stMapFile.u64DestnBoxNum);
		bSuccess = bSuccess && stMapFile.enpMapData != nullptr;//判断是否为空
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (unsigned char*)stMapFile.enpMapData, stMapFile.u64MapWidth * stMapFile.u64MapHight);//写入地图数据集
		return bSuccess;

		static_assert(sizeof(unsigned char) == sizeof(*stMapFile.enpMapData));//确保类型大小一致，不会出现UB
	}

	static bool ReadFile(FILE *fpRead, Map::File &stMapFile)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64MapWidth);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64MapHight);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64AllBoxNum);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64DestnBoxNum);
		bSuccess = bSuccess && (stMapFile.enpMapData = new(std::nothrow) Map::Block[stMapFile.u64MapWidth * stMapFile.u64MapHight]) != nullptr;//分配内存
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, (unsigned char *)stMapFile.enpMapData, stMapFile.u64MapWidth * stMapFile.u64MapHight);//读取地图数据集
		return bSuccess;

		static_assert(sizeof(unsigned char) == sizeof(*stMapFile.enpMapData));//确保类型大小一致，不会出现UB
	}
};

//地图符号数据读写类
#include "Draw.hpp"
class Map_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Map_Draw::File &stMapDrawFile)
	{
		for (long i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
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
		for (long i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
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

//最高纪录数据读写类
#include "Record.hpp"
class Record_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record::File &stControlFile)
	{
		
		
		
	}

	static bool ReadFile(FILE *fpRead, Record::File &stControlFile)
	{
		
		
		
		
		
	}
};

//最高纪录符号数据读写类
class Record_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record_Draw::File &stControlFile)
	{}

	static bool ReadFile(FILE *fpRead, Record_Draw::File &stControlFile)
	{}
};


//游戏控制数据读写类
#include "Control.hpp"
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


#include "Level.hpp"
//关卡文件
class Level_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_Level";
public:
	//玩家、地图、最高纪录(步数越少越高) Highest_Record







};

#include "Playback.hpp"
//回放文件
class Playback_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_PlayBack";
public:





};


#include "Progress.hpp"
//游戏进度文件（残局文件）
class Progress_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_Progress";
public:







};


#include "Config.hpp"
//游戏配置文件
class Config_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_Config";
public:






};
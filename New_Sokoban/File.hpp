#pragma once
#include <stdint.h>
//配置文件读写、地图文件读写、关卡回放与恢复文件读写

#include "Endian.hpp"

#include <new>

//文件头类
class File_Head
{
public:
	enum Type :unsigned char
	{
		Sokoban_Unknow = 0,
		Sokoban_Level,
		Sokoban_Config,
		Sokoban_PlayBack,
		Sokoban_Progress,
	};

	struct File_Info
	{
		Type enFileType;
		size_t szFileVersion;
	};
private:
	static inline const char *const FileHeadStr[] = 
	{
		"Sokoban_Unknow",
		"Sokoban_Level",
		"Sokoban_PlayBack",
		"Sokoban_Progress",
		"Sokoban_Config"
	};

	static constexpr size_t READ_STRLEN = 32;
public:
	static bool ReadFile(FILE *fpRead, File_Info &stFileInfo)
	{
		stFileInfo = {Sokoban_Unknow,0};//重置

		char cReadStr[READ_STRLEN];//0~30 输入字符 31 '\0'
		for (size_t i = 0; i < READ_STRLEN - 1; ++i)
		{
			cReadStr[i] = fgetc(fpRead);
			if (cReadStr[i] == '\0')
			{
				break;
			}
		}
		cReadStr[READ_STRLEN - 1] = '\0';

		//比较前缀
		static constexpr char cBaseStr[] = "Sokoban_";
		static constexpr size_t cBaseStrLen = sizeof(cBaseStr) - 1;
		static_assert(sizeof(cBaseStr) < sizeof(cReadStr) - 1);

		if (strncmp(cReadStr, cBaseStr, cBaseStrLen) != 0)
		{
			return false;
		}

		//比较后缀，通过与列表项依次比较获得实际文件类型枚举
		for (Type i = Sokoban_Level; i <= Sokoban_Config; i = (Type)(i + 1))
		{
			if (strcmp(&FileHeadStr[i][cBaseStrLen], &cReadStr[cBaseStrLen]) == 0)
			{
				stFileInfo.enFileType = i;
			}
		}

		//如果都不属于则返回失败
		if (stFileInfo.enFileType == Sokoban_Unknow)
		{
			return false;
		}

		//读入版本号
		uint64_t u64Read;
		if (!ReadFileWithGeneralEndian(fpRead, u64Read))
		{
			return false;
		}
		stFileInfo.szFileVersion = u64Read;

		return true;
	}

	static bool WriteFile(FILE *fpWrite, const File_Info &stFileInfo)
	{
		//写入类型字符串
		if (!WriteFileWithGeneralEndian(fpWrite, FileHeadStr[stFileInfo.enFileType],
			strlen(FileHeadStr[stFileInfo.enFileType])))//不写入末尾\0
		{
			return false;
		}

		//写入版本号
		uint64_t u64Write = stFileInfo.szFileVersion;
		if (!WriteFileWithGeneralEndian(fpWrite, u64Write))
		{
			return false;
		}
		
		return true;
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

//最高纪录数据读写类
#include "Record.hpp"
class Record_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record::File &stControlFile)
	{
		if (!WriteFileWithGeneralEndian(fpWrite, stControlFile.u64Current) ||
			!WriteFileWithGeneralEndian(fpWrite, stControlFile.u64Histroy))
		{
			return false;
		}
		
		return true;
	}

	static bool ReadFile(FILE *fpRead, Record::File &stControlFile)
	{
		if (!ReadFileWithGeneralEndian(fpRead, stControlFile.u64Current) ||
			!ReadFileWithGeneralEndian(fpRead, stControlFile.u64Histroy))
		{
			return false;
		}

		return true;
	}
};

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


/*
以下所有文件读写例程都添加一个文件版本号
写入时写入版本号
读取时通过版本号调用该版本对应的读取函数，其余缺失的数据项使用默认行为

如果版本号比当前程序最高版本号还高，则报错无法读取，否则读取并升级文件（如果版本低于当前最高版本的话）

每个版本都有自己对应的读写函数
*/

#include "Level.hpp"
//关卡文件
class Level_File
{
private:

public:
	//玩家、地图、最高纪录(步数越少越高) Highest_Record







};

#include "Playback.hpp"
//回放文件
class Playback_File
{
private:

public:





};


#include "Progress.hpp"
//游戏进度文件（残局文件）
class Progress_File
{
private:

public:







};


#include "Config.hpp"
//游戏配置文件
class Config_File
{
private:

public:






};
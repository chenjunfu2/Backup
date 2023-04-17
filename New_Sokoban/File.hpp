#pragma once
#include <stdint.h>
//配置文件读写、地图文件读写、关卡回放与恢复文件读写
#include "Endian.hpp"

#include <new>
#include <string.h>

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
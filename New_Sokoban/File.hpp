#pragma once
#include <stdint.h>
//配置文件读写、地图文件读写、关卡回放与恢复文件读写

//tType目标类型 cpData数据 bDataIsBigEndian数据本身的顺序是否为大端字节序
template<typename T>
void ConvertEndianData2Type(T &tType, const unsigned char *cpData, bool bDataIsBigEndian)
{
	constexpr size_t szTypeByte = sizeof(tType);
	tType = 0;//置空

	if (bDataIsBigEndian)
	{
		for (long i = 0; i < szTypeByte; ++i)//正序遍历
		{
			tType <<= 8;//移动一字节
			tType |= cpData[i];//放在最低字节
		}
	}
	else
	{
		for (long i = szTypeByte - 1; i >= 0; --i)//倒序遍历
		{
			tType <<= 8;//移动一字节
			tType |= cpData[i];//放在最高字节
		}
	}
}

//cpData目标数据 tType类型 bDataIsBigEndian数据最终需要的顺序是否为大端字节序
template<typename T>
void ConvertEndianType2Data(unsigned char *cpData, const T &tType, bool bDataNeedBigEndian)
{
	constexpr size_t szTypeByte = sizeof(tType);
	T tTypeTemp = tType;

	if (bDataNeedBigEndian)
	{
		for (long i = szTypeByte - 1; i >= 0; --i)//倒序遍历
		{
			cpData[i] = (char)tTypeTemp;//截断高字节
			tTypeTemp >>= 8;//右移一字节
		}
	}
	else
	{
		for (long i = 0; i < szTypeByte; ++i)//正序遍历
		{
			cpData[i] = (char)tTypeTemp;//截断高字节
			tTypeTemp >>= 8;//右移一字节
		}
	}
}

template<typename T>
bool ReadFileWithGeneralEndian(FILE *fpRead, T &tData)
{
	//读取数据
	unsigned char cReadData[sizeof(T)];
	if (fread(cReadData, sizeof(T), 1, fpRead) != 1)
	{
		return false;
	}
	//转换字节序
	ConvertEndianData2Type<T>(tData, cReadData, true);

	return true;
}

template<typename T>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T &tData)
{
	//转换字节序
	unsigned char cWriteData[sizeof(T)];
	ConvertEndianType2Data<T>(cWriteData, tData, true);

	//写入数据
	if (fwrite(cWriteData, sizeof(T), 1, fpWrite) != 1)
	{
		return false;
	}

	return true;
}

template<typename T, size_t N>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T(&tArr)[N])
{
	//两种情况处理，每个元素大小等于一字节的数组直接写入文件，否则对每个字节的字节序进行变换
	if (sizeof(T) == 1)
	{
		if (fwrite(tArr, sizeof(T), N, fpWrite) != N)
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (!WriteFileWithGeneralEndian<T>(fpWrite, tArr[i]))
			{
				return false;
			}
		}
	}

	return true;
}

template<typename T, size_t N>
bool ReadFileWithGeneralEndian(FILE *fpRead, T(&tArr)[N])
{
	//两种情况处理，每个元素大小等于一字节的数组直接读入数组，否则对每个字节的字节序进行变换
	if (sizeof(T) == 1)
	{
		if (fread(tArr, sizeof(T), N, fpRead) != N)
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < N; ++i)
		{
			if (!ReadFileWithGeneralEndian<T>(fpRead, tArr[i]))
			{
				return false;
			}
		}
	}

	return true;
}


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

#include "Draw.hpp"

//玩家数据读写类
#include "Player.hpp"
class Player_File
{
private:
	struct File_Structure
	{
		uint64_t x;//玩家坐标x
		uint64_t y;//玩家坐标y
		uint64_t u64PlayerStatus;//玩家状态

		Player_Draw::Symbol stPlayerSymbol[PLAYER_SYMBOL_COUNT];//玩家符号
	};
public:
	bool WriteFile(FILE *fpWrite, const Player &csPlayer, const Player_Draw &csPlayerDraw)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (uint64_t)csPlayer.x);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (uint64_t)csPlayer.y);
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (uint64_t)csPlayer.enPlayerStatus);
		if (!bSuccess)
		{
			return false;
		}

		for (long i = 0; i < PLAYER_SYMBOL_COUNT; ++i)
		{
			if (!WriteFileWithGeneralEndian(fpWrite, csPlayerDraw[i].cStr) ||
				!WriteFileWithGeneralEndian(fpWrite, csPlayerDraw[i].ucColor))
			{
				return false;
			}
		}
		

		return true;
	}

	bool ReadFile(FILE *fpRead, Player &csPlayer)
	{



	}

};



//地图数据读写类
#include "Map.hpp"
class Map_File
{
private:
	struct File_Structure
	{
		uint64_t u64MapWidth;//地图宽
		uint64_t u64MapHight;//地图高
		uint64_t u64AllBoxNum;//箱子总数
		uint64_t u64DestnBoxNum;//在终点的箱子数

		Map_Draw::Symbol stMapSymbol;//地图符号
		Map::Block enMapData;//变长地图数据集
	};
	
	bool WriteFile(FILE *fpWrite, const Map &csMap)
	{



	}

	bool ReadFile(FILE *fpRead, Map &csMap)
	{



	}



};




//链表数据读写类
#include "List.hpp"
#include "Control.hpp"
class List_File
{
private:
	struct File_Structure
	{
		uint64_t u64ListNum;//链表数据个数
		Game_Control::Move_Data stListData;//变长链表数据集
	};

	bool WriteFile(FILE *fpWrite, const List<Game_Control::Move_Data> &csList)
	{



	}

	bool ReadFile(FILE *fpRead, List<Game_Control::Move_Data> &csList)
	{



	}




};

#include "Level.hpp"
//关卡文件
class Level_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_Level")];

		//玩家、地图、最高纪录(时间、步数)
		

	};






};

#include "Playback.hpp"
//回放文件
class Playback_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_PlayBack")];

		

		

		
		
		
	};





};


#include "Progress.hpp"
//游戏进度文件（残局文件）
class Progress_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_Progress")];




	};







};


#include "Config.hpp"
//游戏配置文件
class Config_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_Config")];
		uint64_t u64HeadSize;




	};
};
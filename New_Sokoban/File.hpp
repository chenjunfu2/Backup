#pragma once
#include <stdint.h>
//�����ļ���д����ͼ�ļ���д���ؿ��ط���ָ��ļ���д

#include "Endian.hpp"

#include <new>

//�ļ�ͷ��
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
		stFileInfo = {Sokoban_Unknow,0};//����

		char cReadStr[READ_STRLEN];//0~30 �����ַ� 31 '\0'
		for (size_t i = 0; i < READ_STRLEN - 1; ++i)
		{
			cReadStr[i] = fgetc(fpRead);
			if (cReadStr[i] == '\0')
			{
				break;
			}
		}
		cReadStr[READ_STRLEN - 1] = '\0';

		//�Ƚ�ǰ׺
		static constexpr char cBaseStr[] = "Sokoban_";
		static constexpr size_t cBaseStrLen = sizeof(cBaseStr) - 1;
		static_assert(sizeof(cBaseStr) < sizeof(cReadStr) - 1);

		if (strncmp(cReadStr, cBaseStr, cBaseStrLen) != 0)
		{
			return false;
		}

		//�ȽϺ�׺��ͨ�����б������αȽϻ��ʵ���ļ�����ö��
		for (Type i = Sokoban_Level; i <= Sokoban_Config; i = (Type)(i + 1))
		{
			if (strcmp(&FileHeadStr[i][cBaseStrLen], &cReadStr[cBaseStrLen]) == 0)
			{
				stFileInfo.enFileType = i;
			}
		}

		//������������򷵻�ʧ��
		if (stFileInfo.enFileType == Sokoban_Unknow)
		{
			return false;
		}

		//����汾��
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
		//д�������ַ���
		if (!WriteFileWithGeneralEndian(fpWrite, FileHeadStr[stFileInfo.enFileType],
			strlen(FileHeadStr[stFileInfo.enFileType])))//��д��ĩβ\0
		{
			return false;
		}

		//д��汾��
		uint64_t u64Write = stFileInfo.szFileVersion;
		if (!WriteFileWithGeneralEndian(fpWrite, u64Write))
		{
			return false;
		}
		
		return true;
	}
};

//������ݶ�д��
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

		static_assert(sizeof(unsigned char &) == sizeof(stPlayerFile.enPlayerStatus));//ȷ�����ʹ�Сһ�£��������UB
	}

	static bool ReadFile(FILE *fpRead, Player::File &stPlayerFile)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stPlayerFile.u64X);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stPlayerFile.u64Y);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, (unsigned char &)stPlayerFile.enPlayerStatus);
		return bSuccess;

		static_assert(sizeof(unsigned char &) == sizeof(stPlayerFile.enPlayerStatus));//ȷ�����ʹ�Сһ�£��������UB
	}

};

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

//��ͼ���ݶ�д��
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
		bSuccess = bSuccess && stMapFile.enpMapData != nullptr;//�ж��Ƿ�Ϊ��
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (unsigned char*)stMapFile.enpMapData, stMapFile.u64MapWidth * stMapFile.u64MapHight);//д���ͼ���ݼ�
		return bSuccess;

		static_assert(sizeof(unsigned char) == sizeof(*stMapFile.enpMapData));//ȷ�����ʹ�Сһ�£��������UB
	}

	static bool ReadFile(FILE *fpRead, Map::File &stMapFile)
	{
		bool bSuccess = true;
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64MapWidth);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64MapHight);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64AllBoxNum);
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, stMapFile.u64DestnBoxNum);
		bSuccess = bSuccess && (stMapFile.enpMapData = new(std::nothrow) Map::Block[stMapFile.u64MapWidth * stMapFile.u64MapHight]) != nullptr;//�����ڴ�
		bSuccess = bSuccess && ReadFileWithGeneralEndian(fpRead, (unsigned char *)stMapFile.enpMapData, stMapFile.u64MapWidth * stMapFile.u64MapHight);//��ȡ��ͼ���ݼ�
		return bSuccess;

		static_assert(sizeof(unsigned char) == sizeof(*stMapFile.enpMapData));//ȷ�����ʹ�Сһ�£��������UB
	}
};

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

//��߼�¼���ݶ�д��
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
//��߼�¼�������ݶ�д��
class Record_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record_Draw::File &stControlFile)
	{
		for (size_t i = 0; i < Record_Draw::RECORD_SYMBOL_COUNT; ++i)
		{
			size_t szStrLength = stControlFile.stSymbol[i].strOutput.length();//��ȡ�ַ������ȣ�������ĩβ\0

			if (!WriteFileWithGeneralEndian(fpWrite, szStrLength) ||//д���ַ������ȣ�������ĩβ\0
				!WriteFileWithGeneralEndian(fpWrite, stControlFile.stSymbol[i].strOutput.data(), szStrLength) ||//д���ַ�����������ĩβ\0
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
			if (!ReadFileWithGeneralEndian(fpRead, szStrLength))//��ȡ�ַ������ȣ�������ĩβ\0
			{
				return false;
			}
			stControlFile.stSymbol[i].strOutput.resize(szStrLength);//Ԥ�����ڴ沢�ı�string��С���������ֱ�Ӷ���

			if (!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].strOutput.data(), szStrLength) ||//�����ַ�����������ĩβ\0
				!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].ucNumbColor) ||
				!ReadFileWithGeneralEndian(fpRead, stControlFile.stSymbol[i].ucTextColor))
			{
				return false;
			}
		}

		return true;
	}
};

//��Ϸ�������ݶ�д��
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
���������ļ���д���̶����һ���ļ��汾��
д��ʱд��汾��
��ȡʱͨ���汾�ŵ��øð汾��Ӧ�Ķ�ȡ����������ȱʧ��������ʹ��Ĭ����Ϊ

����汾�űȵ�ǰ������߰汾�Ż��ߣ��򱨴��޷���ȡ�������ȡ�������ļ�������汾���ڵ�ǰ��߰汾�Ļ���

ÿ���汾�����Լ���Ӧ�Ķ�д����
*/

#include "Level.hpp"
//�ؿ��ļ�
class Level_File
{
private:

public:
	//��ҡ���ͼ����߼�¼(����Խ��Խ��) Highest_Record







};

#include "Playback.hpp"
//�ط��ļ�
class Playback_File
{
private:

public:





};


#include "Progress.hpp"
//��Ϸ�����ļ����о��ļ���
class Progress_File
{
private:

public:







};


#include "Config.hpp"
//��Ϸ�����ļ�
class Config_File
{
private:

public:






};
#pragma once
#include <stdint.h>
//�����ļ���д����ͼ�ļ���д���ؿ��ط���ָ��ļ���д

//tTypeĿ������ cpData���� bDataIsBigEndian���ݱ����˳���Ƿ�Ϊ����ֽ���
template<typename T>
void ConvertEndianData2Type(T &tType, const unsigned char *cpData, bool bDataIsBigEndian)
{
	constexpr size_t szTypeByte = sizeof(tType);
	tType = 0;//�ÿ�

	if (bDataIsBigEndian)
	{
		for (long i = 0; i < szTypeByte; ++i)//�������
		{
			tType <<= 8;//�ƶ�һ�ֽ�
			tType |= cpData[i];//��������ֽ�
		}
	}
	else
	{
		for (long i = szTypeByte - 1; i >= 0; --i)//�������
		{
			tType <<= 8;//�ƶ�һ�ֽ�
			tType |= cpData[i];//��������ֽ�
		}
	}
}

//cpDataĿ������ tType���� bDataIsBigEndian����������Ҫ��˳���Ƿ�Ϊ����ֽ���
template<typename T>
void ConvertEndianType2Data(unsigned char *cpData, const T &tType, bool bDataNeedBigEndian)
{
	constexpr size_t szTypeByte = sizeof(tType);
	T tTypeTemp = tType;

	if (bDataNeedBigEndian)
	{
		for (long i = szTypeByte - 1; i >= 0; --i)//�������
		{
			cpData[i] = (char)tTypeTemp;//�ضϸ��ֽ�
			tTypeTemp >>= 8;//����һ�ֽ�
		}
	}
	else
	{
		for (long i = 0; i < szTypeByte; ++i)//�������
		{
			cpData[i] = (char)tTypeTemp;//�ضϸ��ֽ�
			tTypeTemp >>= 8;//����һ�ֽ�
		}
	}
}

template<typename T>
bool ReadFileWithGeneralEndian(FILE *fpRead, T &tData)
{
	//��ȡ����
	unsigned char cReadData[sizeof(T)];
	if (fread(cReadData, sizeof(T), 1, fpRead) != 1)
	{
		return false;
	}
	//ת���ֽ���
	ConvertEndianData2Type<T>(tData, cReadData, true);

	return true;
}

template<typename T>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T &tData)
{
	//ת���ֽ���
	unsigned char cWriteData[sizeof(T)];
	ConvertEndianType2Data<T>(cWriteData, tData, true);

	//д������
	if (fwrite(cWriteData, sizeof(T), 1, fpWrite) != 1)
	{
		return false;
	}

	return true;
}

template<typename T>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T* tArr, size_t szArrLen)
{
	//�����������ÿ��Ԫ�ش�С����һ�ֽڵ�����ֱ��д���ļ��������ÿ���ֽڵ��ֽ�����б任
	if constexpr (sizeof(T) == 1)
	{
		if (fwrite(tArr, sizeof(T), szArrLen, fpWrite) != szArrLen)
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < szArrLen; ++i)
		{
			if (!WriteFileWithGeneralEndian<T>(fpWrite, tArr[i]))
			{
				return false;
			}
		}
	}

	return true;
}

template<typename T>
bool ReadFileWithGeneralEndian(FILE *fpRead, T* tArr, size_t szArrLen)
{
	//�����������ÿ��Ԫ�ش�С����һ�ֽڵ�����ֱ�Ӷ������飬�����ÿ���ֽڵ��ֽ�����б任
	if constexpr (sizeof(T) == 1)
	{
		if (fread(tArr, sizeof(T), szArrLen, fpRead) != szArrLen)
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < szArrLen; ++i)
		{
			if (!ReadFileWithGeneralEndian<T>(fpRead, tArr[i]))
			{
				return false;
			}
		}
	}

	return true;
}

//ת�����ã������Զ����С��ģ��
template<typename T, size_t N>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T(&tArr)[N])
{
	WriteFileWithGeneralEndian(fpWrite, tArr, N);
}

//ת�����ã������Զ����С��ģ��
template<typename T, size_t N>
bool ReadFileWithGeneralEndian(FILE *fpRead, T(&tArr)[N])
{
	ReadFileWithGeneralEndian(fpRead, tArr, N);
}


#include <new>

//�ļ�ͷ��֤��
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
		char cReadStr[32];//0~30 �����ַ� 31 '\0'
		for (long i = 0; i < 31; ++i)
		{
			cReadStr[i] = fgetc(fpRead);
			if (cReadStr[i] == '\0')
			{
				break;
			}
		}
		cReadStr[31] = '\0';

		//�ȽϿ�ͷ
		constexpr char cBaseStr[] = "Sokoban_";
		constexpr size_t cBaseStrLen = sizeof(cBaseStr) - 1;
		static_assert(sizeof(cBaseStr) < sizeof(cReadStr) - 1);

		if (strncmp(cReadStr, cBaseStr, cBaseStrLen) != 0)
		{
			return Unknow;
		}
		
		//�ȽϽ�����������
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
		for (long i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
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
		for (long i = 0; i < Player_Draw::PLAYER_SYMBOL_COUNT; ++i)
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
		for (long i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
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
		for (long i = 0; i < Map_Draw::MAP_SYMBOL_COUNT; ++i)
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
		
		
		
	}

	static bool ReadFile(FILE *fpRead, Record::File &stControlFile)
	{
		
		
		
		
		
	}
};

//��߼�¼�������ݶ�д��
class Record_Draw_File
{
public:
	static bool WriteFile(FILE *fpWrite, const Record_Draw::File &stControlFile)
	{}

	static bool ReadFile(FILE *fpRead, Record_Draw::File &stControlFile)
	{}
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


#include "Level.hpp"
//�ؿ��ļ�
class Level_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_Level";
public:
	//��ҡ���ͼ����߼�¼(����Խ��Խ��) Highest_Record







};

#include "Playback.hpp"
//�ط��ļ�
class Playback_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_PlayBack";
public:





};


#include "Progress.hpp"
//��Ϸ�����ļ����о��ļ���
class Progress_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_Progress";
public:







};


#include "Config.hpp"
//��Ϸ�����ļ�
class Config_File
{
private:
	static constexpr char cpFileHead[] = "Sokoban_Config";
public:






};
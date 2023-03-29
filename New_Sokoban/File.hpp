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

template<typename T, size_t N>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T(&tArr)[N])
{
	//�����������ÿ��Ԫ�ش�С����һ�ֽڵ�����ֱ��д���ļ��������ÿ���ֽڵ��ֽ�����б任
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
	//�����������ÿ��Ԫ�ش�С����һ�ֽڵ�����ֱ�Ӷ������飬�����ÿ���ֽڵ��ֽ�����б任
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

#include "Draw.hpp"

//������ݶ�д��
#include "Player.hpp"
class Player_File
{
private:
	struct File_Structure
	{
		uint64_t x;//�������x
		uint64_t y;//�������y
		uint64_t u64PlayerStatus;//���״̬

		Player_Draw::Symbol stPlayerSymbol[PLAYER_SYMBOL_COUNT];//��ҷ���
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



//��ͼ���ݶ�д��
#include "Map.hpp"
class Map_File
{
private:
	struct File_Structure
	{
		uint64_t u64MapWidth;//��ͼ��
		uint64_t u64MapHight;//��ͼ��
		uint64_t u64AllBoxNum;//��������
		uint64_t u64DestnBoxNum;//���յ��������

		Map_Draw::Symbol stMapSymbol;//��ͼ����
		Map::Block enMapData;//�䳤��ͼ���ݼ�
	};
	
	bool WriteFile(FILE *fpWrite, const Map &csMap)
	{



	}

	bool ReadFile(FILE *fpRead, Map &csMap)
	{



	}



};




//�������ݶ�д��
#include "List.hpp"
#include "Control.hpp"
class List_File
{
private:
	struct File_Structure
	{
		uint64_t u64ListNum;//�������ݸ���
		Game_Control::Move_Data stListData;//�䳤�������ݼ�
	};

	bool WriteFile(FILE *fpWrite, const List<Game_Control::Move_Data> &csList)
	{



	}

	bool ReadFile(FILE *fpRead, List<Game_Control::Move_Data> &csList)
	{



	}




};

#include "Level.hpp"
//�ؿ��ļ�
class Level_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_Level")];

		//��ҡ���ͼ����߼�¼(ʱ�䡢����)
		

	};






};

#include "Playback.hpp"
//�ط��ļ�
class Playback_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_PlayBack")];

		

		

		
		
		
	};





};


#include "Progress.hpp"
//��Ϸ�����ļ����о��ļ���
class Progress_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_Progress")];




	};







};


#include "Config.hpp"
//��Ϸ�����ļ�
class Config_File
{
private:
	struct BinFile
	{
		char cFileHead[sizeof("Sokoban_Config")];
		uint64_t u64HeadSize;




	};
};
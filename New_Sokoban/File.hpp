#pragma once
#include <stdint.h>
//�����ļ���д����ͼ�ļ���д���ؿ��ط���ָ��ļ���д
#include "Endian.hpp"

#include <new>
#include <string.h>

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
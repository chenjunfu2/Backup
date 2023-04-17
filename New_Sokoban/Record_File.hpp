#pragma once
#include "Endian.hpp"

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
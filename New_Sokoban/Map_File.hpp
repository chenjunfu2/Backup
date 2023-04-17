#pragma once
#include "Endian.hpp"

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
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (unsigned char *)stMapFile.enpMapData, stMapFile.u64MapWidth * stMapFile.u64MapHight);//д���ͼ���ݼ�
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
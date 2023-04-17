#pragma once
#include "Endian.hpp"

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
		bSuccess = bSuccess && WriteFileWithGeneralEndian(fpWrite, (unsigned char *)stMapFile.enpMapData, stMapFile.u64MapWidth * stMapFile.u64MapHight);//写入地图数据集
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
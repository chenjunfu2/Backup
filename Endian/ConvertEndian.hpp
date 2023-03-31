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

#include <stdio.h>

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

template<typename T>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T *tArr, size_t szArrLen)
{
	//两种情况处理，每个元素大小等于一字节的数组直接写入文件，否则对每个字节的字节序进行变换
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
bool ReadFileWithGeneralEndian(FILE *fpRead, T *tArr, size_t szArrLen)
{
	//两种情况处理，每个元素大小等于一字节的数组直接读入数组，否则对每个字节的字节序进行变换
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

//转发调用，数组自动求大小套模板
template<typename T, size_t N>
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T(&tArr)[N])
{
	WriteFileWithGeneralEndian(fpWrite, tArr, N);
}

//转发调用，数组自动求大小套模板
template<typename T, size_t N>
bool ReadFileWithGeneralEndian(FILE *fpRead, T(&tArr)[N])
{
	ReadFileWithGeneralEndian(fpRead, tArr, N);
}
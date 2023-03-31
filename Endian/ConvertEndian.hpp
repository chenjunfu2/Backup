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

#include <stdio.h>

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
bool WriteFileWithGeneralEndian(FILE *fpWrite, const T *tArr, size_t szArrLen)
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
bool ReadFileWithGeneralEndian(FILE *fpRead, T *tArr, size_t szArrLen)
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
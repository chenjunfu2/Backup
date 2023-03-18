//tTypeĿ������ cpData���� bDataIsBigEndian���ݱ����˳���Ƿ�Ϊ����ֽ���
template<typename T>
void ConvertEndianData2Type(T &tType, const char *cpData, bool bDataIsBigEndian)
{
	constexpr size_t szTypeByte = sizeof(tType);
	tType = 0;//�ÿ�

	if (bDataIsBigEndian)
	{
		for (int i = 0; i < szTypeByte; ++i)//�������
		{
			tType <<= 8;//�ƶ�һ�ֽ�
			tType |= cpData[i];//��������ֽ�
		}
	}
	else
	{
		for (int i = szTypeByte - 1; i >= 0; --i)//�������
		{
			tType <<= 8;//�ƶ�һ�ֽ�
			tType |= cpData[i];//��������ֽ�
		}
	}
}

//cpDataĿ������ tType���� bDataIsBigEndian����������Ҫ��˳���Ƿ�Ϊ����ֽ���
template<typename T>
void ConvertEndianType2Data(char *cpData, const T &tType, bool bDataNeedBigEndian)
{
	constexpr size_t szTypeByte = sizeof(tType);
	T tTypeTemp = tType;

	if (bDataNeedBigEndian)
	{
		for (int i = szTypeByte - 1; i >= 0; --i)//�������
		{
			cpData[i] = (char)tTypeTemp;//�ضϸ��ֽ�
			tTypeTemp >>= 8;//����һ�ֽ�
		}
	}
	else
	{
		for (int i = 0; i < szTypeByte; ++i)//�������
		{
			cpData[i] = (char)tTypeTemp;//�ضϸ��ֽ�
			tTypeTemp >>= 8;//����һ�ֽ�
		}
	}
}


#include <stdarg.h>
#include <ctype.h>

/* ��д�Ǵ���ֽ��򣬷�����С��
%f float
%d double
%b bool
%c char
%s short
%i int
%p pointer
%l long
%g long long
%r str
%% %
*/

size_t BinStrPrint(char *pTarget, size_t szTargetSize, const char *pFormat, ...)//����νsigned��unsigned���ڲ�ͳһ����unsigned����
{
	va_list ap;//ת�� std::nullptr_t �� void*(C++11 ��)��ת�� float �� double��ת�� bool��char��short ���������� (C++11 ��)ö�ٵ� int
	va_start(ap, pFormat);

	const char *pTemp = pFormat;
	size_t ulCurrent = 0;
	bool bIsString;
	while (*pTemp != '\0')
	{
		bIsString = false;
		if (*pTemp != '%')
		{
			if (ulCurrent + sizeof(*pTarget) > szTargetSize)
			{
				pTarget[ulCurrent] = '\0';
				return ++ulCurrent;
			}

			bIsString = true;
			pTarget[ulCurrent] = *pTemp;
			++ulCurrent;
			++pTemp;
			continue;
		}

		++pTemp;
		bool bIsBigEndian = isupper(*pTemp);
		switch (tolower(*pTemp))
		{
		case 'f'://float
		case 'd'://double
			{
				double dArg = va_arg(ap, double);
				if (ulCurrent + sizeof(dArg) >= szTargetSize)
				{
					return ulCurrent;
				}

				ConvertEndianType2Data(pTarget + ulCurrent, dArg, bIsBigEndian);
				ulCurrent += sizeof(dArg);
			}
			break;
		case 'b'://bool
		case 'c'://char
		case 's'://short
		case 'i'://int
			{
				unsigned int uiArg = va_arg(ap, unsigned int);
				if (ulCurrent + sizeof(uiArg) < szTargetSize)
				{
					return ulCurrent;
				}

				ConvertEndianType2Data(pTarget + ulCurrent, uiArg, bIsBigEndian);
				ulCurrent += sizeof(uiArg);
			}
			break;
		case 'p'://pointer
			{
				void *pArg = va_arg(ap, void *);
				if (ulCurrent + sizeof(pArg) < szTargetSize)
				{
					return ulCurrent;
				}

				ConvertEndianType2Data(pTarget + ulCurrent, pArg, bIsBigEndian);
				ulCurrent += sizeof(pArg);
			}
			break;
		case 'l'://long
			{
				unsigned long ulArg = va_arg(ap, unsigned long);
				if (ulCurrent + sizeof(ulArg) < szTargetSize)
				{
					return ulCurrent;
				}

				ConvertEndianType2Data(pTarget + ulCurrent, ulArg, bIsBigEndian);
				ulCurrent += sizeof(ulArg);
			}
			break;
		case 'g'://long long
			{
				unsigned long long ullArg = va_arg(ap, unsigned long long);
				if (ulCurrent + sizeof(ullArg) < szTargetSize)
				{
					return ulCurrent;
				}

				ConvertEndianType2Data(pTarget + ulCurrent, ullArg, bIsBigEndian);
				ulCurrent += sizeof(ullArg);
			}
			break;
		case 'r'://str
			{
				char *pArg = va_arg(ap, char *);
				while (*pArg != '%')
				{
					if (ulCurrent + sizeof(*pTarget) > szTargetSize)
					{
						pTarget[ulCurrent] = '\0';
						return ++ulCurrent;
					}

					bIsString = true;
					pTarget[ulCurrent] = *pTemp;
					++ulCurrent;
					++pTemp;
				}
			}
			break;
		case '%'://%ת������
			{
				if (ulCurrent + sizeof(*pTarget) > szTargetSize)
				{
					pTarget[ulCurrent] = '\0';
					return ++ulCurrent;
				}

				bIsString = true;
				pTarget[ulCurrent] = '%';
				++ulCurrent;
				++pTemp;
			}
			break;
		default:
			{
				return ulCurrent;
			}
			break;
		}//switch

	}//while (*pTemp != '\0')

	if (bIsString)
	{
		pTarget[ulCurrent] = '\0';
		++ulCurrent;
	}

	va_end(ap);

	return ulCurrent;
}

size_t BinStrScanf(char *pTarget, size_t szTargetSize, const char *pFormat, ...)//����νsigned��unsigned���ڲ�ͳһ����unsigned����
{
	
}
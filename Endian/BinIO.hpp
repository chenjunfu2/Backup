#include "ConvertEndian.hpp"

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

#define ConvertType2Data(ap, type, bigendian, target, targetsize, targetcurrent)\
{\
	type tArg = va_arg(ap, type);\
	if(targetcurrent + sizeof(type) >= targetsize)\
	{\
		return targetcurrent;\
	}\
	ConvertEndianType2Data(target + targetcurrent, tArg, bigendian);\
	targetcurrent += sizeof(type);\
}

size_t BinStrPrint(unsigned char *pTarget, size_t szTargetSize, const char *pFormat, ...)//����νsigned��unsigned���ڲ�ͳһ����unsigned����
{
	va_list ap;//ת�� std::nullptr_t �� void*(C++11 ��)��ת�� float �� double��ת�� bool��char��short ���������� (C++11 ��)ö�ٵ� int
	va_start(ap, pFormat);

	const char *pTemp = pFormat;
	size_t ulCurrent = 0;
	bool bIsString = false;
	while (*pTemp != '\0')
	{
		bIsString = false;
		if (*pTemp != '%' || *(++pTemp) == '%')//�����Ϊ%������鿴��һ�������Ҳ��%��ת���%���������%��һ�������鿴��һ������һ����Ϊ%�ͽ���switch����
		{
			if (ulCurrent + sizeof(*pTarget) >= szTargetSize)
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

		bool bIsBigEndian = isupper(*pTemp);
		switch (tolower(*pTemp))
		{
		case 'f'://float
		case 'd'://double
			{
				//ConvertType2Data(ap, double, bIsBigEndian, pTarget, szTargetSize, ulCurrent);
				//���⴦��ת�������Σ���֤���������ݲ���
			}
			break;
		case 'b'://bool
		case 'c'://char
		case 's'://short
		case 'i'://int
			{
				ConvertType2Data(ap, unsigned int, bIsBigEndian, pTarget, szTargetSize, ulCurrent);
			}
			break;
		case 'p'://pointer
			{
				//ConvertType2Data(ap, void *, bIsBigEndian, pTarget, szTargetSize, ulCurrent);
				//���⴦��ת�������Σ���֤���������ݲ���
			}
			break;
		case 'l'://long
			{
				ConvertType2Data(ap, unsigned long, bIsBigEndian, pTarget, szTargetSize, ulCurrent);
			}
			break;
		case 'g'://long long
			{
				ConvertType2Data(ap, unsigned long long, bIsBigEndian, pTarget, szTargetSize, ulCurrent);
			}
			break;
		case 'r'://str
			{
				for (char *pArg = va_arg(ap, char *); *pArg != '\0'; ++pArg)
				{
					if (ulCurrent + sizeof(*pTarget) >= szTargetSize)
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
		default:
			{
				return ulCurrent;
			}
			break;
		}//switch

	}//while (*pTemp != '\0')

	if (bIsString)
	{
		if (ulCurrent + sizeof(*pTarget) >= szTargetSize)//���˾���ĩβ�ض�
		{
			pTarget[ulCurrent] = '\0';
			++ulCurrent;
		}
		else//��������һ��λ�÷���\0
		{
			++ulCurrent;
			pTarget[ulCurrent] = '\0';
		}
	}

	va_end(ap);

	return ulCurrent;
}

size_t BinStrScanf(unsigned char *pTarget, size_t szTargetSize, const char *pFormat, ...)//����νsigned��unsigned���ڲ�ͳһ����unsigned����
{

}
#include "ConvertEndian.hpp"

#include <stdarg.h>
#include <ctype.h>

/* 大写是大端字节序，否则是小端
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

size_t BinStrPrint(unsigned char *pTarget, size_t szTargetSize, const char *pFormat, ...)//无所谓signed和unsigned，内部统一按照unsigned处理
{
	va_list ap;//转换 std::nullptr_t 到 void*(C++11 起)，转换 float 到 double，转换 bool、char、short 及无作用域 (C++11 起)枚举到 int
	va_start(ap, pFormat);

	const char *pTemp = pFormat;
	size_t ulCurrent = 0;
	bool bIsString = false;
	while (*pTemp != '\0')
	{
		bIsString = false;
		if (*pTemp != '%' || *(++pTemp) == '%')//如果不为%则递增查看下一个，如果也是%就转义成%（即如果是%则一定递增查看下一个，下一个不为%就进入switch处理）
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
				//特殊处理，转换到整形，保证二进制数据不变
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
				//特殊处理，转换到整形，保证二进制数据不变
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
		if (ulCurrent + sizeof(*pTarget) >= szTargetSize)//超了就在末尾截断
		{
			pTarget[ulCurrent] = '\0';
			++ulCurrent;
		}
		else//否则在下一个位置放上\0
		{
			++ulCurrent;
			pTarget[ulCurrent] = '\0';
		}
	}

	va_end(ap);

	return ulCurrent;
}

size_t BinStrScanf(unsigned char *pTarget, size_t szTargetSize, const char *pFormat, ...)//无所谓signed和unsigned，内部统一按照unsigned处理
{

}
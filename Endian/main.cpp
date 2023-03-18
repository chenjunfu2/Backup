#include "ConvertEndian.hpp"


int main(void)
{
	char BigEd[8] = {0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,};
	unsigned long long ullType;

	ConvertEndianData2Type(&ullType, BigEd, true);

	char LitEd[8];
	ConvertEndianType2Data(LitEd, &ullType, false);

	return 0;//此句用于下断点
}
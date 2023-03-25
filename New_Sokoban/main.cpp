#include <stdio.h>
#include "Interaction.hpp"


long funcctc(void *)
{
	printf("ctrl+c ");
	return 0;
}

long funcb(void *)
{
	printf("b ");
	return 0;
}

long funcf12(void *)
{
	printf("f12 ");
	return 0;
}

long funcf1(void *)
{
	printf("f1 ");
	return 0;
}

long funcpgup(void *)
{
	printf("pgup ");
	return 0;
}

int main(void)
{
	Interaction ir;

	ir.RegisterKey(0x03, {funcctc});
	ir.RegisterKey(0x62, {funcb});

	ir.RegisterKey(Interaction::Code_E0, 0x86, {funcf12});
	ir.RegisterKey(Interaction::Code_00, 0x3B, {funcf1});
	ir.RegisterKey(Interaction::Code_E0, 0x48, {funcpgup});


	ir.Loop();


	return 0;
}
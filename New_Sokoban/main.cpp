#include <stdio.h>
#include "Interaction.hpp"
#include "Control.hpp"
#include "Draw.hpp"

#include "File.hpp"

struct ps
{
	Player *pr;
	Map_Draw *mpd;
	Player_Draw *prd;
	Game_Control *gec;
};


void move_draw(ps *stps, long lXMove, long lYMove)
{
	if (stps->gec->MovePlayer(lXMove, lYMove))
	{
		stps->mpd->CrossDraw(stps->pr->x, stps->pr->y);
		stps->prd->Draw();
	}
}

long fcw(void * p)
{
	ps *stps = (ps *)p;
	move_draw(stps, 0, -1);
	return stps->gec->IsWin();
}

long fca(void *p)
{
	ps *stps = (ps *)p;
	move_draw(stps, -1, 0);
	return stps->gec->IsWin();
}

long fcs(void *p)
{
	ps *stps = (ps *)p;
	move_draw(stps, 0, 1);
	return stps->gec->IsWin();
}

long fcd(void *p)
{
	ps *stps = (ps *)p;
	move_draw(stps, 1, 0);
	return stps->gec->IsWin();
}

long fcr(void *p)
{
	ps *stps = (ps *)p;
	if (stps->gec->UndoMove())
	{
		stps->mpd->CrossDraw(stps->pr->x, stps->pr->y);
		stps->prd->Draw();
	}
	return 0;
}

long fcf(void *p)
{
	ps *stps = (ps *)p;
	if (stps->gec->RedoMove())
	{
		stps->mpd->CrossDraw(stps->pr->x, stps->pr->y);
		stps->prd->Draw();
	}
	return 0;
}

int main(void)
{
	unsigned char mb[15][16] =
	{
		{0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0},//1
		{0,0,0,1,2,1,0,0,0,0,0,0,0,0,1,0},
		{0,0,0,1,0,1,0,2,0,0,0,0,0,3,0,0},
		{0,0,0,1,3,1,1,1,1,1,0,0,0,0,0,0},
		{1,1,1,1,0,0,3,0,2,0,0,0,1,0,0,0},
		{1,2,0,3,0,0,0,1,1,1,0,0,0,0,0,0},
		{1,1,1,0,1,3,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,3},
		{0,0,0,3,0,0,1,0,0,0,0,3,0,0,0,0},
		{0,0,0,1,0,0,0,0,0,3,2,2,0,0,0,2},
		{0,0,0,0,0,0,0,0,0,3,2,2,0,3,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	};
	

	Map map((Map::Block *)mb, 16, 15);
	Player player(5, 5);
	Game_Control gamectl(map, player);
	OutputConsole optc;
	optc.SetCursorShow(false);

	Map_Draw::Symbol sym[5] =
	{
		{"  ",OutputConsole::black,},
		{"■",OutputConsole::bright_white,},
		{"○",OutputConsole::bright_red,},
		{"□",OutputConsole::bright_yellow,},
		{"●",OutputConsole::bright_green,},
	};
	Map_Draw mpdraw(map, sym, optc);

	Player_Draw::Symbol pym[2] =
	{
		{"♀",OutputConsole::bright_white,},
		{"♀",OutputConsole::bright_red,},
	};
	Player_Draw prdraw(player, pym, optc);

	ps rgp =
	{
		&player,
		&mpdraw,
		&prdraw,
		&gamectl,
	};

	mpdraw.Draw();
	prdraw.Draw();

	Interaction ir;

	ir.RegisterKey('w', {fcw,&rgp});
	ir.RegisterKey('W', {fcw,&rgp});

	ir.RegisterKey('a', {fca,&rgp});
	ir.RegisterKey('A', {fca,&rgp});

	ir.RegisterKey('s', {fcs,&rgp});
	ir.RegisterKey('S', {fcs,&rgp});

	ir.RegisterKey('d', {fcd,&rgp});
	ir.RegisterKey('D', {fcd,&rgp});

	ir.RegisterKey('r', {fcr,&rgp});
	ir.RegisterKey('R', {fcr,&rgp});

	ir.RegisterKey('f', {fcf,&rgp});
	ir.RegisterKey('F', {fcf,&rgp});

	if (ir.Loop() == 1)
	{
		optc.SetCursorPos(0, 16);
		printf("you win!");
		return 1;
	}
	
	return 0;
}
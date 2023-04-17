#include <stdio.h>
#include "Interaction.hpp"
#include "Game_Control.hpp"
#include "Draw.hpp"
#include "Record.hpp"

#include "File.hpp"

OutputConsole optc;

struct ps
{
	Player &pr;
	Record_Draw &rcd;
	Map_Draw &mpd;
	Player_Draw &prd;
	Game_Control &gec;
};


inline void move_draw(ps &stps, bool bMove)
{
	if (bMove)
	{
		stps.mpd.CrossDrawMap(stps.pr.x, stps.pr.y);
		stps.prd.DrawPlayer({0,0});
		stps.rcd.DrawRecordCurrent({0,16});
		stps.rcd.DrawRecordRanking({0,17});
		stps.rcd.DrawRecordRankingList({0,20});
	}
}

long fcw(void * p)
{
	ps &stps = *(ps *)p;
	move_draw(stps, stps.gec.MovePlayer(0, -1));
	return stps.gec.IsWin();
}

long fca(void *p)
{
	ps &stps = *(ps *)p;
	move_draw(stps, stps.gec.MovePlayer(-1, 0));
	return stps.gec.IsWin();
}

long fcs(void *p)
{
	ps &stps = *(ps *)p;
	move_draw(stps, stps.gec.MovePlayer(0, 1));
	return stps.gec.IsWin();
}

long fcd(void *p)
{
	ps &stps = *(ps *)p;
	move_draw(stps, stps.gec.MovePlayer(1, 0));
	return stps.gec.IsWin();
}

long fcr(void *p)
{
	ps &stps = *(ps *)p;
	move_draw(stps, stps.gec.UndoMove());
	return 0;
}

long fcf(void *p)
{
	ps &stps = *(ps *)p;
	move_draw(stps, stps.gec.RedoMove());
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
	
	size_t szrcd[] = {201,82,90,119,157};
	Map map((Map::Block *)mb, 16, 15);
	Player player(5, 5);
	Record record(0, szrcd, sizeof(szrcd) / sizeof(*szrcd));
	Game_Control gamectl(map, player, record);
	optc.SetCursorShow(false);

	Map_Draw::Symbol sym[Map_Draw::MAP_SYMBOL_COUNT] =
	{
		{"  ",OutputConsole::black,},
		{"■",OutputConsole::bright_white,},
		{"○",OutputConsole::bright_red,},
		{"□",OutputConsole::bright_yellow,},
		{"●",OutputConsole::bright_green,},
	};
	Map_Draw mpdraw(map, sym, optc);

	Player_Draw::Symbol pym[Player_Draw::PLAYER_SYMBOL_COUNT] =
	{
		{"♀",OutputConsole::bright_white,},
		{"♀",OutputConsole::bright_red,},
	};
	Player_Draw prdraw(player, pym, optc);

	Record_Draw::Symbol rym[Record_Draw::RECORD_SYMBOL_COUNT] = 
	{
		"第%r步", OutputConsole::bright_white, OutputConsole::bright_yellow,
		"第%r名", OutputConsole::bright_white, OutputConsole::bright_yellow,
		":%r步", OutputConsole::bright_white, OutputConsole::bright_yellow,
	};
	Record_Draw rcdraw(record, rym, optc);

	ps rgp =
	{
		player,
		rcdraw,
		mpdraw,
		prdraw,
		gamectl,
	};

	mpdraw.DrawMap({0,0});
	prdraw.DrawPlayer({0,0});
	rcdraw.DrawRecordCurrent({0,16});
	rcdraw.DrawRecordRanking({0,17});
	rcdraw.DrawRecordRankingList({0,20});

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
		optc.SetCursorPos(0, 17);
		printf("通关！");
		return 1;
	}
	
	return 0;
}
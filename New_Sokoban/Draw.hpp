#pragma once
#include "Console.hpp"

/*
地图符号
0：空地"  "
1：墙壁"■"
2：目的地"☆" "○"
3：箱子"□"
4：箱子在目的地"★" "●"
*/

#include "Map.hpp"

#define MAP_SYMBOL_COUNT 5
#define MAP_SYMBOL_LEN 2

class Map_Draw//地图绘制
{
public:
	struct Symbol
	{
		char cStr[MAP_SYMBOL_LEN + 1];//每种字符占用2个字节，用于输出符号
		OutputConsole::Color ucColor;//输出的颜色
	};

	struct File
	{
		Symbol (&stSymbol)[MAP_SYMBOL_COUNT];//5种符号
	};
private:
	Map &csMap;
	Symbol stSymbol[MAP_SYMBOL_COUNT];//5种符号
	OutputConsole &csConsole;//控制台对象
public:
	Map_Draw(Map &_csMap, const File &_File, OutputConsole &_csConsole) :
		csMap(_csMap), stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _File.stSymbol, sizeof(stSymbol));
	}

	const File GetFile(void)
	{
		return File(stSymbol);
	}
public:
	Map_Draw(Map &_csMap, const Symbol *_pstSymbol, OutputConsole &_csConsole) :
		csMap(_csMap), stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	~Map_Draw(void) = default;

	void SetSymbol(const Symbol *_pstSymbol)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	const Symbol *GetSymbol(void) const
	{
		return stSymbol;
	}

	//全部绘制
	void Draw(const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		OutputConsole::Color OldColor = csConsole.GetColor();//保存颜色

		for (long y = 0; y < csMap.Hight(); ++y)
		{
			csConsole.SetCursorPos(stDrawPos.x, stDrawPos.y + y);
			for (long x = 0; x < csMap.Width(); ++x)
			{
				//绘制一行
				Map::Block enBlock = csMap(x, y);
				csConsole.SetColor(stSymbol[enBlock].ucColor);
				csConsole.WriteBuffer(stSymbol[enBlock].cStr, MAP_SYMBOL_LEN);
			}
		}

		csConsole.SetColor(OldColor);//恢复颜色
	}

	//交叉重绘，绘制(更新)玩家附近的内容
	void CrossDraw(long lCrossX, long lCrossY, const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		//绘制地图
		OutputConsole::Color OldColor = csConsole.GetColor();//保存颜色

		//绘制一行
		long lBegX = lCrossX - 2 < 0 ? 0 : lCrossX - 2;
		long lEndX = lCrossX + 3 >= csMap.Width() ? csMap.Width() : lCrossX + 3;
		for (long x = lBegX; x < lEndX; ++x)
		{
			csConsole.SetCursorPos(stDrawPos.x + x * 2, stDrawPos.y + lCrossY);

			Map::Block enBlock = csMap(x, lCrossY);
			csConsole.SetColor(stSymbol[enBlock].ucColor);
			csConsole.WriteBuffer(stSymbol[enBlock].cStr, MAP_SYMBOL_LEN);
		}

		//绘制一列
		long lBegY = lCrossY - 2 < 0 ? 0 : lCrossY - 2;
		long lEndY = lCrossY + 3 >= csMap.Hight() ? csMap.Hight() : lCrossY + 3;
		for (long y = lBegY; y < lEndY; ++y)
		{
			csConsole.SetCursorPos(stDrawPos.x + lCrossX * 2, stDrawPos.y + y);

			Map::Block enBlock = csMap(lCrossX, y);
			csConsole.SetColor(stSymbol[enBlock].ucColor);
			csConsole.WriteBuffer(stSymbol[enBlock].cStr, MAP_SYMBOL_LEN);
		}

		csConsole.SetColor(OldColor);//恢复颜色
	}
};

/*
玩家符号：
0：人♀
1：人在目的地♀
*/

#include "Player.hpp"

#define PLAYER_SYMBOL_COUNT 2
#define PLAYER_SYMBOL_LEN 2

class Player_Draw//玩家绘制
{
public:
	struct Symbol
	{
		char cStr[PLAYER_SYMBOL_LEN + 1];//每种字符占用2个字节，用于输出符号
		OutputConsole::Color ucColor;//输出的颜色
	};

	struct File
	{
		Symbol (&stSymbol)[PLAYER_SYMBOL_COUNT];//2种符号
	};
private:
	Player &csPlayer;
	Symbol stSymbol[PLAYER_SYMBOL_COUNT];//2种符号
	OutputConsole &csConsole;//控制台对象
public:
	Player_Draw(Player &_csPlayer, const File &_File, OutputConsole &_csConsole) :
		csPlayer(_csPlayer), stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _File.stSymbol, sizeof(stSymbol));
	}

	const File GetFile(void)
	{
		return File(stSymbol);
	}
public:
	Player_Draw(Player &_csPlayer, const Symbol *_pstSymbol, OutputConsole &_csConsole) :
		csPlayer(_csPlayer), stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	~Player_Draw(void) = default;

	void SetSymbol(const Symbol *_pstSymbol)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	const Symbol *GetSymbol(void) const
	{
		return stSymbol;
	}

	//绘制人物
	void Draw(const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		OutputConsole::Color OldColor = csConsole.GetColor();//保存颜色

		csConsole.SetCursorPos(stDrawPos.x + csPlayer.x * 2, stDrawPos.y + csPlayer.y);

		Player::Block enBlock = csPlayer.enPlayerStatus;
		csConsole.SetColor(stSymbol[enBlock].ucColor);
		csConsole.WriteBuffer(stSymbol[enBlock].cStr, PLAYER_SYMBOL_LEN);

		csConsole.SetColor(OldColor);//恢复颜色
	}
};

#include "Record.hpp"
class Record_Draw
{
	//显示分数的颜色、历史最高纪录的个数等





};

#include "Menu.hpp"

class Menu_Draw//菜单绘制
{










};
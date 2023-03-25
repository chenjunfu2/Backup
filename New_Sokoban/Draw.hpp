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

#define MAP_SYMBOL_COUNT 7
#define MAP_SYMBOL_LEN 2

class Map_Draw//地图绘制
{
public:
	struct Symbol
	{
		char cStr[MAP_SYMBOL_COUNT];//每种字符占用2个字节，用于输出符号
		OutputConsole::Color ucColor;//输出的颜色
	};
private:
	Symbol stSymbol[MAP_SYMBOL_COUNT];//5种符号
	OutputConsole csConsole;//控制台对象
public:
	Map_Draw(const Symbol *_pstSymbol, const OutputConsole &_csConsole = OutputConsole()) :
		stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	~Map_Draw(void) = default;

	Symbol &operator[](long lPos)//返回成员引用
	{
		return stSymbol[lPos];
	}

	//全部绘制
	void DrawMap(const Map &csMap, const OutputConsole::CursorPos &stDrawPos)
	{
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
	}

	//交叉重绘，绘制(更新)玩家附近的内容
	void CrossRedraw(const Map &csMap, long lCrossX, long lCrossY, const OutputConsole::CursorPos &stDrawPos)
	{
		//绘制地图

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
		char cStr[PLAYER_SYMBOL_COUNT];//每种字符占用2个字节，用于输出符号
		OutputConsole::Color ucColor;//输出的颜色
	};
private:
	Symbol stSymbol[PLAYER_SYMBOL_LEN];//2种符号
	OutputConsole csConsole;//控制台对象
public:
	Player_Draw(const Symbol *_pstSymbol, const OutputConsole &_csConsole = OutputConsole()) :
		stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	~Player_Draw(void) = default;

	Symbol &operator[](long lPos)//返回成员引用
	{
		return stSymbol[lPos];
	}

	//绘制人物
	void DrawPlayer(const Player &csPlayer, const OutputConsole::CursorPos &stDrawPos)
	{
		csConsole.SetCursorPos(stDrawPos.x + csPlayer.x * 2, stDrawPos.y + csPlayer.y);

		Player::Block enBlock = csPlayer.enPlayer;
		csConsole.SetColor(stSymbol[enBlock].ucColor);
		csConsole.WriteBuffer(stSymbol[enBlock].cStr, PLAYER_SYMBOL_LEN);
	}
};

#include "Menu.hpp"

class Menu_Draw//菜单绘制
{










};
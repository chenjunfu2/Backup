#pragma once
#include "Console.hpp"
/*
玩家符号：
0：人♀
1：人在目的地♀
*/

#include "Player.hpp"

class Player_Draw//玩家绘制
{
public:
	static constexpr size_t PLAYER_SYMBOL_COUNT = 2;
	static constexpr size_t PLAYER_SYMBOL_LEN = 2;

	struct Symbol
	{
		char cStr[PLAYER_SYMBOL_LEN + 1];//每种字符占用2个字节，用于输出符号
		OutputConsole::Color ucColor;//输出的颜色
	};

	struct File
	{
		Symbol(&stSymbol)[PLAYER_SYMBOL_COUNT];//2种符号
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
	void DrawPlayer(const OutputConsole::CursorPos &stDrawPos = {0,0}) const
	{
		OutputConsole::Color OldColor = csConsole.GetColor();//保存颜色

		csConsole.SetCursorPos(stDrawPos.x + csPlayer.x * 2, stDrawPos.y + csPlayer.y);

		Player::Block enBlock = csPlayer.enPlayerStatus;
		csConsole.SetColor(stSymbol[enBlock].ucColor);
		csConsole.WriteBuffer(stSymbol[enBlock].cStr, PLAYER_SYMBOL_LEN);

		csConsole.SetColor(OldColor);//恢复颜色
	}
};
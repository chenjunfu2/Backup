#pragma once
#include "Console.hpp"
/*
��ҷ��ţ�
0���ˡ�
1������Ŀ�ĵء�
*/

#include "Player.hpp"

class Player_Draw//��һ���
{
public:
	static constexpr size_t PLAYER_SYMBOL_COUNT = 2;
	static constexpr size_t PLAYER_SYMBOL_LEN = 2;

	struct Symbol
	{
		char cStr[PLAYER_SYMBOL_LEN + 1];//ÿ���ַ�ռ��2���ֽڣ������������
		OutputConsole::Color ucColor;//�������ɫ
	};

	struct File
	{
		Symbol(&stSymbol)[PLAYER_SYMBOL_COUNT];//2�ַ���
	};
private:
	Player &csPlayer;
	Symbol stSymbol[PLAYER_SYMBOL_COUNT];//2�ַ���
	OutputConsole &csConsole;//����̨����
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

	//��������
	void DrawPlayer(const OutputConsole::CursorPos &stDrawPos = {0,0}) const
	{
		OutputConsole::Color OldColor = csConsole.GetColor();//������ɫ

		csConsole.SetCursorPos(stDrawPos.x + csPlayer.x * 2, stDrawPos.y + csPlayer.y);

		Player::Block enBlock = csPlayer.enPlayerStatus;
		csConsole.SetColor(stSymbol[enBlock].ucColor);
		csConsole.WriteBuffer(stSymbol[enBlock].cStr, PLAYER_SYMBOL_LEN);

		csConsole.SetColor(OldColor);//�ָ���ɫ
	}
};
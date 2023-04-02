#pragma once
#include "Console.hpp"

/*
��ͼ����
0���յ�"  "
1��ǽ��"��"
2��Ŀ�ĵ�"��" "��"
3������"��"
4��������Ŀ�ĵ�"��" "��"
*/

#include "Map.hpp"

#define MAP_SYMBOL_COUNT 5
#define MAP_SYMBOL_LEN 2

class Map_Draw//��ͼ����
{
public:
	struct Symbol
	{
		char cStr[MAP_SYMBOL_LEN + 1];//ÿ���ַ�ռ��2���ֽڣ������������
		OutputConsole::Color ucColor;//�������ɫ
	};

	struct File
	{
		Symbol (&stSymbol)[MAP_SYMBOL_COUNT];//5�ַ���
	};
private:
	Map &csMap;
	Symbol stSymbol[MAP_SYMBOL_COUNT];//5�ַ���
	OutputConsole &csConsole;//����̨����
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

	//ȫ������
	void Draw(const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		OutputConsole::Color OldColor = csConsole.GetColor();//������ɫ

		for (long y = 0; y < csMap.Hight(); ++y)
		{
			csConsole.SetCursorPos(stDrawPos.x, stDrawPos.y + y);
			for (long x = 0; x < csMap.Width(); ++x)
			{
				//����һ��
				Map::Block enBlock = csMap(x, y);
				csConsole.SetColor(stSymbol[enBlock].ucColor);
				csConsole.WriteBuffer(stSymbol[enBlock].cStr, MAP_SYMBOL_LEN);
			}
		}

		csConsole.SetColor(OldColor);//�ָ���ɫ
	}

	//�����ػ棬����(����)��Ҹ���������
	void CrossDraw(long lCrossX, long lCrossY, const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		//���Ƶ�ͼ
		OutputConsole::Color OldColor = csConsole.GetColor();//������ɫ

		//����һ��
		long lBegX = lCrossX - 2 < 0 ? 0 : lCrossX - 2;
		long lEndX = lCrossX + 3 >= csMap.Width() ? csMap.Width() : lCrossX + 3;
		for (long x = lBegX; x < lEndX; ++x)
		{
			csConsole.SetCursorPos(stDrawPos.x + x * 2, stDrawPos.y + lCrossY);

			Map::Block enBlock = csMap(x, lCrossY);
			csConsole.SetColor(stSymbol[enBlock].ucColor);
			csConsole.WriteBuffer(stSymbol[enBlock].cStr, MAP_SYMBOL_LEN);
		}

		//����һ��
		long lBegY = lCrossY - 2 < 0 ? 0 : lCrossY - 2;
		long lEndY = lCrossY + 3 >= csMap.Hight() ? csMap.Hight() : lCrossY + 3;
		for (long y = lBegY; y < lEndY; ++y)
		{
			csConsole.SetCursorPos(stDrawPos.x + lCrossX * 2, stDrawPos.y + y);

			Map::Block enBlock = csMap(lCrossX, y);
			csConsole.SetColor(stSymbol[enBlock].ucColor);
			csConsole.WriteBuffer(stSymbol[enBlock].cStr, MAP_SYMBOL_LEN);
		}

		csConsole.SetColor(OldColor);//�ָ���ɫ
	}
};

/*
��ҷ��ţ�
0���ˡ�
1������Ŀ�ĵء�
*/

#include "Player.hpp"

#define PLAYER_SYMBOL_COUNT 2
#define PLAYER_SYMBOL_LEN 2

class Player_Draw//��һ���
{
public:
	struct Symbol
	{
		char cStr[PLAYER_SYMBOL_LEN + 1];//ÿ���ַ�ռ��2���ֽڣ������������
		OutputConsole::Color ucColor;//�������ɫ
	};

	struct File
	{
		Symbol (&stSymbol)[PLAYER_SYMBOL_COUNT];//2�ַ���
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
	void Draw(const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		OutputConsole::Color OldColor = csConsole.GetColor();//������ɫ

		csConsole.SetCursorPos(stDrawPos.x + csPlayer.x * 2, stDrawPos.y + csPlayer.y);

		Player::Block enBlock = csPlayer.enPlayerStatus;
		csConsole.SetColor(stSymbol[enBlock].ucColor);
		csConsole.WriteBuffer(stSymbol[enBlock].cStr, PLAYER_SYMBOL_LEN);

		csConsole.SetColor(OldColor);//�ָ���ɫ
	}
};

#include "Record.hpp"
class Record_Draw
{
	//��ʾ��������ɫ����ʷ��߼�¼�ĸ�����





};

#include "Menu.hpp"

class Menu_Draw//�˵�����
{










};
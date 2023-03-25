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

#define MAP_SYMBOL_COUNT 7
#define MAP_SYMBOL_LEN 2

class Map_Draw//��ͼ����
{
public:
	struct Symbol
	{
		char cStr[MAP_SYMBOL_COUNT];//ÿ���ַ�ռ��2���ֽڣ������������
		OutputConsole::Color ucColor;//�������ɫ
	};
private:
	Symbol stSymbol[MAP_SYMBOL_COUNT];//5�ַ���
	OutputConsole csConsole;//����̨����
public:
	Map_Draw(const Symbol *_pstSymbol, const OutputConsole &_csConsole = OutputConsole()) :
		stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	~Map_Draw(void) = default;

	Symbol &operator[](long lPos)//���س�Ա����
	{
		return stSymbol[lPos];
	}

	//ȫ������
	void DrawMap(const Map &csMap, const OutputConsole::CursorPos &stDrawPos)
	{
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
	}

	//�����ػ棬����(����)��Ҹ���������
	void CrossRedraw(const Map &csMap, long lCrossX, long lCrossY, const OutputConsole::CursorPos &stDrawPos)
	{
		//���Ƶ�ͼ

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
		char cStr[PLAYER_SYMBOL_COUNT];//ÿ���ַ�ռ��2���ֽڣ������������
		OutputConsole::Color ucColor;//�������ɫ
	};
private:
	Symbol stSymbol[PLAYER_SYMBOL_LEN];//2�ַ���
	OutputConsole csConsole;//����̨����
public:
	Player_Draw(const Symbol *_pstSymbol, const OutputConsole &_csConsole = OutputConsole()) :
		stSymbol{0}, csConsole(_csConsole)
	{
		memcpy(stSymbol, _pstSymbol, sizeof(stSymbol));
	}

	~Player_Draw(void) = default;

	Symbol &operator[](long lPos)//���س�Ա����
	{
		return stSymbol[lPos];
	}

	//��������
	void DrawPlayer(const Player &csPlayer, const OutputConsole::CursorPos &stDrawPos)
	{
		csConsole.SetCursorPos(stDrawPos.x + csPlayer.x * 2, stDrawPos.y + csPlayer.y);

		Player::Block enBlock = csPlayer.enPlayer;
		csConsole.SetColor(stSymbol[enBlock].ucColor);
		csConsole.WriteBuffer(stSymbol[enBlock].cStr, PLAYER_SYMBOL_LEN);
	}
};

#include "Menu.hpp"

class Menu_Draw//�˵�����
{










};
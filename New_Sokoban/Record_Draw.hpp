#pragma once
#include "Console.hpp"

//显示带颜色的颜色分数、历史最高纪录（排行榜）、排名
#include <string>
#include "Record.hpp"
class Record_Draw
{
public:
	static constexpr size_t RECORD_SYMBOL_COUNT = 3;

	struct Symbol
	{
		std::string strOutput;//输出的文本（使用%r来指定分数文本填入的位置，使用%%来转译获得%）
		OutputConsole::Color ucTextColor;//输出的文本颜色
		OutputConsole::Color ucNumbColor;//输出的数值颜色
	};

	enum Symbol_Type
	{
		SymbolCurrent,//分数符号对象
		SymbolRanking,//排名符号对象
		SymbolRankingList,//排行榜符号对象
	};

	struct File
	{
		Symbol(&stSymbol)[RECORD_SYMBOL_COUNT];
	};
private:
	Record &csRecord;//分数对象

	Symbol stSymbol[RECORD_SYMBOL_COUNT];//符号对象
	size_t szCutPos[RECORD_SYMBOL_COUNT];//截断位置

	OutputConsole &csConsole;//控制台对象


	static void EscapeStr(std::string &strSource, size_t &szCutPos)
	{
		size_t szCurrentPos;
		//查找字符串内的%r并把%%替换成%然后在%r的位置使用\0截断字符串但是不删除后面的内容并把截断位置保存

		//是否遇到分界符%r
		bool bFindDelimiter = false;
		szCurrentPos = strSource.find('%');
		while (szCurrentPos != std::string::npos)
		{
			++szCurrentPos;//查看下一个字符
			if (!bFindDelimiter && strSource[szCurrentPos] == 'r')//第一次遇到%r
			{
				//定位回%截断字符串
				strSource[szCurrentPos - 1] = '\0';//%替换为\0
				strSource.erase(szCurrentPos, 1);//删掉r
				szCutPos = szCurrentPos;//经过转义处理的文本前半段结束和经过转义处理的文本后半段开始
				//找到了，剩下的不处理了直接忽略
				bFindDelimiter = true;
			}
			else if (strSource[szCurrentPos] == '%')//双重%%
			{
				strSource.erase(szCurrentPos, 1);//删掉%
			}

			szCurrentPos = strSource.find('%', szCurrentPos);//从当前位置继续找
		}

		if (bFindDelimiter)
		{
			strSource.push_back(' ');//末尾插入空格

		}
		else
		{
			szCutPos = strSource.length();//末尾索引
		}
	}
public:
	Record_Draw(Record &_csRecord, const File &_File, OutputConsole &_csConsole) :
		csRecord(_csRecord), csConsole(_csConsole)
	{
		for (size_t i = 0; i < RECORD_SYMBOL_COUNT; ++i)
		{
			stSymbol[i] = _File.stSymbol[i];
			EscapeStr(stSymbol[i].strOutput, szCutPos[i]);
		}
	}

	const File GetFile(void)
	{
		return File(stSymbol);
	}

public:
	Record_Draw(Record &_csRecord, const Symbol *_pstSymbol, OutputConsole &_csConsole) :
		csRecord(_csRecord), csConsole(_csConsole)
	{
		for (size_t i = 0; i < RECORD_SYMBOL_COUNT; ++i)
		{
			stSymbol[i] = _pstSymbol[i];
			EscapeStr(stSymbol[i].strOutput, szCutPos[i]);
		}
	}

	~Record_Draw(void) = default;

	void SetSymbol(const Symbol *_pstSymbol)
	{
		for (size_t i = 0; i < RECORD_SYMBOL_COUNT; ++i)
		{
			stSymbol[i] = _pstSymbol[i];
			EscapeStr(stSymbol[i].strOutput, szCutPos[i]);
		}
	}

	const Symbol *GetSymbol(void) const
	{
		return stSymbol;
	}


	static inline void DrawTextAndValue(OutputConsole &csConsole, const Symbol &stDrawSymbol, size_t szDrawCutPos, size_t szDrawValue)
	{
		//输出前半段
		csConsole.UnrecoveredColorWriteBuffer(stDrawSymbol.ucTextColor, &stDrawSymbol.strOutput[0], szDrawCutPos - 1);

		//输出数值
		char strNumb[21];
		int iNumbLen = snprintf(strNumb, sizeof(strNumb), "%zu", szDrawValue);
		csConsole.UnrecoveredColorWriteBuffer(stDrawSymbol.ucNumbColor, strNumb, iNumbLen);

		//输出后半段
		csConsole.UnrecoveredColorWriteBuffer(stDrawSymbol.ucTextColor, &stDrawSymbol.strOutput[szDrawCutPos], stDrawSymbol.strOutput.length() - szDrawCutPos);
	}


	void DrawRecordCurrent(const OutputConsole::CursorPos &stDrawPos = {0,0}) const
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//保存颜色

		//设置光标并输出
		csConsole.SetCursorPos(stDrawPos);
		DrawTextAndValue(csConsole, stSymbol[SymbolCurrent], szCutPos[SymbolCurrent], csRecord.Current());

		csConsole.SetColor(ucOldColor);//恢复颜色
	}

	void DrawRecordRanking(const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//保存颜色

		//设置光标并输出
		csConsole.SetCursorPos(stDrawPos);
		DrawTextAndValue(csConsole, stSymbol[SymbolRanking], szCutPos[SymbolRanking], csRecord.Ranking());

		csConsole.SetColor(ucOldColor);//恢复颜色
	}

	void DrawRecordRankingList(const OutputConsole::CursorPos &stDrawPos = {0,0}, size_t szNewLine = 2)
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//保存颜色

		auto atCurrentY = stDrawPos.y;//保存y值
		csConsole.SetCursorPos(stDrawPos);

		for (size_t i = 1, j = 1; i <= Record::HISTROY_COUNT; ++i)
		{
			if (csRecord.RankingList(i) != Record::szUnvalid)
			{
				//输出名次
				DrawTextAndValue(csConsole, stSymbol[SymbolRanking], szCutPos[SymbolRanking], i);
				csConsole.MoveCursorPos(-1, 0);//左移一格吃掉空格
				//输出排行
				DrawTextAndValue(csConsole, stSymbol[SymbolRankingList], szCutPos[SymbolRankingList], csRecord.RankingList(i));
			}
			else
			{
				break;
			}

			if (j == szNewLine)
			{
				csConsole.SetCursorPos(stDrawPos.x, ++atCurrentY);//换行
				j = 1;//重置
			}
			else
			{
				++j;
			}
		}

		csConsole.SetColor(ucOldColor);//恢复颜色
	}

	void DrawRecordHistroyList(const OutputConsole::CursorPos &stDrawPos = {0,0}, size_t szNewLine = 2)
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//保存颜色

		auto atCurrentY = stDrawPos.y;//保存y值
		csConsole.SetCursorPos(stDrawPos);

		for (size_t i = 1, j = 1; i < Record::HISTROY_COUNT; ++i)
		{
			if (csRecord.HistroyList(i) != Record::szUnvalid)
			{
				//输出名次
				DrawTextAndValue(csConsole, stSymbol[SymbolRanking], szCutPos[SymbolRanking], i);
				csConsole.MoveCursorPos(-1, 0);//左移一格吃掉空格
				//输出排行
				DrawTextAndValue(csConsole, stSymbol[SymbolRankingList], szCutPos[SymbolRankingList], csRecord.HistroyList(i));
			}
			else
			{
				break;
			}

			if (j == szNewLine)
			{
				csConsole.SetCursorPos(stDrawPos.x, ++atCurrentY);//换行
				j = 1;//重置
			}
			else
			{
				++j;
			}
		}

		csConsole.SetColor(ucOldColor);//恢复颜色
	}
};
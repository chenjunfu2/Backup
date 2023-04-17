#pragma once
#include "Console.hpp"

//��ʾ����ɫ����ɫ��������ʷ��߼�¼�����а񣩡�����
#include <string>
#include "Record.hpp"
class Record_Draw
{
public:
	static constexpr size_t RECORD_SYMBOL_COUNT = 3;

	struct Symbol
	{
		std::string strOutput;//������ı���ʹ��%r��ָ�������ı������λ�ã�ʹ��%%��ת����%��
		OutputConsole::Color ucTextColor;//������ı���ɫ
		OutputConsole::Color ucNumbColor;//�������ֵ��ɫ
	};

	enum Symbol_Type
	{
		SymbolCurrent,//�������Ŷ���
		SymbolRanking,//�������Ŷ���
		SymbolRankingList,//���а���Ŷ���
	};

	struct File
	{
		Symbol(&stSymbol)[RECORD_SYMBOL_COUNT];
	};
private:
	Record &csRecord;//��������

	Symbol stSymbol[RECORD_SYMBOL_COUNT];//���Ŷ���
	size_t szCutPos[RECORD_SYMBOL_COUNT];//�ض�λ��

	OutputConsole &csConsole;//����̨����


	static void EscapeStr(std::string &strSource, size_t &szCutPos)
	{
		size_t szCurrentPos;
		//�����ַ����ڵ�%r����%%�滻��%Ȼ����%r��λ��ʹ��\0�ض��ַ������ǲ�ɾ����������ݲ��ѽض�λ�ñ���

		//�Ƿ������ֽ��%r
		bool bFindDelimiter = false;
		szCurrentPos = strSource.find('%');
		while (szCurrentPos != std::string::npos)
		{
			++szCurrentPos;//�鿴��һ���ַ�
			if (!bFindDelimiter && strSource[szCurrentPos] == 'r')//��һ������%r
			{
				//��λ��%�ض��ַ���
				strSource[szCurrentPos - 1] = '\0';//%�滻Ϊ\0
				strSource.erase(szCurrentPos, 1);//ɾ��r
				szCutPos = szCurrentPos;//����ת�崦����ı�ǰ��ν����;���ת�崦����ı����ο�ʼ
				//�ҵ��ˣ�ʣ�µĲ�������ֱ�Ӻ���
				bFindDelimiter = true;
			}
			else if (strSource[szCurrentPos] == '%')//˫��%%
			{
				strSource.erase(szCurrentPos, 1);//ɾ��%
			}

			szCurrentPos = strSource.find('%', szCurrentPos);//�ӵ�ǰλ�ü�����
		}

		if (bFindDelimiter)
		{
			strSource.push_back(' ');//ĩβ����ո�

		}
		else
		{
			szCutPos = strSource.length();//ĩβ����
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
		//���ǰ���
		csConsole.UnrecoveredColorWriteBuffer(stDrawSymbol.ucTextColor, &stDrawSymbol.strOutput[0], szDrawCutPos - 1);

		//�����ֵ
		char strNumb[21];
		int iNumbLen = snprintf(strNumb, sizeof(strNumb), "%zu", szDrawValue);
		csConsole.UnrecoveredColorWriteBuffer(stDrawSymbol.ucNumbColor, strNumb, iNumbLen);

		//�������
		csConsole.UnrecoveredColorWriteBuffer(stDrawSymbol.ucTextColor, &stDrawSymbol.strOutput[szDrawCutPos], stDrawSymbol.strOutput.length() - szDrawCutPos);
	}


	void DrawRecordCurrent(const OutputConsole::CursorPos &stDrawPos = {0,0}) const
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//������ɫ

		//���ù�겢���
		csConsole.SetCursorPos(stDrawPos);
		DrawTextAndValue(csConsole, stSymbol[SymbolCurrent], szCutPos[SymbolCurrent], csRecord.Current());

		csConsole.SetColor(ucOldColor);//�ָ���ɫ
	}

	void DrawRecordRanking(const OutputConsole::CursorPos &stDrawPos = {0,0})
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//������ɫ

		//���ù�겢���
		csConsole.SetCursorPos(stDrawPos);
		DrawTextAndValue(csConsole, stSymbol[SymbolRanking], szCutPos[SymbolRanking], csRecord.Ranking());

		csConsole.SetColor(ucOldColor);//�ָ���ɫ
	}

	void DrawRecordRankingList(const OutputConsole::CursorPos &stDrawPos = {0,0}, size_t szNewLine = 2)
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//������ɫ

		auto atCurrentY = stDrawPos.y;//����yֵ
		csConsole.SetCursorPos(stDrawPos);

		for (size_t i = 1, j = 1; i <= Record::HISTROY_COUNT; ++i)
		{
			if (csRecord.RankingList(i) != Record::szUnvalid)
			{
				//�������
				DrawTextAndValue(csConsole, stSymbol[SymbolRanking], szCutPos[SymbolRanking], i);
				csConsole.MoveCursorPos(-1, 0);//����һ��Ե��ո�
				//�������
				DrawTextAndValue(csConsole, stSymbol[SymbolRankingList], szCutPos[SymbolRankingList], csRecord.RankingList(i));
			}
			else
			{
				break;
			}

			if (j == szNewLine)
			{
				csConsole.SetCursorPos(stDrawPos.x, ++atCurrentY);//����
				j = 1;//����
			}
			else
			{
				++j;
			}
		}

		csConsole.SetColor(ucOldColor);//�ָ���ɫ
	}

	void DrawRecordHistroyList(const OutputConsole::CursorPos &stDrawPos = {0,0}, size_t szNewLine = 2)
	{
		OutputConsole::Color ucOldColor = csConsole.GetColor();//������ɫ

		auto atCurrentY = stDrawPos.y;//����yֵ
		csConsole.SetCursorPos(stDrawPos);

		for (size_t i = 1, j = 1; i < Record::HISTROY_COUNT; ++i)
		{
			if (csRecord.HistroyList(i) != Record::szUnvalid)
			{
				//�������
				DrawTextAndValue(csConsole, stSymbol[SymbolRanking], szCutPos[SymbolRanking], i);
				csConsole.MoveCursorPos(-1, 0);//����һ��Ե��ո�
				//�������
				DrawTextAndValue(csConsole, stSymbol[SymbolRankingList], szCutPos[SymbolRankingList], csRecord.HistroyList(i));
			}
			else
			{
				break;
			}

			if (j == szNewLine)
			{
				csConsole.SetCursorPos(stDrawPos.x, ++atCurrentY);//����
				j = 1;//����
			}
			else
			{
				++j;
			}
		}

		csConsole.SetColor(ucOldColor);//�ָ���ɫ
	}
};
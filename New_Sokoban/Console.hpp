#pragma once

#include <Windows.h>

class Console
{
private:
	HANDLE hConsole;
	CONSOLE_SCREEN_BUFFER_INFO stConsoleInfo;
public:
	enum class TextColor
	{
		//����ɫ
		black = 0,
		blue = FOREGROUND_BLUE,
		green = FOREGROUND_GREEN,
		light_green = FOREGROUND_GREEN | FOREGROUND_BLUE,
		red = FOREGROUND_RED,
		purple = FOREGROUND_BLUE | FOREGROUND_RED,
		yellow = FOREGROUND_GREEN | FOREGROUND_RED,
		white = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
		gray = FOREGROUND_INTENSITY,
		bright_blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		bright_green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		bright_light_green = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		bright_red = FOREGROUND_RED | FOREGROUND_INTENSITY,
		bright_purple = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
		bright_yellow = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
		bright_white = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
		//����ɫ(ʹ��ʱ������ɫ��λ��:|)
		bg_blue = BACKGROUND_BLUE,
		bg_green = BACKGROUND_GREEN,
		bg_light_green = BACKGROUND_GREEN | BACKGROUND_BLUE,
		bg_red = BACKGROUND_RED,
		bg_purple = BACKGROUND_BLUE | BACKGROUND_RED,
		bg_yellow = BACKGROUND_GREEN | BACKGROUND_RED,
		bg_white = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
		bg_gray = BACKGROUND_INTENSITY,
		bg_bright_blue = BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		bg_bright_green = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
		bg_bright_light_green = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		bg_bright_red = BACKGROUND_RED | BACKGROUND_INTENSITY,
		bg_bright_purple = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY,
		bg_bright_yellow = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY,
		bg_bright_white = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY,
	};

	struct CursorPos
	{
		SHORT x, y;
	};

	Console(HANDLE _hConsole = GetStdHandle(STD_OUTPUT_HANDLE)) :hConsole(_hConsole)
	{
		GetConsoleScreenBufferInfo(hConsole, &stConsoleInfo);
	}
	~Console(void) = default;

	HANDLE GetConsole(void)
	{
		return hConsole;
	}

	void SetConsole(HANDLE _hConsole)
	{
		hConsole = _hConsole;
	}

	DWORD WriteBuffer(const char *cpBuffer, DWORD dwWriteLen)
	{
		DWORD dwWrittenLen;
		if (!WriteConsoleA(hConsole, cpBuffer, dwWriteLen, &dwWrittenLen, NULL))
		{
			return 0;
		}
		return dwWrittenLen;
	}

	DWORD ClearBuffer(void)
	{
		DWORD dwWrittenLen;
		if (!FillConsoleOutputAttribute(hConsole, 0, stConsoleInfo.dwSize.X * stConsoleInfo.dwSize.Y, COORD{0, 0}, &dwWrittenLen))
		{
			return 0;
		}
		return dwWrittenLen;
	}

	//���ù������
	bool SetCursorPos(SHORT x, SHORT y)
	{
		return SetConsoleCursorPosition(hConsole, COORD{x, y});//ͬ��������̨(Set����)��Console����̨����Cursor��꣩ ��Positionλ�ã�
	}

	//��ȡ�������
	CursorPos GetCursorPos(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
		GetConsoleScreenBufferInfo(hConsole, &screen_buffer_info);
		return {screen_buffer_info.dwCursorPosition.X,screen_buffer_info.dwCursorPosition.Y};
	}

	//�ӵ�ǰλ���ƶ����
	bool MoveCursorPos(SHORT x, SHORT y)
	{
		CursorPos stCurrentPos = GetCursorPos();
		return SetCursorPos(stCurrentPos.x + x, stCurrentPos.y + y);
	}

	//�����ı���ɫ
	bool SetTextColor(TextColor enColor)
	{
		return SetConsoleTextAttribute(hConsole, (WORD)enColor);
	}

	//��ȡ�ı���ɫ
	TextColor GetTextColor(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
		GetConsoleScreenBufferInfo(hConsole, &screen_buffer_info);
		return (TextColor)screen_buffer_info.wAttributes;
	}

	//�õ���ǰ�������
	bool GetCursorShow(void)//return true������ɼ�
	{
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(hConsole, &cursor_info);//STD_OUTPUT_HANDLE  STD_INPUT_HANDLE
		return cursor_info.bVisible;
	}

	//�������||��ʾ
	bool SetCursorShow(bool bShow)//Show=false�����겻�ɼ�
	{
		CONSOLE_CURSOR_INFO cursor_info = {1, bShow};//1�����ɹ�������ַ���Ԫ��İٷֱȡ� ��ֵ����1��100֮�䡣 �����ۻ�仯����Χ����ȫ��䵥Ԫ�񵽵�Ԫ�ײ���ˮƽ������
		return SetConsoleCursorInfo(hConsole, &cursor_info);//STD_OUTPUT_HANDLE  STD_INPUT_HANDLE
	}
};
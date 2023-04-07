#pragma once

#include <Windows.h>

class OutputConsole
{
public:
	enum TextColor//低4位
	{
		//字体色
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
	};

	enum BackColor//高4位
	{
		//背景色(使用时与字体色按位或:|)
		bg_black = 0,
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

	typedef BYTE Color;//由TextColor|BackColor得来

	struct CursorPos
	{
		SHORT x, y;
	};
private:
	HANDLE hOutputConsole;
public:
	OutputConsole(HANDLE _hOutputConsole = GetStdHandle(STD_OUTPUT_HANDLE)) :
		hOutputConsole(_hOutputConsole)
	{}
	~OutputConsole(void) = default;

	HANDLE GetConsole(void)
	{
		return hOutputConsole;
	}

	void SetConsole(HANDLE _hOutputConsole)
	{
		hOutputConsole = _hOutputConsole;
	}

	DWORD WriteBuffer(const char *cpBuffer, DWORD dwWriteLen)
	{
		DWORD dwWrittenLen;
		if (!WriteConsoleA(hOutputConsole, cpBuffer, dwWriteLen, &dwWrittenLen, NULL))
		{
			return 0;
		}
		return dwWrittenLen;
	}

	DWORD UnrecoveredColorWriteBuffer(Color ucColor, const char *cpBuffer, DWORD dwWriteLen)
	{
		if (!SetColor(ucColor))
		{
			return 0;
		}
		return WriteBuffer(cpBuffer, dwWriteLen);
	}

	DWORD RecoveredColorWriteBuffer(Color ucColor, const char *cpBuffer, DWORD dwWriteLen)
	{
		Color ucOriginalColor = GetColor();
		DWORD dwWrittenLen = UnrecoveredColorWriteBuffer(ucColor, cpBuffer, dwWriteLen);
		SetColor(ucOriginalColor);
		return dwWrittenLen;
	}

	DWORD UnrecoveredCursorPosWriteBuffer(const CursorPos &stPos, const char *cpBuffer, DWORD dwWriteLen)
	{
		if (!SetCursorPos(stPos))
		{
			return 0;
		}
		return WriteBuffer(cpBuffer, dwWriteLen);
	}

	DWORD RecoveredCursorPosWriteBuffer(const CursorPos &stPos, const char *cpBuffer, DWORD dwWriteLen)
	{
		CursorPos stOriginalPos = GetCursorPos();
		DWORD dwWrittenLen = UnrecoveredCursorPosWriteBuffer(stPos, cpBuffer, dwWriteLen);
		SetCursorPos(stOriginalPos);
		return dwWrittenLen;
	}

	DWORD ClearBuffer(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO stOutputConsoleInfo;
		if (!GetConsoleScreenBufferInfo(hOutputConsole, &stOutputConsoleInfo))
		{
			return 0;
		}

		DWORD dwWriteLen = stOutputConsoleInfo.dwSize.X * stOutputConsoleInfo.dwSize.Y;
		DWORD dwWrittenLen;
		if (!FillConsoleOutputAttribute(hOutputConsole, 0, dwWriteLen, COORD{0, 0}, &dwWrittenLen))
		{
			return 0;
		}

		return dwWrittenLen;
	}

	//设置光标坐标
	bool SetCursorPos(SHORT x, SHORT y)
	{
		return SetCursorPos(CursorPos{x,y});
	}

	bool SetCursorPos(const CursorPos &stPos)
	{
		return SetConsoleCursorPosition(hOutputConsole, COORD{stPos.x, stPos.y});//同步到控制台(Set设置)（Console控制台）（Cursor光标） （Position位置）
	}

	//获取光标坐标
	CursorPos GetCursorPos(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
		GetConsoleScreenBufferInfo(hOutputConsole, &screen_buffer_info);
		return {screen_buffer_info.dwCursorPosition.X,screen_buffer_info.dwCursorPosition.Y};
	}

	//从当前位置移动光标
	bool MoveCursorPos(SHORT x, SHORT y)
	{
		CursorPos stCurrentPos = GetCursorPos();
		return SetCursorPos(stCurrentPos.x + x, stCurrentPos.y + y);
	}

	//设置文本颜色
	bool SetColor(Color ucColor)
	{
		return SetConsoleTextAttribute(hOutputConsole, (WORD)ucColor);
	}

	//获取文本颜色
	Color GetColor(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
		GetConsoleScreenBufferInfo(hOutputConsole, &screen_buffer_info);

		return (Color)screen_buffer_info.wAttributes;
	}

	void SeparateColor(Color ucColor, TextColor &enTextColor, BackColor &enBackColor)
	{
		enTextColor = (TextColor)(ucColor & 0x0F);//低4位
		enBackColor = (BackColor)(ucColor >> 4);//高4位
	}

	//得到当前光标设置
	bool GetCursorShow(void)//return true代表光标可见
	{
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(hOutputConsole, &cursor_info);//STD_OUTPUT_HANDLE  STD_INPUT_HANDLE
		return cursor_info.bVisible;
	}

	//光标隐藏||显示
	bool SetCursorShow(bool bShow)//Show=false代表光标不可见
	{
		CONSOLE_CURSOR_INFO cursor_info = {1, bShow};//1代表由光标填充的字符单元格的百分比。 此值介于1到100之间。 光标外观会变化，范围从完全填充单元格到单元底部的水平线条。
		return SetConsoleCursorInfo(hOutputConsole, &cursor_info);//STD_OUTPUT_HANDLE  STD_INPUT_HANDLE
	}
};


class InputConsole
{
private:
	HANDLE hInputConsole;
public:
	InputConsole(HANDLE _hInputConsole = GetStdHandle(STD_INPUT_HANDLE)):
		hInputConsole(_hInputConsole)
	{}
	~InputConsole(void) = default;

	bool SetUnbuffer(bool bUnBuffer)
	{
		DWORD dwInputConsoleMode;
		GetConsoleMode(hInputConsole, &dwInputConsoleMode);
		if (bUnBuffer)
		{
			dwInputConsoleMode &= (~ENABLE_LINE_INPUT);
			dwInputConsoleMode &= (~ENABLE_ECHO_INPUT);
			dwInputConsoleMode &= (~ENABLE_PROCESSED_INPUT);
			return SetConsoleMode(hInputConsole, dwInputConsoleMode);
		}
		else
		{
			dwInputConsoleMode |= ENABLE_LINE_INPUT;
			dwInputConsoleMode |= ENABLE_ECHO_INPUT;
			dwInputConsoleMode |= ENABLE_PROCESSED_INPUT;
			return SetConsoleMode(hInputConsole, dwInputConsoleMode);
		}
	}
	
	bool GetUnbuffer(void)
	{
		DWORD dwInputConsoleMode;
		GetConsoleMode(hInputConsole, &dwInputConsoleMode);
		return (dwInputConsoleMode & ENABLE_LINE_INPUT) != ENABLE_LINE_INPUT;//如果行缓冲模式为打开则为有缓冲输入
	}
	
	int GetChar(void)
	{
		DWORD dwReadNumber;
		INPUT_RECORD stInput;
	
		do
		{
			if (!ReadConsoleInputA(hInputConsole, &stInput, 1, &dwReadNumber) || dwReadNumber != 1)
			{
				return -1;//EOF
			}
		} while (stInput.EventType != KEY_EVENT);//不是按键事件则丢弃
		
		return (int)stInput.Event.KeyEvent.uChar.AsciiChar;
	}
	
	int UnbufferGetChar(void)
	{
		DWORD dwInputConsoleMode;
		GetConsoleMode(hInputConsole, &dwInputConsoleMode);
		SetConsoleMode(hInputConsole, dwInputConsoleMode & (~ENABLE_LINE_INPUT) & (~ENABLE_ECHO_INPUT));
		unsigned char ucGet;
		DWORD dwReadByte;
		if (!ReadConsoleA(hInputConsole, &ucGet, sizeof(ucGet), &dwReadByte, NULL) || dwReadByte != sizeof(ucGet))
		{
			return -1;//EOF
		}
		SetConsoleMode(hInputConsole, dwInputConsoleMode);
	
		return (int)ucGet;
	}

};
#include "tools.h"
#include "windows.h"
#include <stdio.h>
// #include <locale>
// #include <codecvt>
// #include "atlbase.h"
// #include "atlstr.h"

void SetWindowSize(int lines, int cols)//设置窗口大小
{
	system("title GoBang");//设置窗口标题
	char cmd[30];
	sprintf_s(cmd, 30, "mode con cols=%d lines=%d", cols * 2, lines);//一个图形■占两个字符，故宽度乘以2
	system(cmd);//system(mode con cols=88 lines=88)设置窗口宽度和高度
}

void SetCursorPosition(const int x, const int y)//设置光标位置
{
	COORD position;
	position.X = x * 2;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void SetColor(int colorID)//设置文本颜色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

void SetBackColor()//设置文本背景色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_BLUE |
		BACKGROUND_BLUE |
		BACKGROUND_GREEN |
		BACKGROUND_RED);
}

void OutputDebugPrintf(const char *strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	va_end(vlArgs);
	// std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	// std::wstring wideStr = converter.from_bytes(strBuffer);
	OutputDebugString(static_cast<LPCSTR>(strBuffer));
}

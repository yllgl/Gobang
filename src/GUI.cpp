#include "GUI.h"
#include<iostream>
#include<string>
using namespace std;
#include"tools.h"
#include"cell.h"
string GUI::alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string GUI::fullalpha = "ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ";
const int GUI::numOfselections = 2;
const int GUI::gap = 8;
string GUI::selections[GUI::numOfselections] = { "双人模式","人机模式" };
void GUI::showBoard()
{
	system("color 8e");
	for (int i = -1; i < length; i++) {
		SetCursorPosition(origin_x - 1, origin_y + i);
		for (int j = -1; j < 2 * length - 1; j++) {
			cout.width(2);
			if (i == -1) {
				cout.width(0);
				if (j >= 0 && j % 2 == 0) {
					cout << fullalpha.substr(j/2*3, 3);
				}
				else if (j == -1) {

					cout << "  ";
				}
			}
			else if (j == -1) {
				if (i >= 0) {
					cout << i + 1;
				}
			}
			else if (i == 0) {
				if (j == 0) {
					cout << "┌";
				}
				else if (j == 2 * length - 2) {
					cout << "┐";
				}
				else if (j % 2 == 1) {
					cout << "─";
				}
				else {
					cout << "┬";
				}
			}
			else if (i == length - 1) {
				if (j == 0) {
					cout << "└";
				}
				else if (j == 2 * length - 2) {
					cout << "┘";
				}
				else if (j % 2 == 1) {
					cout << "─";
				}
				else {
					cout << "┴";
				}
			}
			else {
				if (j == 0) {
					cout << "├";
				}
				else if (j == 2 * length - 2) {
					cout << "┤";
				}
				else if (j % 2 == 1) {
					cout << "─";
				}
				else {
					cout << "┼";
				}
			}
		}
	}
}
void GUI::setStep(int row, int col, bool isblack, bool islast) {
	SetCursorPosition(origin_x + col, origin_y + row);
	if (islast) {
		if (isblack) {
			SetColor(0x80);
		}
		else {
			SetColor(0x8f);
		}
		cout << "  \b\b▲";
		return;
	}
	if (isblack) {
		SetColor(0x80);
		cout << "  \b\b●";
	}
	else {
		SetColor(0x8f);
		cout << "  \b\b●";
	}
}
void GUI::unsetStep(int row, int col) {
	SetCursorPosition(origin_x + col, origin_y + row);
	if (row == 0) {
		if (col == 0) {
			cout << "  \b\b┌";
		}
		else if (col == length - 1) {
			cout << "  \b\b┐";
		}
		else {
			cout << "  \b\b┬";
		}
	}
	else if (row == length - 1) {
		if (col == 0) {
			cout << "  \b\b└";
		}
		else if (col == length - 1) {
			cout << "  \b\b┘";
		}
		else {
			cout << "  \b\b┴";
		}
	}
	else {
		if (col == 0) {
			cout << "  \b\b├";
		}
		else if (col == length - 1) {
			cout << "  \b\b┤";
		}
		else {
			cout << "  \b\b┼";
		}
	}
}
void GUI::showWaitForInput(bool iscomputer,Piece playerPiece) {
	SetCursorPosition(output_col, output_row);
	SetColor(0x8e);
	string tmp(200, ' ');
	std::cout << tmp.c_str();
	SetCursorPosition(output_col, output_row);
	std::cout << "当前选手：";
	if (boardstate->isblack) {
		std::cout << "黑方，";
		if (iscomputer&&playerPiece != Black) {
			std::cout << "程序计算中...";
		}
		else {
			std::cout << "请输入下棋位置：";
		}
	}
	else {
		std::cout << "白方，";
		if (iscomputer&&playerPiece !=White) {
			std::cout << "程序计算中...";
		}
		else {
			std::cout << "请输入下棋位置：";
		}
	}
}
void GUI::showSelectMode(int key) {
	SetColor(0x8e);
	SetCursorPosition(output_col, output_row);
	std::cout << "请选择游戏模式：";
	std::cout << "(左右键选择,回车确认)";
	for (int i = 0; i < numOfselections; i++) {
		SetCursorPosition(output_col + gap * i, output_row + 1);
		if (key == i) {
			SetBackColor();//第一个选项设置背景色以表示当前选中
		}
		else {
			SetColor(0x83);
		}
		std::cout << selections[i].c_str();
	}
}
void GUI::showSelectPiece(int key) {
	SetCursorPosition(output_col, output_row);
	SetColor(0x8e);
	string tmp(200, ' ');
	std::cout << tmp.c_str();
	SetCursorPosition(output_col, output_row);
	std::cout << "请选择玩家执黑还是执白：";
	std::cout << "(左右键选择,回车确认)";
	const int numOfselections = 2;
	string selections[numOfselections] = { "玩家执白","玩家执黑" };
	for (int i = 0; i < numOfselections; i++) {
		SetCursorPosition(output_col + gap * i, output_row + 1);
		if (key == i) {
			SetBackColor();//第一个选项设置背景色以表示当前选中
		}
		else {
			SetColor(0x83);
		}
		std::cout << selections[i].c_str();
	}
}
void GUI::showEndGame(Piece result) {
	SetCursorPosition(5, output_row - 5);
	SetBackColor();
	switch (result) {
	case Black:
		std::cout << "黑方胜！按任意键继续...";
		break;
	case White:
		std::cout << "白方胜！按任意键继续...";
		break;
	case Empty:
		std::cout << "平局！按任意键继续...";
		break;
	case Wrong:
		std::cout << "黑棋禁手！黑方输，白方获胜！按任意键继续...";
		break;
	}
	return;
}
void GUI::showError(int mode) {
	SetCursorPosition(5, output_row +1);
	SetColor(0x84);
	switch (mode) {
	case 0:
		std::cout << "  输入错误！";
		break;
	case 1:
		std::cout << "  下棋失败！按任意键继续....";
		break;
	}
	return;
}
void GUI::clearPanel() {
	string tmp(200, ' ');
	SetCursorPosition(output_col, output_row);
	SetColor(0x8e);
	std::cout << tmp.c_str();
	return;
}

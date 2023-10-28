#include "Controller.h"
#include<conio.h>
#include"cell.h"
bool Controller::AITurn() {
	bool isend = false;
	cell* p = boardstate->getNextPoint();
	if (p&&this->setStep(p->row, p->col,true)) {
		Piece winner = boardstate->winner();
		if (winner != Normal) {
			gui->showEndGame(winner);
			char ch = _getch();
			isend = true;
		}
	}
	return isend;
}
bool Controller::PlayerTurn(bool isAImode) {
	int idx = 0;
	char input[3];
	char ch;
	bool isend = false;
	while ((ch = _getch())) {
		if (((ch >= 65 && ch <= 90) || (ch >= 48 && ch <= 57) || (ch >= 97 && ch <= 122))) {//字母数字键
			if (idx == 0 && ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))) {
				std::cout << ch;
				input[idx] = ch;
				idx++;
			}
			else if (idx == 1 && ((ch >= 48 && ch <= 57))) {
				std::cout << ch;
				input[idx] = ch;
				idx++;
				input[idx] = '!';
			}
			else if (idx == 2 && ((input[1] == '0' && (ch >= 49 && ch <= 57)) || (input[1] == '1' && (ch >= 48 && ch <= 53)))) {
				std::cout << ch;
				input[idx] = ch;
				idx++;
			}

		}
		else if (ch == 13) {//回车键
			if (idx > 1) {
				int tmp;
				if (input[2] == '!') {
					tmp = (input[1] - '0');
				}
				else {
					tmp = (input[1] - '0') * 10 + input[2] - '0';
				}

				if (tmp <= 15 && tmp >= 1) {
					if (input[0] >= 'a'&&input[0] <= 'z') {
						input[0] -= 32;
					}
					int row = tmp - 1;
					int col = gui->alpha.find(input[0]);
					SetColor(0x84);
					if (col != gui->alpha.npos) {
						bool isok = this->setStep(row, col, isAImode);
						if (isok) {
							Piece winner = boardstate->winner();
							if (winner != Normal) {
								gui->showEndGame(winner);
								ch = _getch();
								isend = true;
								break;
							}
						}
						else {
							gui->showError(1);
							ch = _getch();
						}
					}
					else {
						gui->showError(0);
					}
					break;
				}
				else {
					gui->showError(0);
					break;
				}
			}

		}
		else if (ch == 8 && idx > 0) {//退格键
			std::cout << "\b \b";
			input[--idx]='!';
		}
		else if (ch == 96) {//~
			if(isAImode)
			this->unsetStep(2);
			else
				this->unsetStep(1);
			break;
		}
		else {
			continue;
		}
	}
	return isend;
}
void Controller::AIGame()//选择界面
{
	int key = 0;
	gui->showBoard();
	gui->showSelectPiece(key);
	int ch;//记录键入值

	bool flag = false;//记录是否键入Enter键标记，初始置为否
	while ((ch = _getch()))
	{
		switch (ch)//检测输入键
		{
		case 75://左
			if (key > 0)//当此时选中项为第一项时，UP上方向键无效
			{
				key--;
				gui->showSelectPiece(key);
			}
			break;

		case 77://右
			if (key < GUI::numOfselections - 1)
			{
				key++;
				gui->showSelectPiece(key);
			}
			break;

		case 13://Enter回车键
			flag = true;
			break;
		default://无效按键
			break;
		}
		if (flag) break;//输入Enter回车键确认，退出检查输入循环
	}
	gui->clearPanel();
	bool isend = false;
	while (1) {
		gui->showWaitForInput(true,(Piece)key);
		if ((boardstate->isblack&&key == White) || (!boardstate->isblack&&key == Black)) {
			isend = AITurn();
		}
		else {
			isend = PlayerTurn(true);
		}
		if (isend)break;
	}
	system("cls");
}
void Controller::MultiPlayerGame() {
	bool isend = false;
	gui->showBoard();
	while (1) {
		gui->showWaitForInput(false);
		isend = PlayerTurn(false);
		if (isend)break;
	}
	system("cls");
}
void Controller::unsetStep(int num,bool isAImode)
{
	if (!isAImode) {
		for (int i = 0; i < num; i++) {
			cell* tmppoint = boardstate->getLast(1);
			if (tmppoint != NULL && boardstate->unsetPiece()) {
				gui->unsetStep(tmppoint->row, tmppoint->col);
				tmppoint = boardstate->getLast(1);
				if (tmppoint != NULL) {
					gui->setStep(tmppoint->row, tmppoint->col, !boardstate->isblack, true);
				}
			}
		}
	}
	else {
		for (int i = 0; i < num; i++) {
			cell* tmppoint = boardstate->getLast(1);
			if (tmppoint != NULL && boardstate->unsetPiece()) {
				gui->unsetStep(tmppoint->row, tmppoint->col);
				int x[4] = { 0,1,1,1 };
				int y[4] = { 1,0,-1,1 };
				for (int i = 0; i < 4; i++) {
					for (int j = -4; j <= 4; j++) {
						cell* tmp = boardstate->getCell(tmppoint->row + x[i] * j, tmppoint->col + y[i] * j);
						if (tmp) {
							tmp->cand--;
							boardstate->board[tmp->row][tmp->col].updateScore();
							boardstate->eval[Black] -= tmp->eval[Black];
							boardstate->eval[White] -= tmp->eval[White];
							tmp->updatePattern();
							tmp->updateEval();
							boardstate->eval[Black] += tmp->eval[Black];
							boardstate->eval[White] += tmp->eval[White];
						}
						
					}
				}
				tmppoint = boardstate->getLast(1);
				if (tmppoint != NULL) {
					gui->setStep(tmppoint->row, tmppoint->col, !boardstate->isblack, true);
				}
			}
		}
	}
	return;
}
bool Controller::setStep(int row, int col,bool isAImode)
{
	if (!isAImode) {
		if (boardstate->setPiece(row, col, (Piece)boardstate->isblack)) {
			boardstate->getCell(row, col)->updatePattern();
			gui->setStep(row,col, !boardstate->isblack, true);
			cell * lastpoint = boardstate->getLast(2);
			if (lastpoint) {
				gui->setStep(lastpoint->row, lastpoint->col, boardstate->isblack);
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (boardstate->setPiece(row, col, (Piece)boardstate->isblack)) {
			gui->setStep(row, col, !boardstate->isblack, true);
			cell * lastpoint = boardstate->getLast(2);
			if (lastpoint) {
				gui->setStep(lastpoint->row, lastpoint->col, boardstate->isblack);
			}
			cell* tmppoint = boardstate->getLast(1);
			int x[4] = { 0,1,1,1 };
			int y[4] = { 1,0,-1,1 };
			for (int i = 0; i < 4; i++) {
				for (int j = -4; j <= 4; j++) {
					cell* tmp = boardstate->getCell(tmppoint->row + x[i] * j, tmppoint->col + y[i] * j);
					if (tmp) {
						tmp->cand++;
						boardstate->board[tmp->row][tmp->col].updateScore();
						boardstate->eval[Black] -= tmp->eval[Black];
						boardstate->eval[White] -= tmp->eval[White];
						tmp->updatePattern();
						tmp->updateEval();
						boardstate->eval[Black] += tmp->eval[Black];
						boardstate->eval[White] += tmp->eval[White];
					}
					
				}
			}
			return true;
		}
		else {
			return false;
		}
		
	}
}
int Controller::SelectMode() {
	int key = 0;
	int ch;//记录键入值
	gui->showSelectMode(key);
	bool flag = false;//记录是否键入Enter键标记，初始置为否
	while ((ch = _getch()))
	{
		if (ch == 224)ch = _getch();
		switch (ch)//检测输入键
		{
		case 75://左
			if (key > 0)//当此时选中项为第一项时，UP上方向键无效
			{
				key--;
				gui->showSelectMode(key);
			}
			break;

		case 77://右
			if (key < GUI::numOfselections - 1)
			{
				key++;
				gui->showSelectMode(key);
			}
			break;

		case 13://Enter回车键
			flag = true;
			break;
		default://无效按键
			break;
		}
		if (flag) break;//输入Enter回车键确认，退出检查输入循环
	}
	return key;
}
void Controller::Game()
{
	SetWindowSize(height, width);
	while (1) {
		system("color 8e");
		int key = SelectMode();
		Mode mode = (Mode)key;
		if (mode == MultiPlayer) {
			MultiPlayerGame();
		}
		else if (mode == AI) {
			AIGame();
		}
		boardstate->reset();
	}
}
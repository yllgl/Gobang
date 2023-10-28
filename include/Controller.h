#ifndef CONTROLLER_H
#define CONTROLLER_H
#include<iostream>
#include"GUI.h"
#include"BoardState.h"
class Controller
{
public:
	GUI*gui;
	BoardState* boardstate;
	int key;
	enum Mode { MultiPlayer = 0, AI };
	int height;
	int width;
	
	int gap = 8;
	Controller() {
		height = 30, width = 30;
		boardstate = new BoardState();
		gui = new GUI(1, 1, boardstate);
	}
	Controller(int length) {
		height = 30, width = 30;
		boardstate = new BoardState(length);
		gui = new GUI(1, 1, boardstate);
	}
	Controller(int height, int width, int length) {
		boardstate = new BoardState(length);
		gui = new GUI(1, 1, boardstate);
		this->height = height;
		this->width = width;
	}
	~Controller() {
		delete gui;
		delete boardstate;
	}
	int SelectMode();
	void Game();
	void MultiPlayerGame();
	void unsetStep(int num, bool isAImode = false);
	bool setStep(int row, int col, bool isAImode = false);
	bool AITurn();
	bool PlayerTurn(bool isAImode);
	void AIGame();
};

#endif
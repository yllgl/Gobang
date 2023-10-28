#ifndef GUI_H
#define GUI_H
#include"cell.h"
#include <string>
#include"tools.h"
using namespace std;
class GUI
{
public:
	int origin_x, origin_y;
	int output_row, output_col;
	BoardState* boardstate;
	int length;
	static string alpha;
	static string fullalpha;
	static const int numOfselections;
	static const int gap;
	static string selections[];

	GUI() {
		origin_x = 1;
		origin_y = 1;
		length = 0;
		output_row = origin_y + length + 7;
		output_col = 0;
		boardstate = NULL;
	}


	GUI(int row, int col, BoardState* bs) {
		origin_x = col;
		origin_y = row;
		boardstate = bs;
		length = bs->length;
		output_row = origin_y + length + 7;
		output_col = 0;
	}
	void showBoard();
	void setStep(int row, int col, bool isblack, bool islast = false);
	void unsetStep(int row, int col);

	void showWaitForInput(bool iscomputer, Piece piece = Black);

	void showSelectMode(int key);

	void showSelectPiece(int key);

	void showEndGame(Piece result);

	void showError(int mode);

	void clearPanel();
	
};

#endif
#ifndef BOARDSTATE_H
#define BOARDSTATE_H
#include <vector>
#include <random>
#include <limits>
#include<unordered_set>
#include<set>
class cell;
using namespace std;
class BoardState
{
public:
	vector<vector<cell> > board;
	int length;
	bool isblack;
	int cntOfPiece;
	vector<cell> history;
	int eval[2];
	static int MAX_DEPTH;
	set<cell> searchSpace;
	cell* nextStep;
	vector<cell> BlockFourFlexThree;
	bool isBlockFourFlexThree;
#ifdef _DEBUG
	vector<cell> trace;
#endif
	BoardState();
	BoardState(int length);
	Piece getPiece(int row, int col);

	cell* getCell(int row, int col);

	bool setPiece(int row, int col, Piece p);

	cell* getLast(int numOfLast);

	bool unsetPiece();

	Piece getSelf();

	Piece getOppo();

	Piece winner();

	cell* getNextPoint();

	void reset();

	int evaluate(Piece self);

	int alphabeta(int depth, int alpha, int beta);
};
#endif

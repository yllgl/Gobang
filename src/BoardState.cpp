#include "cell.h"
#include"tools.h"
#include<algorithm>
/*#ifdef _DEBUG
#undef _DEBUG
#endif*/
int BoardState::MAX_DEPTH = 2;
BoardState::BoardState() {
	length = 15;
	isblack = true;
	cntOfPiece = 0;
	eval[0] =eval[1] = 0;
	nextStep = NULL;
	isBlockFourFlexThree = false;
	board = vector<vector<cell> >(length, vector<cell>());
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			board[i].push_back(cell(i, j, this));
		}
	}
}
BoardState::BoardState(int length)
{
	isblack = true;
	cntOfPiece = 0;
	eval[0] = eval[1] = 0;
	nextStep = NULL;
	isBlockFourFlexThree = false;
	this->length = length;
	board = vector<vector<cell> >(length, vector<cell>());
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			board[i].push_back(cell(i, j, this));
		}
	}
}
Piece BoardState::getPiece(int row, int col) {
	if (row < 0 || col < 0 || row >= length || col >= length) {
		return Wrong;
	}
	return board[row][col].piece;
}
cell*BoardState::getCell(int row, int col) {
	if (row < 0 || col < 0 || row >= length || col >= length) {
		return NULL;
	}
	return &board[row][col];
}
bool BoardState::setPiece(int row, int col, Piece p) {
	if (getPiece(row, col) == Empty) {
		if (getSelf() == p) {
			board[row][col].piece = p;
			isblack = !isblack;
			history.push_back(board[row][col]);
			cntOfPiece++;
			getCell(row, col)->updatePattern();
			if (getCell(row,col)->pattern[Block][Four] + getCell(row, col)->pattern[Flex][Three] > 0) {
				isBlockFourFlexThree = true;
				BlockFourFlexThree.push_back(*getCell(row, col));
			}
			return true;
		}
		else {
			OutputDebugPrintf("set Piece error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		}
	}
	else if (p == Empty) {
		if (getPiece(row, col) == history.back().piece) {
			board[row][col].piece = p;
			history.pop_back();
			isblack = !isblack;
			isBlockFourFlexThree = false;
			BlockFourFlexThree.clear();
			return true;
		}
		else {
			OutputDebugPrintf("set Empty error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			return false;
		}
	}
	return false;
}
cell* BoardState::getLast(int numOfLast) {
	if (history.size() >= numOfLast) {
		cell tmp = history[history.size() - numOfLast];
		return &board[tmp.row][tmp.col];
	}
	return NULL;
}
bool BoardState::unsetPiece() {
	cell* tmp = getLast(1);
	Piece p = tmp->piece;
	if (tmp) {
		if (setPiece(tmp->row, tmp->col, Empty)) {
			cntOfPiece--;
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}
Piece BoardState::getSelf() {
	if (isblack) {
		return Black;
	}
	else {
		return White;
	}
}
Piece BoardState::getOppo() {
	if (isblack) {
		return White;
	}
	else {
		return Black;
	}
}
Piece BoardState::winner() {
	cell* tmp = getLast(1);
	if (tmp) {
		int (*pattern)[5] = tmp->getPattern();
		if (pattern[Block][Five] > 0 || pattern[Flex][Five] > 0) {
			return tmp->piece;
		}
		if (tmp->piece==Black&&(pattern[Flex][Four] >= 2 || pattern[Flex][Three] >= 2 || pattern[Flex][Long] != 0 || pattern[Block][Long] != 0)) {
			return Wrong;
		}
		if (history.size() == length * length) {
			return Empty;
		}
	}
	return Normal;
}
cell* BoardState::getNextPoint() {
	if (cntOfPiece == 0) {
		return &board[length/2][length/2];
	}
	else {
		searchSpace.clear();
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				if (getPiece(i, j) == Empty && getCell(i, j)->cand > 0) {
					searchSpace.insert(*getCell(i, j));
				}
			}
		}
		int val;
		val = alphabeta(MAX_DEPTH, -INF, INF);
		isBlockFourFlexThree = false;
		BlockFourFlexThree.clear();
		return nextStep;
	}
}
void BoardState::reset() {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			board[i][j].clear();
		}
	}
	history.clear();
	isblack = true;
	for (int i = 0; i < MAX_DEPTH; i++) {
		searchSpace.clear();
	}
	cntOfPiece = 0;
	if (nextStep)
		delete nextStep;
	nextStep = NULL;
	eval[Black] = 0;
	eval[White] = 0;
}
int BoardState::evaluate(Piece self) {
	return eval[self] - eval[!self];
}
bool operator < (const cell&stu1, const cell&stu2) {

	if (stu1.getScore() > stu2.getScore()) {
		return true;
	}
	else if (stu1.getScore() == stu2.getScore()) {
		if ((abs(stu1.row - 7) + abs(stu1.col - 7) < abs(stu2.row - 7) + abs(stu2.col - 7))) {
			return true;
		}
		else if ((abs(stu1.row - 7) + abs(stu1.col - 7) == abs(stu2.row - 7) + abs(stu2.col - 7))) {
			int a = 0, b = 0;
			for (int i = 3; i >= 0; i--) {
				a += stu1.pattern[Flex][i];
				b += stu2.pattern[Flex][i];
			}
			if (a > b) {
				return true;
			}
			else if (a == b) {
				int a = 0, b = 0;
				for (int i = 3; i >= 0; i--) {
					a += stu1.pattern[Block][i];
					b += stu2.pattern[Block][i];
				}
				if (a > b) {
					return true;
				}
			}
		}
	}
	return false;
}
int BoardState::alphabeta(int depth, int alpha, int beta) {
	if (depth <= 0) {
		int score = evaluate(getSelf());
#ifdef _DEBUG
		for (int i = 0; i < trace.size(); i++) {
			OutputDebugPrintf("[%c,%d],", trace[i].col + 'A', trace[i].row + 1);
		}
#endif
		return score;
	}
#ifdef _DEBUG
	OutputDebugPrintf("depth:%d,searchSpace:",depth);
	for (auto it = searchSpace.begin(); it != searchSpace.end(); it++) {
		OutputDebugPrintf("[%c,%d],", it->col + 'A', it->row + 1);
	}
	OutputDebugPrintf("\n");
	OutputDebugPrintf("trace:");
	for (int i = 0; i < trace.size(); i++) {
		OutputDebugPrintf("[%c,%d],", trace[i].col + 'A', trace[i].row + 1);
	}
#endif
	int n = searchSpace.size();
	int value = -INF;
	bool eval_is_exact = false;
	auto it = searchSpace.begin();
	for (int i = 0; i < n&&it != searchSpace.end(); i++, it++) {
		const cell* tmppoint = &(*it);
		if (setPiece(tmppoint->row, tmppoint->col,(Piece)isblack)) {
			int x[4] = { 0,1,1,1 };
			int y[4] = { 1,0,-1,1 };
			#ifdef _DEBUG
			trace.push_back(cell(*tmppoint));
			#endif
			for (int i = 0; i < 4; i++) {
				for (int j = -4; j <= 4; j++) {
					cell* tmp = getCell(tmppoint->row + x[i] * j, tmppoint->col + y[i] * j);
					if (tmp) {
						tmp->cand++;
						if (tmp->piece == Empty && tmp->cand > 0) {
							searchSpace.insert(*tmp);
						}
						board[tmp->row][tmp->col].updateScore();
						eval[Black] -= tmp->eval[Black];
						eval[White] -= tmp->eval[White];
						tmp->updatePattern();
						tmp->updateEval();
						eval[Black] += tmp->eval[Black];
						eval[White] += tmp->eval[White];
					}
				}
			}
			value = max(-alphabeta(depth - 1, -beta, -alpha),value);
#ifdef _DEBUG
			for (int i = 0; i < trace.size(); i++) {
				OutputDebugPrintf("[%c,%d],",trace[i].col+'A', trace[i].row + 1);
			}
			OutputDebugPrintf("depth:%d,value:%d,alpha:%d,beta:%d\n",depth,value,alpha,beta);
#endif
			if (unsetPiece()) {
				int x[4] = { 0,1,1,1 };
				int y[4] = { 1,0,-1,1 };
				if (tmppoint->piece == Empty) {
					for (int i = 0; i < 4; i++) {
						for (int j = -4; j <= 4; j++) {
							cell* tmp = getCell(tmppoint->row + x[i] * j, tmppoint->col + y[i] * j);
							if (tmp) {
								tmp->cand--;
								board[tmp->row][tmp->col].updateScore();
								eval[Black] -= tmp->eval[Black];
								eval[White] -= tmp->eval[White];
								tmp->updatePattern();
								tmp->updateEval();
								eval[Black] += tmp->eval[Black];
								eval[White] += tmp->eval[White];
							}
							
						}
					}
#ifdef _DEBUG
					trace.pop_back();
#endif
					if (value > alpha) {
						alpha = value;
						if (depth == MAX_DEPTH) {
							if (nextStep)
								delete nextStep;
							nextStep = new cell(*tmppoint);
						}
					}
					if (alpha >= beta) {
						return value;
					}
				}
				else {
					OutputDebugPrintf("unsetPiece error!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
	}
	return value;
}
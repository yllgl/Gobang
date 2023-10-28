#ifndef CELL_H
#define CELL_H
#include"tools.h"
enum Piece {
	White = 0, Black,Empty,Wrong,Normal
};
enum PieceLength {
	Two=0,Three=1,Four=2,Five=3,Long=4
};
enum PatternType {
	Block=0,Flex=1
};
#include"BoardState.h"
class cell
{
public:
	int row, col;
	int eval[2];
	int score[2];
	int cand;
	Piece piece;
	int pattern[2][5];
	BoardState* boardstate;
	cell(int row,int col, BoardState* bs) {
		cand = 0;
		piece = Empty;
		this->row = row;
		this->col = col;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 5; j++) {
				pattern[i][j] = 0;
			}
			eval[i] = 0;
			score[i] = 0;
		}
		boardstate = bs;
	};
	inline cell & operator =(const cell &p) {
		this->row = p.row;
		this->col = p.col;
		this->piece = p.piece;
		this->cand = p.cand;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 5; j++) {
				this->pattern[i][j] = p.pattern[i][j];
			}
			this->eval[i] = p.eval[i];
			this->score[i] = p.score[i];
		}
		this->boardstate = p.boardstate;
		return *this;
	}
	cell(const cell &p) {
		this->row = p.row;
		this->col = p.col;
		this->piece = p.piece;
		this->cand = p.cand;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 5; j++) {
				this->pattern[i][j] = p.pattern[i][j];
			}
			this->eval[i] = p.eval[i];
			this->score[i] = p.score[i];
		}
		this->boardstate = p.boardstate;
	}

	int(*getPattern())[5];

	void clear();
	void updatePattern();
	void updateEval();
	void updateScore();
	int getScore() const;
	int getScore();
};
#endif


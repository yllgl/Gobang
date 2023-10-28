#include "cell.h"
#include"tools.h"
#include<vector>
using namespace std;
int (* cell::getPattern())[5] {
	return pattern;
}
void cell::clear() {
	this->piece = Empty;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			this->pattern[i][j] = 0;
		}
		this->eval[i] = 0;
		this->score[i] = 0;
	}
	cand = 0;
}

void cell::updatePattern()
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			pattern[i][j] = 0;
		}
	}
	if (piece == Empty)return;
	int x[4] = { 0,1,1,1 };
	int y[4] = { 1,0,-1,1 };
	int numOfconnections = 0;
	for (int i = 0; i < 4; i++) {
		int count1 = 0;
		bool existGap = true;
		int line[5] = { 0 };
		int idx = 2;
		int j1 = 1;
		while (1) {
			if (boardstate->getPiece(row+x[i]*j1,col+y[i]*j1) == piece) {
				line[idx]++;
			}
			else if (boardstate->getPiece(row + x[i]*j1, col + y[i]*j1) == Empty && existGap) {
				idx++;
				existGap = !existGap;
			}
			else {
				idx++;
				if (boardstate->getPiece(row + x[i] * j1, col + y[i] * j1) == (Piece)(!piece)) {
					line[idx] = -1;
				}
				break;
			}
			j1++;
		}
		int j2 = -1;
		idx = 2;
		existGap = true;
		while (1) {
			if (boardstate->getPiece(row + x[i] * j2, col + y[i] * j2) == piece) {
				line[idx]++;
			}
			else if (boardstate->getPiece(row + x[i] * j2, col + y[i] * j2) == Empty && existGap) {
				idx--;
				existGap = !existGap;
			}
			else {
				idx--;
				if (boardstate->getPiece(row + x[i] * j2, col + y[i] * j2) == (Piece)(!piece)) {
					line[idx] = -1;
				}
				break;
			}
			j2--;
		}
		line[2]++;
		//line[i]==0说明占一个空格,line[i]==digit说明占一个空格加连续的棋子,line[i]==-1说明占一个对方棋子
		if (line[2] > 5) {
			pattern[Block][Long] = 1;
			pattern[Flex][Long] = 1;
		}
		if (line[2] == 5) {
			pattern[Block][Five]=1;
			pattern[Flex][Five]=1;
		}
		if (line[0] != -1 && line[1] != -1 && line[3] != -1 && line[4] != -1) {
			//以下*代表任意个o或一个_
			if (line[2]<4&&line[1] + line[2] == 4) {//_o_ooo_*  _oo_oo_*
				pattern[Flex][Four]++;
			}
			if (line[2] < 4 && line[2] + line[3] == 4) {//*_ooo_o_  *_oo_oo_
				pattern[Flex][Four]++;
			}
			if ((line[1] == 0 || line[3] == 0) && line[2] == 4) {//*_oooo__   __oooo_*
				pattern[Flex][Four]++;
			}
			if (line[2] < 3 && line[1] + line[2] == 3) {//_o_oo_*  _oo_o_*
				pattern[Flex][Three]++;
			}
			if (line[2] < 3 && line[2] + line[3] == 3) {// *_oo_o_   *_o_oo_
				pattern[Flex][Three]++;
			}
			if (line[1] == 0 && line[3] == 0 && line[2] == 3&&(line[0]==0||line[4]==0)) { //  *_ooo_*  两个*至少有一个是_
				pattern[Flex][Three]++;
			}
			if (line[2] == 2 && line[1] == 1 && line[3] == 1) { // _o_oo_o_
				pattern[Flex][Three]--;//去除重复计算情况
			}
			if (line[2] < 2 && line[1] + line[2] == 2) { // _o_o_*
				pattern[Flex][Two]++;
			}
			if (line[2] < 2 && line[2] + line[3] == 2) {// *_o_o_
				pattern[Flex][Two]++;
			}
			if ((line[1] == 0 || line[3] == 0) && line[2] == 2) {//__oo__
				pattern[Flex][Two]++;
			}
		}
		else {
			//以下*代表x或_
			if (line[2] < 4 && line[1] + line[2] == 4) {// *o_ooo_x  *oo_oo_x xo_ooo_*  xoo_oo_*
				pattern[Block][Four]++;
			}
			if (line[2] < 4 && line[2] + line[3] == 4) {// *_ooo_o_   *_oo_oo_ 
				pattern[Block][Four]++;
			}
			if (((line[1] == -1 && line[3] == 0) || (line[1] == 0 && line[3] == -1)) && line[2] == 4) {
				pattern[Block][Four]++;
			}
			if ((line[1] == 0 && line[3] == 0) && line[2] == 4) {
				pattern[Flex][Four]++;
			}
			if (line[2] < 3 && line[1] + line[2] == 3) {
				if ((line[0] == -1 && line[3] == 0) || (line[0] == 0 && line[3] == -1)) {//  xo_oo_*  _o_oox
					pattern[Block][Three]++;
				}
				if (line[0] == 0 && line[3] == 0) {//_o_oo_x
					pattern[Flex][Three]++;
				}

			}
			if (line[2] < 3 && line[2] + line[3] == 3) {
				if ((line[1] == -1 && line[4] == 0) || (line[1] == 0 && line[4] == -1)) { // xo_oo_   *_o_oox
					pattern[Block][Three]++;
				}
				if (line[1] == 0 && line[4] == 0) {
					pattern[Flex][Three]++;
				}

			}
			if (line[1] == 0 && line[3] == 0 && line[2] == 3 && (line[0] == 0 || line[4] == 0)) { //  *_ooo_*  两个*至少有一个是_
				pattern[Flex][Three]++;
			}
			if ((line[1] == -1 && line[3] == 0 && line[2] == 3 && line[4] == 0) || (line[3] == -1 && line[1] == 0 && line[2] == 3 && line[0] == 0)) {
				pattern[Block][Three]++;
			}
			if (line[2] < 2 && line[1] + line[2] == 2) {
				if ((line[0] == -1 && line[3] == 0 && line[4] == 0) || (line[0] == 0 && line[3] == -1 && boardstate->getPiece(row + x[i] * (j2 - 1), col + y[i] * (j2 - 1)) == Empty)) { // xo_o__  __o_ox 
					pattern[Block][Two]++;
				}
				if (line[0] == 0 && line[3] == 0) {
					if (boardstate->getPiece(row + x[i] * (j2 - 1), col + y[i] * (j2 - 1)) == Empty) {//__o_o_x
						pattern[Flex][Two]++;
					}
					else {
						pattern[Block][Two]++;
					}
				}
			}
			if (line[2] < 2 && line[2] + line[3] == 2) { // __o_ox  xo_o__
				if ((line[4] == -1 && line[1] == 0 && line[0] == 0) || (line[4] == 0 && line[1] == -1 && boardstate->getPiece(row + x[i] * (j1 + 1), col + y[i] * (j1 + 1)) == Empty)) {
					pattern[Block][Two]++;
				}
				if (line[1] == 0 && line[4] == 0) {
					if (boardstate->getPiece(row + x[i] * (j1 + 1), col + y[i] * (j1 + 1)) == Empty) {//  x_o_o_
						pattern[Flex][Two]++;
					}
					else {
						pattern[Block][Two]++;
					}
				}
			}
			if (line[1] == 0 && line[3] == 0 && line[2] == 2 && ((line[0] == 0 && boardstate->getPiece(row + x[i] * (j2 - 1), col + y[i] * (j2 - 1)) == Empty) || (line[4] == 0 && boardstate->getPiece(row + x[i] * (j1 + 1), col + y[i] * (j1 + 1)) == Empty))) { // __oo_x
				pattern[Flex][Two]++;
			}
			if ((line[1] == -1 && line[3] == 0 && line[2] == 2 && line[4] == 0 && boardstate->getPiece(row + x[i] * (j1 + 1), col + y[i] * (j1 + 1)) == Empty) || (line[3] == -1 && line[1] == 0 && line[2] == 2 && line[0] == 0 && boardstate->getPiece(row + x[i] * (j2 - 1), col + y[i] * (j2 - 1)) == Empty)) {
				pattern[Block][Two]++;
			}
		}
	}
}
void cell::updateEval() {
	eval[Black] = 0;
	eval[White] = 0;
	if (piece == Empty) {
		return;
	}
	else {
		int base = 10;
		if (piece == Black&& (pattern[Flex][Three] >= 2 || (pattern[Flex][Four] + pattern[Block][Four] >= 2) || (pattern[Block][Long] + pattern[Flex][Long] >= 1))) {
			eval[Black] = -WIN_MAX;
		}
		else {
			if (pattern[Flex][Five] > 0 || pattern[Block][Five] > 0) {
				eval[piece] = WIN_MAX;
			}
			int base = 10;
			for (int i = 0; i < 4; i++) {
				eval[piece] += base * pattern[Flex][i];
				base *= 10;
			}
			base = 2;
			for (int i = 0; i < 4; i++) {
				eval[piece] += base * pattern[Block][i];
				base *= 10;
			}
			if (pattern[Flex][Four] + pattern[Block][Four] >= 1 && pattern[Flex][Three] >= 1) {
				eval[piece] += 1000 * pattern[Flex][Three];
			}
		}
	}
	return;
}
void cell::updateScore() {
	score[Black] = 0;
	score[White] = 0;
	if (piece == Empty) {
		piece = Black;
		updatePattern();
		if ((pattern[Flex][Three] >= 2 || (pattern[Flex][Four] + pattern[Block][Four] >= 2) || (pattern[Block][Long] + pattern[Flex][Long] >= 1))) {
			score[Black] = - WIN_MAX;
		}
		else {
			int base = 10;
			if (pattern[Flex][Five] > 0 || pattern[Block][Five] > 0) {
				score[piece] = WIN_MAX;
			}
			for (int i = 0; i < 4; i++) {
				score[piece] += base * pattern[Flex][i];
				base *= 10;
			}
			base = 2;
			for (int i = 0; i < 4; i++) {
				score[piece] += base * pattern[Block][i];
				base *= 10;
			}
			if (pattern[Flex][Four] + pattern[Block][Four] >= 1 && pattern[Flex][Three] >= 1) {
				score[piece] += 1000 * pattern[Flex][Three];
			}
		}
		piece = White;
		updatePattern();
		if (pattern[Flex][Five] > 0 || pattern[Block][Five] > 0 || (pattern[Block][Long] + pattern[Flex][Long] >= 1)) {
			score[piece] = WIN_MAX;
		}
		int base = 10;
		for (int i = 0; i < 4; i++) {
			score[piece] += base * pattern[Flex][i];
			base *= 10;
		}
		base = 2;
		for (int i = 0; i < 4; i++) {
			score[piece] += base * pattern[Block][i];
			base *= 10;
		}
		if (pattern[Flex][Four] + pattern[Block][Four] >= 1 && pattern[Flex][Three] >= 1) {
			score[piece] += 1000 * pattern[Flex][Three];
		}
		piece = Empty;
		return;
	}
	else {
		score[Black] = 0;
		score[White] = 0;
		return;
	}
}
int cell::getScore()const  {
	return score[boardstate->getSelf()] + score[boardstate->getOppo()] + score[boardstate->getSelf()];
}
int cell::getScore() {
	return score[boardstate->getSelf()] + score[boardstate->getOppo()] + score[boardstate->getSelf()];
}
#include "SpertApp.h"

Position::Position() : side(white), wCastle(both), bCastle(both), enPassant(0),
		inCheck(false), wKingCol(e), wKingRow(1), bKingCol(e), bKingRow(8), evaluation(0),
		material(0), figureCount(14), fiftyMoveChecker(0), wBishops(2), bBishops(2),
		wQueens(1), bQueens(1), wPawnsOnLight(4), wPawnsOnDark(4), bPawnsOnLight(4), bPawnsOnDark(4),
		halfMoves(0)
{
	for (size_t i = 0; i < 10; ++i)
		for (size_t j = 0; j < 10; ++j)
			board[i][j] = initBOARD[i][j];
}

Position::Position(const Position& pos) : side(pos.side), wCastle(pos.wCastle), bCastle(pos.bCastle),
	enPassant(pos.enPassant), inCheck(pos.inCheck), wKingCol(pos.wKingCol),
	wKingRow(pos.wKingRow), bKingCol(pos.bKingCol), bKingRow(pos.bKingRow),
	evaluation(pos.evaluation), material(pos.material), figureCount(pos.figureCount),
	fiftyMoveChecker(pos.fiftyMoveChecker), wBishops(pos.wBishops), bBishops(pos.bBishops),
	wQueens(pos.wQueens), bQueens(pos.bQueens), wPawnsOnLight(pos.wPawnsOnLight),
	wPawnsOnDark(pos.wPawnsOnDark), bPawnsOnLight(pos.bPawnsOnLight),
	bPawnsOnDark(pos.bPawnsOnDark), halfMoves(pos.halfMoves)
{
	for (size_t i = 0; i < 10; ++i)
		for (size_t j = 0; j < 10; ++j)
			board[i][j] = pos.board[i][j];
}

bool Position::operator==(const Position& pos) const
{
	if (side != pos.side)
		return false;
	if (wCastle != pos.wCastle || bCastle != pos.bCastle)
		return false;
	if (enPassant != pos.enPassant)
		return false;
	for (size_t i = 1; i < 9; ++i)
		for (size_t j = 1; j < 9; ++j)
			if (board[i][j] != pos.board[i][j])
				return false;
	return true;
}

bool Position::operator!=(const Position& pos) const
{
	return !(*this == pos);
}

void Position::SwitchSide()
{
	side = side == white ? black : white;
}

bool Position::WhiteAttack(size_t y, size_t x) const		// white piece is controlling square
{
	// pawn attack
	if (y - 1 > 0 && y - 1 < 9 && x - 1 > 0 && x - 1 < 9
		&& board[x - 1][y - 1] == wP)
		return true;
	if (y + 1 > 0 && y + 1 < 9 && x - 1 > 0 && x - 1 < 9
		&& board[x - 1][y + 1] == wP)
		return true;

	// knight attack
	if (x - 2 > 0 && x - 2 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x - 2][y + 1] == wN)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y + 2 > 0 && y + 2 < 9
		&& board[x - 1][y + 2] == wN)
		return true;
	if (x - 2 > 0 && x - 2 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x - 2][y - 1] == wN)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y - 2 > 0 && y - 2 < 9
		&& board[x - 1][y - 2] == wN)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y - 2 > 0 && y - 2 < 9
		&& board[x + 1][y - 2] == wN)
		return true;
	if (x + 2 > 0 && x + 2 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x + 2][y - 1] == wN)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y + 2 > 0 && y + 2 < 9
		&& board[x + 1][y + 2] == wN)
		return true;
	if (x + 2 > 0 && x + 2 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x + 2][y + 1] == wN)
		return true;

	// bishop / queen attack
	size_t d = 1;
	while (x + d > 0 && x + d < 9 && y + d > 0 && y + d < 9)
	{
		if (board[x + d][y + d] == 0)
			++d;
		else if (board[x + d][y + d] == wB
			|| board[x + d][y + d] == wQ)
			return true;
		else break;
	}
	d = 1;
	while (x + d > 0 && x + d < 9 && y - d > 0 && y - d < 9)
	{
		if (board[x + d][y - d] == 0)
			++d;
		else if (board[x + d][y - d] == wB
			|| board[x + d][y - d] == wQ)
			return true;
		else break;
	}
	d = 1;
	while (x - d > 0 && x - d < 9 && y + d > 0 && y + d < 9)
	{
		if (board[x - d][y + d] == 0)
			++d;
		else if (board[x - d][y + d] == wB
			|| board[x - d][y + d] == wQ)
			return true;
		else break;
	}
	d = 1;
	while (x - d > 0 && x - d < 9 && y - d > 0 && y - d < 9)
	{
		if (board[x - d][y - d] == 0)
			++d;
		else if (board[x - d][y - d] == wB
			|| board[x - d][y - d] == wQ)
			return true;
		else break;
	}

	// rook / queen attack
	d = 1;
	while (x + d > 0 && x + d < 9)
	{
		if (board[x + d][y] == 0)
			++d;
		else if (board[x + d][y] == wR
			|| board[x + d][y] == wQ)
			return true;
		else break;
	}
	d = 1;
	while (x - d > 0 && x - d < 9)
	{
		if (board[x - d][y] == 0)
			++d;
		else if (board[x - d][y] == wR
			|| board[x - d][y] == wQ)
			return true;
		else break;
	}
	d = 1;
	while (y + d > 0 && y + d < 9)
	{
		if (board[x][y + d] == 0)
			++d;
		else if (board[x][y + d] == wR
			|| board[x][y + d] == wQ)
			return true;
		else break;
	}
	d = 1;
	while (y - d > 0 && y - d < 9)
	{
		if (board[x][y - d] == 0)
			++d;
		else if (board[x][y - d] == wR
			|| board[x][y - d] == wQ)
			return true;
		else break;
	}

	// king attack
	if (x - 1 > 0 && x - 1 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x - 1][y - 1] == wK)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y > 0 && y < 9
		&& board[x - 1][y] == wK)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x - 1][y + 1] == wK)
		return true;
	if (x > 0 && x < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x][y - 1] == wK)
		return true;
	if (x > 0 && x < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x][y + 1] == wK)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x + 1][y - 1] == wK)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y > 0 && y < 9
		&& board[x + 1][y] == wK)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x + 1][y + 1] == wK)
		return true;

	// no attack:
	return false;
}

bool Position::BlackAttack(size_t y, size_t x) const		// black piece is controlling square
{
	if (y - 1 > 0 && y - 1 < 9 && x + 1 > 0 && x + 1 < 9
		&& board[x + 1][y - 1] == bP)
		return true;
	if (y + 1 > 0 && y + 1 < 9 && x + 1 > 0 && x + 1 < 9
		&& board[x + 1][y + 1] == bP)
		return true;

	// knight attack
	if (x - 2 > 0 && x - 2 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x - 2][y + 1] == bN)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y + 2 > 0 && y + 2 < 9
		&& board[x - 1][y + 2] == bN)
		return true;
	if (x - 2 > 0 && x - 2 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x - 2][y - 1] == bN)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y - 2 > 0 && y - 2 < 9
		&& board[x - 1][y - 2] == bN)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y - 2 > 0 && y - 2 < 9
		&& board[x + 1][y - 2] == bN)
		return true;
	if (x + 2 > 0 && x + 2 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x + 2][y - 1] == bN)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y + 2 > 0 && y + 2 < 9
		&& board[x + 1][y + 2] == bN)
		return true;
	if (x + 2 > 0 && x + 2 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x + 2][y + 1] == bN)
		return true;

	// bishop / queen attack
	size_t d = 1;
	while (x + d > 0 && x + d < 9 && y + d > 0 && y + d < 9)
	{
		if (board[x + d][y + d] == 0)
			++d;
		else if (board[x + d][y + d] == bB
			|| board[x + d][y + d] == bQ)
			return true;
		else break;
	}
	d = 1;
	while (x + d > 0 && x + d < 9 && y - d > 0 && y - d < 9)
	{
		if (board[x + d][y - d] == 0)
			++d;
		else if (board[x + d][y - d] == bB
			|| board[x + d][y - d] == bQ)
			return true;
		else break;
	}
	d = 1;
	while (x - d > 0 && x - d < 9 && y + d > 0 && y + d < 9)
	{
		if (board[x - d][y + d] == 0)
			++d;
		else if (board[x - d][y + d] == bB
			|| board[x - d][y + d] == bQ)
			return true;
		else break;
	}
	d = 1;
	while (x - d > 0 && x - d < 9 && y - d > 0 && y - d < 9)
	{
		if (board[x - d][y - d] == 0)
			++d;
		else if (board[x - d][y - d] == bB
			|| board[x - d][y - d] == bQ)
			return true;
		else break;
	}

	// rook / queen attack
	d = 1;
	while (x + d > 0 && x + d < 9)
	{
		if (board[x + d][y] == 0)
			++d;
		else if (board[x + d][y] == bR
			|| board[x + d][y] == bQ)
			return true;
		else break;
	}
	d = 1;
	while (x - d > 0 && x - d < 9)
	{
		if (board[x - d][y] == 0)
			++d;
		else if (board[x - d][y] == bR
			|| board[x - d][y] == bQ)
			return true;
		else break;
	}
	d = 1;
	while (y + d > 0 && y + d < 9)
	{
		if (board[x][y + d] == 0)
			++d;
		else if (board[x][y + d] == bR
			|| board[x][y + d] == bQ)
			return true;
		else break;
	}
	d = 1;
	while (y - d > 0 && y - d < 9)
	{
		if (board[x][y - d] == 0)
			++d;
		else if (board[x][y - d] == bR
			|| board[x][y - d] == bQ)
			return true;
		else break;
	}

	// king attack
	if (x - 1 > 0 && x - 1 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x - 1][y - 1] == bK)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y > 0 && y < 9
		&& board[x - 1][y] == bK)
		return true;
	if (x - 1 > 0 && x - 1 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x - 1][y + 1] == bK)
		return true;
	if (x > 0 && x < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x][y - 1] == bK)
		return true;
	if (x > 0 && x < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x][y + 1] == bK)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y - 1 > 0 && y - 1 < 9
		&& board[x + 1][y - 1] == bK)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y > 0 && y < 9
		&& board[x + 1][y] == bK)
		return true;
	if (x + 1 > 0 && x + 1 < 9 && y + 1 > 0 && y + 1 < 9
		&& board[x + 1][y + 1] == bK)
		return true;

	// no attack:
	return false;
}

bool Position::IsLegal() const
{
	// own king is in check and opponent to move, position is impossible:
	if (side == white && WhiteAttack(bKingCol, bKingRow))
		return false;
	if (side == black && BlackAttack(wKingCol, wKingRow))
		return false;

	// if not, position is legal:
	return true;
}
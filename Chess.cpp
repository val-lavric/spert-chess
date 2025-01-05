#include "SpertApp.h"
#include <cmath>

void Chess::GenerateMoves(Position& position)		// every generated move will be added to Chess::moves
{
	for (size_t i = 1; i < 9; ++i)
	{
		for (size_t j = 1; j < 9; ++j)
		{
			int p = position.board[i][j];
			if (position.side == white && p <= 0)	// only white pieces move
				continue;
			if (position.side == black && p >= 0)	// only black pieces move
				continue;
			switch (p)
			{
			case wN:
			case bN:
				KnightMoves(position, j, i);
				break;
			case wB:
			case bB:
				BishopMoves(position, j, i);
				break;
			case wR:
			case bR:
				RookMoves(position, j, i);
				break;
			case wQ:
			case bQ:
				QueenMoves(position, j, i);
				break;
			case wP:
			case bP:
				PawnMoves(position, j, i);
				break;
			case wK:
			case bK:
				KingMoves(position, j, i);
				break;
			default:
				break;
			}
		}
	}
}

void Chess::KnightMoves(Position& position, size_t j, size_t i)
{
	int N = position.side == white ? wN : bN;

	if (i - 2 > 0 && i - 2 < 9 && j + 1 > 0 && j + 1 < 9 && position.board[i - 2][j + 1] * N <= 0)
	{
		Mutare move(N, j, i, j + 1, i - 2);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i - 1 > 0 && i - 1 < 9 && j + 2 > 0 && j + 2 < 9 && position.board[i - 1][j + 2] * N <= 0)
	{
		Mutare move(N, j, i, j + 2, i - 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i - 2 > 0 && i - 2 < 9 && j - 1 > 0 && j - 1 < 9 && position.board[i - 2][j - 1] * N <= 0)
	{
		Mutare move(N, j, i, j - 1, i - 2);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i - 1 > 0 && i - 1 < 9 && j - 2 > 0 && j - 2 < 9 && position.board[i - 1][j - 2] * N <= 0)
	{
		Mutare move(N, j, i, j - 2, i - 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 1 > 0 && i + 1 < 9 && j - 2 > 0 && j - 2 < 9 && position.board[i + 1][j - 2] * N <= 0)
	{
		Mutare move(N, j, i, j - 2, i + 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 2 > 0 && i + 2 < 9 && j - 1 > 0 && j - 1 < 9 && position.board[i + 2][j - 1] * N <= 0)
	{
		Mutare move(N, j, i, j - 1, i + 2);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 1 > 0 && i + 1 < 9 && j + 2 > 0 && j + 2 < 9 && position.board[i + 1][j + 2] * N <= 0)
	{
		Mutare move(N, j, i, j + 2, i + 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 2 > 0 && i + 2 < 9 && j + 1 > 0 && j + 1 < 9 && position.board[i + 2][j + 1] * N <= 0)
	{
		Mutare move(N, j, i, j + 1, i + 2);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
}

void Chess::BishopMoves(Position& position, size_t j, size_t i)
{
	int B = position.side == white ? wB : bB;

	size_t d = 1;
	while (i + d > 0 && i + d < 9 && j + d > 0 && j + d < 9)
	{
		if (position.board[i + d][j + d] * B > 0)
			break;
		Mutare move(B, j, i, j + d, i + d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i + d][j + d] * B < 0)
			break;
		++d;
	}
	d = 1;
	while (i + d > 0 && i + d < 9 && j - d > 0 && j - d < 9)
	{
		if (position.board[i + d][j - d] * B > 0)
			break;
		Mutare move(B, j, i, j - d, i + d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i + d][j - d] * B < 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j + d > 0 && j + d < 9)
	{
		if (position.board[i - d][j + d] * B > 0)
			break;
		Mutare move(B, j, i, j + d, i - d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i - d][j + d] * B < 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j - d > 0 && j - d < 9)
	{
		if (position.board[i - d][j - d] * B > 0)
			break;
		Mutare move(B, j, i, j - d, i - d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i - d][j - d] * B < 0)
			break;
		++d;
	}
}

void Chess::RookMoves(Position& position, size_t j, size_t i)
{
	int R = position.side == white ? wR : bR;

	size_t d = 1;
	while (i + d > 0 && i + d < 9)
	{
		if (position.board[i + d][j] * R > 0)
			break;
		Mutare move(R, j, i, j, i + d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i + d][j] * R < 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9)
	{
		if (position.board[i - d][j] * R > 0)
			break;
		Mutare move(R, j, i, j, i - d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i - d][j] * R < 0)
			break;
		++d;
	}
	d = 1;
	while (j + d > 0 && j + d < 9)
	{
		if (position.board[i][j + d] * R > 0)
			break;
		Mutare move(R, j, i, j + d, i);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i][j + d] * R < 0)
			break;
		++d;
	}
	d = 1;
	while (j - d > 0 && j - d < 9)
	{
		if (position.board[i][j - d] * R > 0)
			break;
		Mutare move(R, j, i, j - d, i);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i][j - d] * R < 0)
			break;
		++d;
	}
}

void Chess::QueenMoves(Position& position, size_t j, size_t i)
{
	int Q = position.side == white ? wQ : bQ;

	size_t d = 1;
	while (i + d > 0 && i + d < 9 && j + d > 0 && j + d < 9)
	{
		if (position.board[i + d][j + d] * Q > 0)
			break;
		Mutare move(Q, j, i, j + d, i + d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i + d][j + d] * Q < 0)
			break;
		++d;
	}
	d = 1;
	while (i + d > 0 && i + d < 9 && j - d > 0 && j - d < 9)
	{
		if (position.board[i + d][j - d] * Q > 0)
			break;
		Mutare move(Q, j, i, j - d, i + d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i + d][j - d] * Q < 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j + d > 0 && j + d < 9)
	{
		if (position.board[i - d][j + d] * Q > 0)
			break;
		Mutare move(Q, j, i, j + d, i - d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i - d][j + d] * Q < 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j - d > 0 && j - d)
	{
		if (position.board[i - d][j - d] * Q > 0)
			break;
		Mutare move(Q, j, i, j - d, i - d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i - d][j - d] * Q < 0)
			break;
		++d;
	}

	d = 1;
	while (i + d > 0 && i + d < 9)
	{
		if (position.board[i + d][j] * Q > 0)
			break;
		Mutare move(Q, j, i, j, i + d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i + d][j] * Q < 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9)
	{
		if (position.board[i - d][j] * Q > 0)
			break;
		Mutare move(Q, j, i, j, i - d);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i - d][j] * Q < 0)
			break;
		++d;
	}
	d = 1;
	while (j + d > 0 && j + d < 9)
	{
		if (position.board[i][j + d] * Q > 0)
			break;
		Mutare move(Q, j, i, j + d, i);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i][j + d] * Q < 0)
			break;
		++d;
	}
	d = 1;
	while (j - d > 0 && j - d < 9)
	{
		if (position.board[i][j - d] * Q > 0)
			break;
		Mutare move(Q, j, i, j - d, i);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
		if (position.board[i][j - d] * Q < 0)
			break;
		++d;
	}
}

void Chess::PawnMoves(Position& position, size_t j, size_t i)
{
	int P = position.side == white ? wP : bP;

	// white pawn
	if (position.side == white)
	{
		if (j - 1 > 0 && j - 1 < 9 && position.board[i + 1][j - 1] < 0)
		{
			Mutare move(P, j, i, j - 1, i + 1);
			if (move.MakeMove(position).IsLegal())
			{
				if (i == 7)			// every type of promotion will be a move
				{
					move.promotion = wQ;
					moves.push_back(move);
					move.promotion = wR;
					moves.push_back(move);
					move.promotion = wB;
					moves.push_back(move);
					move.promotion = wN;
					moves.push_back(move);
				}
				else
					moves.push_back(move);
			}
		}
		if (j + 1 > 0 && j + 1 < 9 && position.board[i + 1][j + 1] < 0)
		{
			Mutare move(P, j, i, j + 1, i + 1);
			if (move.MakeMove(position).IsLegal())
			{
				if (i == 7)
				{
					move.promotion = wQ;
					moves.push_back(move);
					move.promotion = wR;
					moves.push_back(move);
					move.promotion = wB;
					moves.push_back(move);
					move.promotion = wN;
					moves.push_back(move);
				}
				else
					moves.push_back(move);
			}
		}
		if (position.board[i + 1][j] == 0)
		{
			Mutare move(P, j, i, j, i + 1);
			if (move.MakeMove(position).IsLegal())
			{
				if (i == 7)
				{
					move.promotion = wQ;
					moves.push_back(move);
					move.promotion = wR;
					moves.push_back(move);
					move.promotion = wB;
					moves.push_back(move);
					move.promotion = wN;
					moves.push_back(move);
				}
				else
					moves.push_back(move);
			}
		}
		if (i == 2 && position.board[i + 1][j] == 0 && position.board[i + 2][j] == 0)
		{
			Mutare move(P, j, i, j, i + 2);
			if (move.MakeMove(position).IsLegal())
				moves.push_back(move);
		}
		if (position.enPassant && position.enPassant == j - 1 && i == 5)
		{
			Mutare move(P, j, i, j - 1, i + 1);
			if (move.MakeMove(position).IsLegal())
				moves.push_back(move);
		}
		if (position.enPassant && position.enPassant == j + 1 && i == 5)
		{
			Mutare move(P, j, i, j + 1, i + 1);
			if (move.MakeMove(position).IsLegal())
				moves.push_back(move);
		}
	}

	// black pawn
	if (position.side == black)
	{
		if (j - 1 > 0 && j - 1 < 9 && position.board[i - 1][j - 1] > 0)
		{
			Mutare move(P, j, i, j - 1, i - 1);
			if (move.MakeMove(position).IsLegal())
			{
				if (i == 2)
				{
					move.promotion = bQ;
					moves.push_back(move);
					move.promotion = bR;
					moves.push_back(move);
					move.promotion = bB;
					moves.push_back(move);
					move.promotion = bN;
					moves.push_back(move);
				}
				else
					moves.push_back(move);
			}
		}
		if (j + 1 > 0 && j + 1 < 9 && position.board[i - 1][j + 1] > 0)
		{
			Mutare move(P, j, i, j + 1, i - 1);
			if (move.MakeMove(position).IsLegal())
			{
				if (i == 2)
				{
					move.promotion = bQ;
					moves.push_back(move);
					move.promotion = bR;
					moves.push_back(move);
					move.promotion = bB;
					moves.push_back(move);
					move.promotion = bN;
					moves.push_back(move);
				}
				else
					moves.push_back(move);
			}
		}
		if (position.board[i - 1][j] == 0)
		{
			Mutare move(P, j, i, j, i - 1);
			if (move.MakeMove(position).IsLegal())
			{
				if (i == 2)
				{
					move.promotion = bQ;
					moves.push_back(move);
					move.promotion = bR;
					moves.push_back(move);
					move.promotion = bB;
					moves.push_back(move);
					move.promotion = bN;
					moves.push_back(move);
				}
				else
					moves.push_back(move);
			}
		}
		if (i == 7 && position.board[i - 1][j] == 0 && position.board[i - 2][j] == 0)
		{
			Mutare move(P, j, i, j, i - 2);
			if (move.MakeMove(position).IsLegal())
				moves.push_back(move);
		}
		if (position.enPassant && position.enPassant == j - 1 && i == 4)
		{
			Mutare move(P, j, i, j - 1, i - 1);
			if (move.MakeMove(position).IsLegal())
				moves.push_back(move);
		}
		if (position.enPassant && position.enPassant == j + 1 && i == 4)
		{
			Mutare move(P, j, i, j + 1, i - 1);
			if (move.MakeMove(position).IsLegal())
				moves.push_back(move);
		}
	}
}

void Chess::KingMoves(Position& position, size_t j, size_t i)
{
	int K = position.side == white ? wK : bK;

	if (i - 1 > 0 && i - 1 < 9 && j - 1 > 0 && j - 1 < 9 && position.board[i - 1][j - 1] * K <= 0)
	{
		Mutare move(K, j, i, j - 1, i - 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i - 1 > 0 && i - 1 < 9 && position.board[i - 1][j] * K <= 0)
	{
		Mutare move(K, j, i, j, i - 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i - 1 > 0 && i - 1 < 9 && j + 1 > 0 && j + 1 < 9 && position.board[i - 1][j + 1] * K <= 0)
	{
		Mutare move(K, j, i, j + 1, i - 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (j - 1 > 0 && j - 1 < 9 && position.board[i][j - 1] * K <= 0)
	{
		Mutare move(K, j, i, j - 1, i);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (j + 1 > 0 && j + 1 < 9 && position.board[i][j + 1] * K <= 0)
	{
		Mutare move(K, j, i, j + 1, i);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 1 > 0 && i + 1 < 9 && j - 1 > 0 && j - 1 < 9 && position.board[i + 1][j - 1] * K <= 0)
	{
		Mutare move(K, j, i, j - 1, i + 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 1 > 0 && i + 1 < 9 && position.board[i + 1][j] * K <= 0)
	{
		Mutare move(K, j, i, j, i + 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}
	if (i + 1 > 0 && i + 1 < 9 && j + 1 > 0 && j + 1 < 9 && position.board[i + 1][j + 1] * K <= 0)
	{
		Mutare move(K, j, i, j + 1, i + 1);
		if (move.MakeMove(position).IsLegal())
			moves.push_back(move);
	}

	// castles
	if (position.side == white && j == e && i == 1 && !position.inCheck &&
		(position.wCastle == kingside || position.wCastle == both))
	{
		if (position.board[1][f] == 0 && position.board[1][g] == 0)
		{
			Mutare tmove(wK, e, 1, f, 1);
			if (tmove.MakeMove(position).IsLegal())
			{
				Mutare move(wK, e, 1, g, 1);
				if (move.MakeMove(position).IsLegal())
					moves.push_back(move);
			}
		}
	}
	if (position.side == white && j == e && i == 1 && !position.inCheck && (position.wCastle == queenside || position.wCastle == both))
	{
		if (position.board[1][d] == 0 && position.board[1][c] == 0 && position.board[1][b] == 0)
		{
			Mutare tmove(wK, e, 1, d, 1);
			if (tmove.MakeMove(position).IsLegal())
			{
				Mutare move(wK, e, 1, c, 1);
				if (move.MakeMove(position).IsLegal())
					moves.push_back(move);
			}
		}
	}
	if (position.side == black && j == e && i == 8 && !position.inCheck && (position.bCastle == kingside || position.bCastle == both))
	{
		if (position.board[8][f] == 0 && position.board[8][g] == 0)
		{
			Mutare tmove(bK, e, 8, f, 8);
			if (tmove.MakeMove(position).IsLegal())
			{
				Mutare move(bK, e, 8, g, 8);
				if (move.MakeMove(position).IsLegal())
					moves.push_back(move);
			}
		}
	}
	if (position.side == black && j == e && i == 8 && !position.inCheck && (position.bCastle == queenside || position.bCastle == both))
	{
		if (position.board[8][d] == 0 && position.board[8][c] == 0 && position.board[8][b] == 0)
		{
			Mutare tmove(bK, e, 8, d, 8);
			if (tmove.MakeMove(position).IsLegal())
			{
				Mutare move(bK, e, 8, c, 8);
				if (move.MakeMove(position).IsLegal())
					moves.push_back(move);
			}
		}
	}
}

//////////////////////////////////////////////////

void Chess::Evaluate(Position& position)
{
	if (position.inCheck)
	{
		GenerateMoves(position);
		if (moves.empty())			// checkmate, no need for evaluating
		{
			position.evaluation = position.side == white ? minINF : INF;
			return;
		}
	}

	mobility = 0;
	strategy = 0;
	figDensity = position.figureCount + position.wQueens + position.bQueens;
	wPawns = position.wPawnsOnLight + position.wPawnsOnDark;
	bPawns = position.bPawnsOnLight + position.bPawnsOnDark;

	for (size_t i = 1; i < 9; ++i)
	{
		for (size_t j = 1; j < 9; ++j)
		{
			int p = position.board[i][j];
			switch (p)
			{
			case wN:
			case bN:
				KnightMobility(position, j, i);
				KnightStrategy(position, j, i);
				break;
			case wB:
			case bB:
				BishopMobility(position, j, i);
				BishopStrategy(position, j, i);
				break;
			case wR:
			case bR:
				RookMobility(position, j, i);
				RookStrategy(position, j, i);
				break;
			case wQ:
			case bQ:
				QueenMobility(position, j, i);
				QueenStrategy(position, j, i);
				break;
			case wP:
			case bP:
				PawnMobility(position, j, i);
				PawnStrategy(position, j, i);
				break;
			case wK:
			case bK:
				KingMobility(position, j, i);
				KingStrategy(position, j, i);
				break;
			default:
				break;
	 		}
		}
	}

	int pawnFinalBonus = (wPawns - bPawns) * (16 - figDensity) * 100 / 64;
	material = position.material + pawnFinalBonus;

	material *= 2;
	mobility += mobility * figDensity / 16;		// first more mobility
	strategy += strategy * (16 - figDensity) / 16;	// then more strategy

	// more explanation of evaluation adjustments:
	// density of figures on the board changes the assesment of the position during the game
	// density of figures on the board changes the value of pawn material during the game
	// number of pawns a side has changes the value of its figure material during the game

	position.evaluation = material + mobility + strategy;
}

bool Chess::SameColour(int firstPiece, int secondPiece) const			// compares two pieces' colours
{
	if (firstPiece > 0 && secondPiece > 0)
		return true;
	if (firstPiece < 0 && secondPiece < 0)
		return true;
	return false;
}

bool Chess::CaptureIsGood(int Piece_1, int Piece_2) const
{
	if (Piece_2 == 0)
		return false;
	if (Piece_1 > 0 && Piece_2 < 0 && Piece_1 + Piece_2 < 0)
		return true;
	if (Piece_1 < 0 && Piece_2 > 0 && Piece_1 + Piece_2 > 0)
		return true;
	return false;
}

void Chess::KnightMobility(const Position& position, size_t j, size_t i)
{
	int mob = 0;
	int threat = 0;
	int N = position.board[i][j];

	if (i - 2 > 0 && i - 2 < 9 && j + 1 > 0 && j + 1 < 9)
	{
		int p = position.board[i - 2][j + 1];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}
	if (i - 1 > 0 && i - 1 < 9 && j + 2 > 0 && j + 2 < 9)
	{
		int p = position.board[i - 1][j + 2];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if(SameColour(N, p) == false)
			++mob;
	}
	if (i - 2 > 0 && i - 2 < 9 && j - 1 > 0 && j - 1 < 9)
	{
		int p = position.board[i - 2][j - 1];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}
	if (i - 1 > 0 && i - 1 < 9 && j - 2 > 0 && j - 2 < 9)
	{
		int p = position.board[i - 1][j - 2];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}
	if (i + 1 > 0 && i + 1 < 9 && j - 2 > 0 && j - 2 < 9)
	{
		int p = position.board[i + 1][j - 2];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}
	if (i + 2 > 0 && i + 2 < 9 && j - 1 > 0 && j - 1 < 9)
	{
		int p = position.board[i + 2][j - 1];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}
	if (i + 1 > 0 && i + 1 < 9 && j + 2 > 0 && j + 2 < 9)
	{
		int p = position.board[i + 1][j + 2];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}
	if (i + 2 > 0 && i + 2 < 9 && j + 1 > 0 && j + 1 < 9)
	{
		int p = position.board[i + 2][j + 1];
		if (CaptureIsGood(N, p) == true)
			++threat;
		else if (SameColour(N, p) == false)
			++mob;
	}

	if (N == wN)
		mobility += 2 * mob + 10 * threat;
	else if (N == bN)
		mobility -= 2 * mob + 10 * threat;
}

void Chess::BishopMobility(const Position& position, size_t j, size_t i)
{
	int mob = 0;
	int threat = 0;
	int B = position.board[i][j];

	size_t d = 1;
	while (i + d > 0 && i + d < 9 && j + d > 0 && j + d < 9)
	{
		int p = position.board[i + d][j + d];
		if (CaptureIsGood(B, p) == true)
			++threat;
		else if (SameColour(B, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i + d > 0 && i + d < 9 && j - d > 0 && j - d < 9)
	{
		int p = position.board[i + d][j - d];
		if (CaptureIsGood(B, p) == true)
			++threat;
		else if (SameColour(B, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j + d > 0 && j + d < 9)
	{
		int p = position.board[i - d][j + d];
		if (CaptureIsGood(B, p) == true)
			++threat;
		else if (SameColour(B, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j - d > 0 && j - d < 9)
	{
		int p = position.board[i - d][j - d];
		if (CaptureIsGood(B, p) == true)
			++threat;
		else if (SameColour(B, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}

	if (B == wB)
		mobility += mob + 10 * threat;
	else if (B == bB)
		mobility -= mob + 10 * threat;
}

void Chess::RookMobility(const Position& position, size_t j, size_t i)
{
	if (figDensity > 15)
		return;

	int mob = 0;
	int threat = 0;
	int R = position.board[i][j];

	size_t d = 1;
	while (i + d > 0 && i + d < 9)
	{
		int p = position.board[i + d][j];
		if (CaptureIsGood(R, p) == true)
			++threat;
		else if (SameColour(R, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9)
	{
		int p = position.board[i - d][j];
		if (CaptureIsGood(R, p) == true)
			++threat;
		else if (SameColour(R, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (j + d > 0 && j + d < 9)
	{
		int p = position.board[i][j + d];
		if (CaptureIsGood(R, p) == true)
			++threat;
		else if (SameColour(R, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (j - d > 0 && j - d < 9)
	{
		int p = position.board[i][j - d];
		if (CaptureIsGood(R, p) == true)
			++threat;
		else if (SameColour(R, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}

	mob -= mob * figDensity / 16;
	if (R == wR)
		mobility += mob + 10 * threat;
	else if (R == bR)
		mobility -= mob + 10 * threat;
}

void Chess::QueenMobility(const Position& position, size_t j, size_t i)
{
	if (figDensity > 15)
		return;

	int mob = 0;
	int Q = position.board[i][j];

	size_t d = 1;					// bishopy:
	while (i + d > 0 && i + d < 9 && j + d > 0 && j + d < 9)
	{
		int p = position.board[i + d][j + d];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i + d > 0 && i + d < 9 && j - d > 0 && j - d < 9)
	{
		int p = position.board[i + d][j - d];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j + d > 0 && j + d < 9)
	{
		int p = position.board[i - d][j + d];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9 && j - d > 0 && j - d)
	{
		int p = position.board[i - d][j - d];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}

	d = 1;							// rooky:
	while (i + d > 0 && i + d < 9)
	{
		int p = position.board[i + d][j];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (i - d > 0 && i - d < 9)
	{
		int p = position.board[i - d][j];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (j + d > 0 && j + d < 9)
	{
		int p = position.board[i][j + d];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}
	d = 1;
	while (j - d > 0 && j - d < 9)
	{
		int p = position.board[i][j - d];
		if (SameColour(Q, p) == false)
			mob++;
		if (p != 0)
			break;
		++d;
	}

	mob -= mob * figDensity / 16;
	if (Q == wQ)
		mobility += mob / 8;
	else if (Q == bQ)
		mobility -= mob / 8;
}

void Chess::PawnMobility(const Position& position, size_t j, size_t i)
{
	int mob = 0;
	int threat = 0;
	int P = position.board[i][j];

	// white pawn
	if (P == wP)
	{
		if (j - 1 > 0 && j - 1 < 9 && position.board[i + 1][j - 1] < 0)
		{
			if (position.board[i + 1][j - 1] == bP)
				++mob;
			else
				++threat;
		}	
		if (j + 1 > 0 && j + 1 < 9 && position.board[i + 1][j + 1] < 0)
		{
			if (position.board[i + 1][j + 1] == bP)
				++mob;
			else
				++threat;
		}
		if (position.board[i + 1][j] == 0)
		{
			++mob;
			if (i == 2 && position.board[i + 2][j] == 0)
				++mob;
		}
		if (position.enPassant && position.enPassant == j - 1 && i == 5)
			++mob;
		if (position.enPassant && position.enPassant == j + 1 && i == 5)
			++mob;
	}

	// black pawn
	else if (P == bP)
	{
		if (j - 1 > 0 && j - 1 < 9 && position.board[i - 1][j - 1] > 0)
		{
			if (position.board[i - 1][j - 1] == wP)
				++mob;
			else
				++threat;
		}
		if (j + 1 > 0 && j + 1 < 9 && position.board[i - 1][j + 1] > 0)
		{
			if (position.board[i - 1][j + 1] == wP)
				++mob;
			else
				++threat;
		}
		if (position.board[i - 1][j] == 0)
		{
			++mob;
			if (i == 7 && position.board[i - 2][j] == 0)
				++mob;
		}
		if (position.enPassant && position.enPassant == j - 1 && i == 4)
			++mob;
		if (position.enPassant && position.enPassant == j + 1 && i == 4)
			++mob;
	}

	if (P == wP)
		mobility += mob + 10 * threat;
	else if (P == bP)
		mobility -= mob + 10 * threat;
}

void Chess::KingMobility(const Position& position, size_t j, size_t i)
{
	if (figDensity > 15)
		return;

	int mob = 0;
	int K = position.board[i][j];
	if (K == wK && position.bQueens > 0)
		return;
	if (K == bK && position.wQueens > 0)
		return;

	if (i - 1 > 0 && i - 1 < 9 && j - 1 > 0 && j - 1 < 9)
	{
		int p = position.board[i - 1][j - 1];
		if (p == 0)
			++mob;
	}
	if (i - 1 > 0 && i - 1 < 9)
	{
		int p = position.board[i - 1][j];
		if (p == 0)
			++mob;
	}
	if (i - 1 > 0 && i - 1 < 9 && j + 1 > 0 && j + 1 < 9)
	{
		int p = position.board[i - 1][j + 1];
		if (p == 0)
			++mob;
	}
	if (j - 1 > 0 && j - 1 < 9)
	{
		int p = position.board[i][j - 1];
		if (p == 0)
	 		++mob;
	}
	if (j + 1 > 0 && j + 1 < 9)
	{
		int p = position.board[i][j + 1];
		if (p == 0)
			++mob;
	}
	if (i + 1 > 0 && i + 1 < 9 && j - 1 > 0 && j - 1 < 9)
	{
		int p = position.board[i + 1][j - 1];
		if (p == 0)
			++mob;
	}
	if (i + 1 > 0 && i + 1 < 9)
	{
		int p = position.board[i + 1][j];
		if (p == 0)
			++mob;
	}
	if (i + 1 > 0 && i + 1 < 9 && j + 1 > 0 && j + 1 < 9)
	{
		int p = position.board[i + 1][j + 1];
		if (p == 0)
			++mob;
	}
		
	mob -= mob * figDensity / 16;
	if (K == wK)
		mobility += mob;
	else if (K == bK)
		mobility -= mob;
}

void Chess::PawnStrategy(const Position& position, size_t j, size_t i)
{
	int P = position.board[i][j];
	int strat = 0;
	int stratOpen = 0;

	if (P == wP)
	{
		// central pawns
		if (i == 4)
		{
			if (j == d || j == e)
				stratOpen += 32;
			else if (j == c)
				stratOpen += 16;
		}
		else if (i == 5)
		{
			if (j == d || j == e)
				stratOpen += 32;
		}

		if (position.board[i + 1][j] != 0)		// blocked pawn
			strat -= 8;

		if (position.board[0][j] > 1)			// doubled pawns
			strat -= 16;

		if (position.board[i][j - 1] == wP || position.board[i][j + 1] == wP)	// adjacent pawns
			strat += 4;
	}
	else if (P == bP)
	{
		// central pawns
		if (i == 5)
		{
			if (j == d || j == e)
				stratOpen += 32;
			else if (j == c)
				stratOpen += 16;
		}
		else if (i == 4)
		{
			if (j == d || j == e)
				stratOpen += 32;
		}

		if (position.board[i - 1][j] != 0)		// blocked pawn
			strat -= 8;

		if (position.board[9][j] > 1)			// doubled pawns
			strat -= 16;

		if (position.board[i][j - 1] == bP || position.board[i][j + 1] == bP)	// adjacent pawns
			strat += 4;
	}

	if (PassedPawn(position, j, i))
	{
		int stratPassed = 0;
		if (P == wP)
		{
			if (i == 7)
				stratPassed += 64;
			else if (i == 6)
				stratPassed += 32;
			else if (i == 5)
				stratPassed += 16;
			else if (i == 4)
				stratPassed += 8;
			else
				stratPassed += 4;
		}
		else if (P == bP)
		{
			if (i == 2)
				stratPassed += 64;
			else if (i == 3)
				stratPassed += 32;
			else if (i == 4)
				stratPassed += 16;
			else if (i == 5)
				stratPassed += 8;
			else
				stratPassed += 4;
		}
		strat += stratPassed * (16 - figDensity) / 8;
	}
	else
	{
		if (P == wP)
		{
			if (i == 6)
				strat += 32;
		}
		else if (P == bP)
		{
			if (i == 3)
				strat += 32;
		}

		if (IsolatedPawn(position, j, i))
		{
			strat -= 10;
			if (P == wP && position.board[9][j] == 0)
				stratOpen -= 8;
			else if (P == bP && position.board[0][j] == 0)
				stratOpen -= 8;
		}
	}

	strat += stratOpen * figDensity / 16;
	if (P == wP)
		strategy += strat;
	else if (P == bP)
		strategy -= strat;
}

void Chess::KnightStrategy(const Position& position, size_t j, size_t i)
{
	int strat = 0;
	int N = position.board[i][j];

	if (i > 2 && i < 7 && j > b && j < g)				// central knight
		strat += 16;
	else if (i == 1 || i == 8 || j == a || j == h)		// marginal knight
		strat -= 16;

	if (N == wN)
		strategy += strat;
	else if (N == bN)
		strategy -= strat;
}

void Chess::BishopStrategy(const Position& position, size_t j, size_t i)
{
	int strat = 0;
	int B = position.board[i][j];

	if (i == j || i + j == 9)								// long diagonal bishop
		strat += 2 * figDensity;

	if (B == wB && position.wBishops > 1)					// pair of bishops
		strat += 32 - figDensity;
	else if (B == bB && position.bBishops > 1)				// pair of bishops
		strat += 32 - figDensity;

	if (B == wB)
	{
		if ((i + j) % 2)									// limited by its own pawns
			strat += 16 - position.wPawnsOnLight * 4;
		else
			strat += 16 - position.wPawnsOnDark * 4;
	}
	else if (B == bB)
	{
		if ((i + j) % 2)									// limited by its own pawns
			strat += 16 - position.bPawnsOnLight * 4;
		else
			strat += 16 - position.bPawnsOnDark * 4;
	}

	if (B == wB)
		strategy += strat;
	else if (B == bB)
		strategy -= strat;
}

void Chess::RookStrategy(const Position& position, size_t j, size_t i)
{
	int strat = 0;
	int R = position.board[i][j];

	if (position.board[0][j] == 0 && position.board[9][j] == 0)			// open column
	{
		int pawns = wPawns + bPawns;
		if (pawns > 8)
			strat += (pawns - 8) * 4;
	}

	if (R == wR && i == 7)					// 7th row 
	{
		strat += 32 - figDensity;
	}
	else if (R == bR && i == 2)				// 2nd row
	{
		strat += 32 - figDensity;
	}

	if (R == wR)
		strategy += strat;
	else if (R == bR)
		strategy -= strat;
}

void Chess::QueenStrategy(const Position& position, size_t j, size_t i)
{
	if (figDensity > 15)
		return;

	int strat = 0;
	int Q = position.board[i][j];

	if (i > 3 && i < 6 && j > c && j < f)				// center queen in the endgame
		strat += 16 - figDensity;

	if (Q == wQ)
		strategy += strat;
	else if (Q == bQ)
		strategy -= strat;
}

void Chess::KingStrategy(const Position& position, size_t j, size_t i)
{
	int strat = 0;
	int K = position.board[i][j];

	// if not endgame, castle matters:
	if (K == wK)
	{
		if (position.wCastle == none)
			strat -= 3 * figDensity;
		else if (position.wCastle == done)
			strat += 3 * figDensity;
	}
	else if (K == bK)
	{
		if (position.bCastle == none)
			strat -= 3 * figDensity;
		else if (position.bCastle == done)
			strat += 3 * figDensity;
	}
	// endgame, centralized King:
	if ((K == wK && position.bQueens == 0) || (K == bK && position.wQueens == 0))
	{
		if (i > 3 && i < 6 && j > c && j < f)
			strat += (16 - figDensity) * 2;
		else if (i > 1 && i < 8 && j > a && j < h)
			strat += 16 - figDensity;
	}
	// mating lonely king
	if (figDensity < 4) // && wPawns == 0 && bPawns == 0)
	{
		if ((position.material > wB && K == wK) || (position.material < bB && K == bK))
		{
			// strong king to get closer to weak king
			if (wPawns == 0 && bPawns == 0)
			{
				int colDif = abs((int)position.wKingCol - (int)position.bKingCol);
				int rowDif = abs((int)position.wKingRow - (int)position.bKingRow);
				strat += 784 - 8 * (colDif * colDif + rowDif * rowDif);
			}
			else
			{
				int colDif = abs((int)position.wKingCol - (int)position.bKingCol);
				int rowDif = abs((int)position.wKingRow - (int)position.bKingRow);
				strat += 196 - 2 * (colDif * colDif + rowDif * rowDif);
			}
		}
	}

	if (K == wK)				// white king attacked
	{
		for (size_t di = 0; di < 3; ++di)
			for (size_t dj = 0; dj < 3; ++dj)
			{
				if (i + di - 1 > 0 && i + di - 1 < 9)
				{
					if (position.BlackAttack(j + dj - 1, i + di - 1))
						strat -= 10;
				}
			}
	}
	else if (K == bK)			// black king attacked
	{
		for (size_t di = 0; di < 3; ++di)
			for (size_t dj = 0; dj < 3; ++dj)
			{
				if (i + di - 1 > 0 && i + di - 1 < 9)
				{
					if (position.WhiteAttack(j + dj - 1, i + di - 1))
						strat -= 10;
				}
			}
	}

	if (K == wK)
		strategy += strat;
	else if (K == bK)
		strategy -= strat;
}

bool Chess::PassedPawn(const Position& position, size_t j, size_t i) const
{
	int P = position.board[i][j];

	if (P == wP)
	{
		if (position.board[9][j] == 0 && position.board[9][j - 1] == 0 && position.board[9][j + 1] == 0)
			return true;
		for (size_t ii = i + 1; ii < 8; ++ii)
			if (position.board[ii][j] == bP || position.board[ii][j - 1] == bP
				|| position.board[ii][j + 1] == bP)
				return false;
	}
	else if (P == bP)
	{
		if (position.board[0][j] == 0 && position.board[0][j - 1] == 0 && position.board[0][j + 1] == 0)
			return true;
		for (size_t ii = i - 1; ii > 1; --ii)
			if (position.board[ii][j] == wP || position.board[ii][j - 1] == wP
				|| position.board[ii][j + 1] == wP)
				return false;
	}
	return true;
}

bool Chess::IsolatedPawn(const Position& position, size_t j, size_t i) const
{
	int P = position.board[i][j];

	if (P == wP)
	{
		if (position.board[0][j - 1] == 0 && position.board[0][j + 1] == 0)
			return true;
	}
	else if (P == bP)
	{
		if (position.board[9][j - 1] == 0 && position.board[9][j + 1] == 0)
			return true;
	}
	return false;
}

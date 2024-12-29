#pragma once
#include <wx/wx.h>
#include <wx/sound.h>
#include <wx/aboutdlg.h>

#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

constexpr int INF		=  1000000;
constexpr int minINF	= -1000000;
constexpr int wP		=  100;
constexpr int bP		= -100;
constexpr int wN		=  300;
constexpr int bN		= -300;
constexpr int wB		=  325;
constexpr int bB		= -325;
constexpr int wR		=  500;
constexpr int bR		= -500;
constexpr int wQ		=  900;
constexpr int bQ		= -900;
constexpr int wK		=  100000;
constexpr int bK		= -100000;
constexpr int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8;	// column letter
constexpr int initBOARD[10][10]{ 0,  1,  1,  1,  1,  1,  1,  1,  1, 0,
								 0, wR, wN, wB, wQ, wK, wB, wN, wR, 0,
								 0, wP, wP, wP, wP, wP, wP, wP, wP, 0,
								 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
								 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
								 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
								 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
								 0, bP, bP, bP, bP, bP, bP, bP, bP, 0,
								 0, bR, bN, bB, bQ, bK, bB, bN, bR, 0,
								 0,  1,  1,  1,  1,  1,  1,  1,  1, 0 };
constexpr int DEPTH = 6;	// depth parameter for SearchMove()

enum SideToMove		// side currently to move
{
	white,
	black
};
enum Player			// who playes the game
{
	computer,
	human
};
enum Result			// result of the game
{
	win,
	lose,
	draw,
	undecided
};
enum State			// state of the game
{
	playing,
	finished
};
enum Castle			// castle situation
{
	done,           // castle done
	none,           // castle not available
	kingside,       // castle available only on kingside
	queenside,      // castle available only on queenside
	both            // castle available on both kingside and queenside
};

enum Speed			// speed of the game (how much spert is computing)
{
	rapid,			// very fast
	normal,			// not so fast
	slow			// really slow
};

class SpertApp : public wxApp
{
public:
	bool OnInit() override;
	int OnExit() override;	
};

class Position
{
public:
	int board[10][10];
	SideToMove side;	// white/black
	Castle wCastle;     // done/none/kingside/queenside/both
	Castle bCastle;     // done/none/kingside/queenside/both

	size_t enPassant;	// coloana pe care se poate captura pionul; 0 = nu se poate
	bool inCheck;		// is set by constructor and Move::MakeMove
	size_t wKingCol;	// updated by Mutare::MakeMove
	size_t wKingRow;
	size_t bKingCol;
	size_t bKingRow;
	int evaluation;		// evaluation of the position (material + mobility + strategy)
	int material;
	int figureCount;		// number of figures = knights + bishops + rooks + queens; updated by Mutare::MakeMove
	int fiftyMoveChecker;	// number of non-pawn/non-capture moves
	int wBishops;			// number of white bishops on board
	int bBishops;			// number of black bishops on board
	int wQueens;
	int bQueens;
	int wPawnsOnLight;		// number of white pawns on light squares
	int wPawnsOnDark;		// number of white pawns on dark squares
	int bPawnsOnLight;		// number of black pawns on light squares
	int bPawnsOnDark;		// number of black pawns on dark squares
	int halfMoves;			// number of half-moves played so far

	Position();
	Position(const Position& pos);
	~Position() = default;

	Position& operator=(const Position& pos) = default;
	bool operator==(const Position& pos) const;
	bool operator!=(const Position& pos) const;
	
	void SwitchSide();							// used by chess methods, before not after checking IsLegal()
	bool IsLegal() const;                       // used by Chess methods, to not let your own king in check
	bool WhiteAttack(size_t y, size_t x) const; // check if square is being attacked by W
	bool BlackAttack(size_t y, size_t x) const; // check if square is being attacked by B
};

class Mutare
{
public:
	int piece;					// 0 means no piece (empty square)
	size_t origCol;
	size_t origRow;
	size_t destCol;
	size_t destRow;
	int promotion;				// 0 means no promotion, non-0 means piece to promote to

	// column is put first, for the human sake, but in board[][], raw is put first
	Mutare(int pieceA, size_t origColA, size_t origRowA, size_t destColA, size_t destRowA);
	Mutare() = default;
	~Mutare() = default;

	bool operator==(const Mutare& mut) const;
	bool operator!=(const Mutare& mut) const;
	
	Position MakeMove(Position& position) const;
};

class OpeningBook			// the book of theoretical opening moves loaded by SpertApp::OnInit()::LoadBook()
{
public:
	Mutare node;							// the move in the node of opening book
	std::vector<OpeningBook> children;		// variations in the opening book
};

class Chess
{
public:
	std::list<Mutare> moves;		// the list of all generated moves in the position
	int material	{ 0 };
	int mobility	{ 0 };
	int strategy	{ 0 };
	int figDensity	{ 0 };
	int wPawns		{ 0 };
	int bPawns		{ 0 };

	void GenerateMoves(Position& position);						// generates all moves in the position
	void KnightMoves(Position& position, size_t j, size_t i);	// part of GenerateMoves()
	void BishopMoves(Position& position, size_t j, size_t i);	// ...
	void RookMoves(Position& position, size_t j, size_t i);		// ...
	void QueenMoves(Position& position, size_t j, size_t i);	// ...
	void PawnMoves(Position& position, size_t j, size_t i);		// ...
	void KingMoves(Position& position, size_t j, size_t i);		// ...

	bool SameColour(int Piece_1, int Piece_2) const;			// same colour (empty square returns false)
	bool CaptureIsGood(int Piece_1, int Piece_2) const;	// Piece_1 is opposite colour and smaller than Piece_2
	void Evaluate(Position& position);		// evaluate the position and store the value in position.evaluation

	// calculate mobility of specific pieces
	void PawnMobility(const Position& position, size_t j, size_t i);
	void KnightMobility(const Position& position, size_t j, size_t i);
	void BishopMobility(const Position& position, size_t j, size_t i);
	void RookMobility(const Position& position, size_t j, size_t i);
	void QueenMobility(const Position& position, size_t j, size_t i);
	void KingMobility(const Position& position, size_t j, size_t i);

	// calculate strategic value of specific pieces
	void PawnStrategy(const Position& position, size_t j, size_t i);
	void KnightStrategy(const Position& position, size_t j, size_t i);
	void BishopStrategy(const Position& position, size_t j, size_t i);
	void RookStrategy(const Position& position, size_t j, size_t i);
	void QueenStrategy(const Position& position, size_t j, size_t i);
	void KingStrategy(const Position& position, size_t j, size_t i);

	bool PassedPawn(const Position& position, size_t j, size_t i) const;
	bool IsolatedPawn(const Position& position, size_t j, size_t i) const;
};

class Search
{
public:
	struct Node
	{
		Position position;
		Mutare best;
		int score;

		Node() = default;
		~Node() = default;
		Node(Position& pos) : position(pos), best(Mutare()), score(position.side == white ? minINF : INF) {}
	} root;

	Search(Position& position);
	Search() = delete;
	~Search() = default;

	void SearchMove(Node& node, size_t depth, int alfa, int beta);
};

class Game
{
public:
	Position position;
	Chess gameChess;
	Player wPlayer;		// computer/human
	Player bPlayer;		// computer/human
	Result result;		// win/lose/draw/undecided
	State state;		// playing/finished
	Speed speed;		// rapid/normal/slow
	size_t depth;		// number of plies to calculate
	bool isComputing;	// Spert is currently computing
	bool inBook;		// current game is still in the opening book moves
	bool drawOffered;	// a draw has just been offered by human

	Game();
	~Game() = default;

	Mutare ComputerPlays();
	bool IsValidMove(const Mutare& mutare) const;	// move exists in game_chess.moves
};

namespace playground
{
	extern Position lastPositions[24];
	extern Position repeatedPositions[24];
	extern Game joc;
	extern OpeningBook* bookRoot;
	extern OpeningBook* bookPtr;
	extern std::vector<Position> moveList;
	extern size_t moveIndex;

	extern void SetPlayground();
	extern void LoadBook();
	extern void CleanPlayground();
}
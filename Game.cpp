#include "SpertApp.h"

#include <vector>

Game::Game() : wPlayer(human), bPlayer(computer), result(undecided), state(playing),
			depth(DEPTH), speed(normal), isComputing(false), inBook(true), drawOffered(false)
{
	position = Position();
	gameChess = Chess();
	playground::moveList.clear();
	playground::moveList.push_back(position);
	playground::moveIndex = 0;
	wxSound gameSound("sound_game.wav");
	if (gameSound.IsOk())
	{
		gameSound.Play();
	}
	for (Position& p : playground::lastPositions)
	{
		p = Position();
	}
	for (Position& p : playground::repeatedPositions)
	{
		p = Position();
	}
}

Mutare Game::ComputerPlays()
{
	if (inBook == true)
	{
		int noOfVars = playground::bookPtr->children.size();
		if (noOfVars > 0)
		{
			srand((unsigned)time(NULL));
			int random = rand();
			random = random % noOfVars;
			return playground::bookPtr->children[random].node;		// randomly picking book move
		}
		else
		{
			inBook = false;
		}
	}

	isComputing = true;					// signals that Spert is busy computing
	bool mateFound = false;
	size_t deep = 0;

	Search search(position);

	while (mateFound == false && deep < depth)		// search for a move untill finds a checkmate
	{
		deep++;										// starting from deep 1 and increasing to deep Game::depth

		search.SearchMove(search.root, deep, minINF, INF);
		search.root.position.evaluation = search.root.score;
		if ((search.root.position.side == white && search.root.score == INF) ||
			(search.root.position.side == black && search.root.score == minINF))
		{
			mateFound = true;
		}
	}

	isComputing = false;				// signals that Spert finished computing
	
	return search.root.best;
}

bool Game::IsValidMove(const Mutare& mutare) const
{
	for (const Mutare& m : gameChess.moves)
	{
		if (m == mutare)
			return true;
	}
	return false;
}
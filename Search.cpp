#include "SpertApp.h"

Search::Search(Position& position) : root(position) {}

void Search::SearchMove(Node& node, size_t depth, int alfa, int beta)
{
	if (depth == 4)
	{
		wxYield();				// makes the GUI responsive during move computation
	}
	Chess c;

	if (depth == 0)				// only deepest move is evaluated
	{
		c.Evaluate(node.position);
		node.score = node.position.evaluation;
		if (node.position.side == white && node.score > alfa)
			alfa = node.score;
		else if (node.position.side == black && node.score < beta)
			beta = node.score;
	}
	else
	{
		node.score = node.position.side == white ? minINF : INF;
		int drawValue = node.position.side == white && playground::joc.wPlayer == computer ? -100 
			: node.position.side == black && playground::joc.bPlayer == computer ? 100
			: 0;		// draw is evalauated as not so good for computer (playing for a win!)
		
		c.GenerateMoves(node.position);

		if (c.moves.empty())			// no possible moves = checkmate or stalemate
		{
			if (node.position.inCheck == false)		// stalemate (or else mate)
				node.score = drawValue;
			
			if (node.position.side == white && node.score > alfa)
				alfa = node.score;
			else if (node.position.side == black && node.score < beta)
				beta = node.score;
		}
		else
		{
			for (auto it = c.moves.begin(); it != c.moves.end(); ++it)
			{
				Position p(it->MakeMove(node.position));
				Node child(p);

				bool drawable = false;
				if (depth > 2)
				{
					for (int i = 0; i < 24; ++i)
					{
						if (child.position == playground::lastPositions[i])
						{
							drawable = true;
							child.score = drawValue;	// position is repeated
							i = 24;
						}
					}
				}
				if (drawable == false)
				{
					if (node.position.fiftyMoveChecker > 99)
					{
						drawable = true;
						child.score = drawValue;		// fifty-move rule
					}
				}
				if (drawable == false)
				{
					SearchMove(child, depth - 1, alfa, beta);
				}

				if (node.position.side == white)
				{
					if (child.score >= node.score)
					{
						node.score = child.score;
						node.best = *it;
					}
					if (node.score > beta)		// beta pruning
					{
						break;
					}
					if (node.score > alfa)
					{
						alfa = node.score;
					}
				}
				else if (node.position.side == black)
				{
					if (child.score <= node.score)
					{
						node.score = child.score;
						node.best = *it;
					}
					if (node.score < alfa)		// alpha pruning
					{
						break;
					}
					if (node.score < beta)
					{
						beta = node.score;
					}
				}
			}
		}
	}
}
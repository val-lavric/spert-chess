#include "SpertApp.h"
#include "SpertFrame.h"

#include <vector>

wxIMPLEMENT_APP(SpertApp);

bool SpertApp::OnInit()
{
	SpertFrame* spertFrame = new SpertFrame("Spert Chess");
	spertFrame->SetSize(wxSize(600, 600));
	spertFrame->Center();
	spertFrame->Show(true);
	playground::SetPlayground();
	
	return true;
}

int SpertApp::OnExit()
{
	playground::CleanPlayground();

	return wxApp::OnExit();
}

namespace playground
{
	Game joc = Game();
	Position lastPositions[24];					// stores the last 12 positions occured once in the game
	Position repeatedPositions[24];				// stores the last 12 positions occured twice in the game
	OpeningBook* bookRoot = new OpeningBook();	// the root of the opening book
	OpeningBook* bookPtr = bookRoot;			// current variation in the opening book
	std::vector<Position>moveList;		// the list of current game moves
	size_t moveIndex = 0;				// position from list currently displayed (needed for replay moves)

	void SetPlayground()
	{
		joc = Game();
		bookRoot = new OpeningBook();
		bookPtr = bookRoot;
		moveIndex = 0;
		LoadBook();								// loads opening book moves from file
	}

	void LoadBook()
	{
		std::ifstream Book("book.txt");
		if (!Book.is_open())
		{
			joc.inBook = false;		// if cant open file, the start position will be considered out of book
			return;
		}

		// add each variation from file to OpeningBook tree
		std::string line;
		while (std::getline(Book, line))
		{
			OpeningBook* var = bookRoot;
			SideToMove colour = white;   // first move is white's
			std::istringstream iss(line);
			std::string word;
			while (iss >> word)		// every word of a line from file is a move
			{
				Mutare openingMove = Mutare();
				if (word == "O-O")
				{
					openingMove.piece = colour == white ? wK : bK;
					openingMove.origCol = e;
					openingMove.origRow = colour == white ? 1 : 8;
					openingMove.destCol = g;
					openingMove.destRow = colour == white ? 1 : 8;
				}
				else if (word == "O-O-O")
				{
					openingMove.piece = colour == white ? wK : bK;
					openingMove.origCol = e;
					openingMove.origRow = colour == white ? 1 : 8;
					openingMove.destCol = c;
					openingMove.destRow = colour == white ? 1 : 8;
				}
				else
				{
					openingMove.piece = wP;
					size_t it = 0;
					if (word[0] == 'K')
						openingMove.piece = wK;
					else if (word[0] == 'Q')
						openingMove.piece = wQ;
					else if (word[0] == 'R')
						openingMove.piece = wR;
					else if (word[0] == 'B')
						openingMove.piece = wB;
					else if (word[0] == 'N')
						openingMove.piece = wN;
					else
						it = -1;
					if (colour == black)
						openingMove.piece *= -1;
					openingMove.origCol = (int)word[++it] - 96;
					openingMove.origRow = (int)word[++it] - 48;
					openingMove.destCol = (int)word[++it] - 96;
					openingMove.destRow = (int)word[++it] - 48;
					if (++it < word.size())
					{
						if (word[it] == 'Q')
							openingMove.promotion = wQ;
						else if (word[it] == 'R')
							openingMove.promotion = wR;
						else if (word[it] == 'B')
							openingMove.promotion = wB;
						else if (word[it] == 'N')
							openingMove.promotion = wN;
						if (colour == black)
							openingMove.promotion *= -1;
					}
				}

				// add variation to the tree, if the move is not a child of the node yet
				bool foundMove = false;
				for (size_t i = 0; i < var->children.size(); ++i)
				{
					if (openingMove == var->children[i].node)
					{
						var = &var->children[i];
						foundMove = true;
						break;
					}
				}
				if (foundMove == false)
				{
					OpeningBook newChild;
					newChild.node = openingMove;
					var->children.push_back(newChild);
					int n = var->children.size() - 1;
					var = &var->children[n];
				}

				if (colour == white)
					colour = black;
				else if (colour == black)
					colour = white;
			}
		}

		bookPtr = bookRoot;
		Book.close();
	}

	void CleanPlayground()
	{
		if (bookRoot != nullptr)
		{
			delete bookRoot;
			bookRoot = nullptr;
			bookPtr = nullptr;
		}

		moveList.clear();
	}
}
#include "SpertFrame.h"

#include <vector>

SpertFrame::SpertFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	SetupSizers();
	SetMaxSize(wxSize(900, 900));

	wxMenuBar* menuBar = new wxMenuBar();

	wxMenu* menuFile = new wxMenu();
	menuBar->Append(menuFile, "Menu");
	wxMenuItem* newGame = menuFile->Append(ID_NewGame, "&New Game\tCtrl+N");
	menuFile->AppendSeparator();
	wxMenuItem* computerToMove = menuFile->Append(ID_ComputerToMove, "Computer To Move\tCtrl+C");
	menuFile->AppendSeparator();
	wxMenuItem* setRapidGame = menuFile->Append(ID_Rapid, "&Game Speed:   Rapid");
	wxMenuItem* setNormalGame = menuFile->Append(ID_Normal, "&Game Speed:   Normal");
	wxMenuItem* setLongGame = menuFile->Append(ID_Long, "&Game Speed:   Slow");
	menuFile->AppendSeparator();
	wxMenuItem* offerDraw = menuFile->Append(ID_Draw_Offer, "Offer Draw");
	wxMenuItem* resigneGame = menuFile->Append(ID_Resign, "Resign Game");
	menuFile->AppendSeparator();
	wxMenuItem* saveGame = menuFile->Append(ID_Save, "Save Position\tCtrl+Shift+S");
	wxMenuItem* loadGame = menuFile->Append(ID_Load, "Load Position\tCtrl+Shift+L");
	menuFile->AppendSeparator();
	wxMenuItem* about = menuFile->Append(wxID_ABOUT, "About\tCtrl+I");
	menuFile->AppendSeparator();
	wxMenuItem* exit = menuFile->Append(wxID_EXIT, "Exit\tCtrl+Q");

	SetMenuBar(menuBar);
	ResetToolBar();

	if (playground::joc.wPlayer == human && playground::joc.position.side == white)
	{
		for (int i = 0; i < 64; ++i)
		{
			SetSquareControls(i);
		}
	}
	else
	{
		for (int i = 0; i < 64; ++i)
		{
			int j = 63 - i;
			SetSquareControls(j);
		}
	}

	sizer->SetSizeHints(this);

	Bind(wxEVT_MENU, &SpertFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &SpertFrame::OnNewGame, this, ID_NewGame);
	Bind(wxEVT_MENU, &SpertFrame::OnResign, this, ID_Resign);
	Bind(wxEVT_MENU, &SpertFrame::OnDrawOffer, this, ID_Draw_Offer);
	Bind(wxEVT_MENU, &SpertFrame::OnRapidPlay, this, ID_Rapid);
	Bind(wxEVT_MENU, &SpertFrame::OnNormalPlay, this, ID_Normal);
	Bind(wxEVT_MENU, &SpertFrame::OnLongPlay, this, ID_Long);
	Bind(wxEVT_MENU, &SpertFrame::OnComputerToMove, this, ID_ComputerToMove);
	Bind(wxEVT_MENU, &SpertFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &SpertFrame::OnSaveGame, this, ID_Save);
	Bind(wxEVT_MENU, &SpertFrame::OnLoadGame, this, ID_Load);
	Bind(wxEVT_TOOL, &SpertFrame::OnLeftArrow, this, ID_Left_Arrow);
	Bind(wxEVT_TOOL, &SpertFrame::OnRightArrow, this, ID_Right_Arrow);
	
	Bind(wxEVT_SIZE, &SpertFrame::OnFrameSize, this);
	panel->Bind(wxEVT_SIZE, &SpertFrame::OnResize, this);
}

void SpertFrame::CreateControls()
{
	panel = new wxPanel(this);
	panel->SetBackgroundColour(BACKGROUND);		// 144 120 96
	wxImage::AddHandler(new wxPNGHandler());
	for (int i = 0; i < 64; ++i)
		squares[i] = new wxPanel(panel);
}

void SpertFrame::SetupSizers()
{
	sizer = new wxBoxSizer(wxVERTICAL);
	boardSizer = new wxGridSizer(8, 8, 3, 3);
	sizer->Add(boardSizer, 1, wxEXPAND | wxALL, 18);
	panel->SetSizer(sizer);
}

void SpertFrame::SetSquareControls(int i)
{
	wxImage image;
	int row = 8 - i / 8;
	int col = 1 + i % 8;
	int piece = initBOARD[row][col];

	squareControls[i] = new wxStaticBitmap(squares[i], wxID_ANY, bitmap[i]);
	PaintSquare(i, piece, 60, image, bitmap[i]);

	if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
	{
		squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
	}
	else
	{
		squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
	}

	boardSizer->Add(squares[i], 1, wxSHAPED);
	squareControls[i]->Bind(wxEVT_LEFT_DOWN, &SpertFrame::OnSquareClicked, this);
}

void SpertFrame::PaintSquare(int i, int piece, int size, wxImage& image, wxBitmap& bitmap)
{
	size -= 5;
	switch (piece)
	{
	case wP:
		image = wxImage("white_pawn.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case bP:
		image = wxImage("black_pawn.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case wN:
		image = wxImage("white_knight.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case bN:
		image = wxImage("black_knight.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case wB:
		image = wxImage("white_bishop.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case bB:
		image = wxImage("black_bishop.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case wR:
		image = wxImage("white_rook.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case bR:
		image = wxImage("black_rook.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case wQ:
		image = wxImage("white_queen.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case bQ:
		image = wxImage("black_queen.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case wK:
		image = wxImage("white_king.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	case bK:
		image = wxImage("black_king.png", wxBITMAP_TYPE_PNG);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
		break;
	default:
		wxImage image(68, 68);
		image.SetMaskColour(0, 0, 0);
		image.Rescale(size, size, wxIMAGE_QUALITY_HIGH);
		bitmap = wxBitmap(image);
	}
	squareControls[i]->SetBitmap(bitmap);
}

void SpertFrame::ResetToolBar()
{
	toolBar->ClearTools();
	toolBar->SetBackgroundColour(BLACK_SQUARE);
	wxStaticText* txt;
	wxString str = "";
	int fontSize = 10;
	if (playground::joc.state == playing)
	{
		wxBitmap sideBitmap(14, 14);
		wxMemoryDC dc;
		dc.SelectObject(sideBitmap);
		dc.SetBackground(BLACK_SQUARE);
		dc.Clear();
		dc.SetBrush(sideColour);
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawCircle(7, 7, 7);
		dc.SelectObject(wxNullBitmap);
		toolBar->AddTool(wxID_ANY, wxT(""), sideBitmap);

		toolBar->AddStretchableSpace();
		
		if (playground::joc.speed == normal)
			str = "Normal  Game   ";
		else if (playground::joc.speed == rapid)
			str = "Rapid  Game   ";
		else if (playground::joc.speed == slow)
			str = "Slow  Game   ";
	}
	else if (playground::joc.state == finished)
	{
		toolBar->AddStretchableSpace();

		toolBar->AddTool(ID_Left_Arrow, _("Custom Control"), wxBitmap("arrow_left.png", wxBITMAP_TYPE_PNG),
			wxNullBitmap, wxITEM_NORMAL);
		toolBar->AddTool(ID_Right_Arrow, _("Custom Control"), wxBitmap("arrow_right.png", wxBITMAP_TYPE_PNG),
			wxNullBitmap, wxITEM_NORMAL);

		toolBar->AddStretchableSpace();

		fontSize = 16;
		
		if (playground::joc.result == win)
			str = "1 - 0   ";
		else if (playground::joc.result == lose)
			str = "0 - 1   ";
		else if (playground::joc.result == draw)
			str = "1/2 - 1/2   ";
	}

	txt = new wxStaticText(toolBar, wxID_ANY, str);
	wxFont font = txt->GetFont();
	font.SetPointSize(fontSize);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	txt->SetFont(font);
	txt->SetForegroundColour(wxColour(255, 255, 255));

	toolBar->AddControl(txt);
	toolBar->Realize();
}

void SpertFrame::OnFrameSize(wxSizeEvent& event)
{
	int width = GetClientSize().GetWidth() / 8;
	int height = GetClientSize().GetHeight() / 8;
	if (width > height)
	{
		width = height;
	}
	else
	{
		height = width;
	}
	SetClientSize(wxSize(width * 8, height * 8));
	Refresh();
	event.Skip();
}

void SpertFrame::OnResize(wxSizeEvent& event)
{
	int width = event.GetSize().GetWidth();
	int height = event.GetSize().GetHeight();
	int size = height;
	if (width > height)
	{
		size = width;
	}
	size /= 8;

	prevFromSquare = -1;
	prevToSquare = -1;
	fromSelected = -1;
	toSelected = -1;

	for (int i = 0; i < 64; ++i)
	{
		wxImage image;
		wxBitmap bitmap;
		int row = 8 - i / 8;
		int col = 1 + i % 8;
		int piece = playground::joc.position.board[row][col];
		int aux = i;
		if (playground::joc.wPlayer == computer)
			i = 63 - i;

		PaintSquare(i, piece, size, image, bitmap);

		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}

		i = aux;
	}

	panel->Refresh();
	event.Skip();
}

void SpertFrame::OnSquareClicked(wxMouseEvent& event)
{
	if (playground::joc.state != playing)
		return;
	if (playground::joc.position.side == white && playground::joc.wPlayer == computer)
		return;
	if (playground::joc.position.side == black && playground::joc.bPlayer == computer)
		return;

	int i;
	if (prevFromSquare != -1 && prevToSquare != -1)
	{
		i = prevFromSquare;
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
		squareControls[i]->Refresh();

		i = prevToSquare;
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
		squareControls[i]->Refresh();
	}

	wxPoint mousePos = ScreenToClient(wxGetMousePosition());
	int x = mousePos.x;
	int y = mousePos.y;
	int size = panel->GetSize().GetWidth() / 8;
	int col = x / size;
	int row = y / size;
	i = row * 8 + col;

	if (fromSelected == -1)
	{
		fromSelected = i;
		squareControls[i]->SetBackgroundColour(CLICKED_SQUARE);
	}
	else if (toSelected == -1)
	{
		toSelected = i;
		if (fromSelected == toSelected)
		{
			if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
			{
				squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
			}
		}
		else
		{
			squareControls[i]->SetBackgroundColour(CLICKED_SQUARE);
		}
	}
	squareControls[i]->Refresh();

	if (fromSelected != -1 && toSelected != -1)
	{
		i = fromSelected;
		if (playground::joc.wPlayer == computer)
			i = 63 - i;
		size_t fromCol = static_cast<size_t>(1) + i % 8;
		size_t fromRow = static_cast<size_t>(8) - i / 8;
		i = toSelected;
		if (playground::joc.wPlayer == computer)
			i = 63 - i;
		size_t toCol = static_cast<size_t>(1) + i % 8;;
		size_t toRow = static_cast<size_t>(8) - i / 8;
		int piece = playground::joc.position.board[fromRow][fromCol];
		Mutare mutare = Mutare(piece, fromCol, fromRow, toCol, toRow);
		int promotion = 0;
		if ((playground::joc.position.side == white && playground::joc.position.board[fromRow][fromCol] == wP && fromRow == 7) ||
			(playground::joc.position.side == black && playground::joc.position.board[fromRow][fromCol] == bP && fromRow == 2))
		{
			promotion = wQ;
			if (playground::joc.position.side == black)
				promotion *= -1;
		}
		mutare.promotion = promotion;
		playground::joc.gameChess.moves.clear();
		playground::joc.gameChess.GenerateMoves(playground::joc.position);

		if (playground::joc.IsValidMove(mutare))
		{
			if (mutare.promotion == wQ || mutare.promotion == bQ)
			{
				wxArrayString choices;
				choices.Add("Queen");
				choices.Add("Rook");
				choices.Add("Bishop");
				choices.Add("Knight");

				wxSingleChoiceDialog dialog(this,
					"Choose a piece for pawn promotion",
					"Pawn Promotion",
					choices);

				if (dialog.ShowModal() == wxID_OK)
				{
					wxString choice = dialog.GetStringSelection();

					if (choice == "Queen")
						mutare.promotion = wQ;
					else if (choice == "Rook")
						mutare.promotion = wR;
					else if (choice == "Bishop")
						mutare.promotion = wB;
					else if (choice == "Knight")
						mutare.promotion = wN;
				}
				if (mutare.piece < 0)
					mutare.promotion *= -1;
			}

			if (prevFromSquare != -1 && prevToSquare != -1)
			{
				WipeLastMove();
			}
			int piece = playground::joc.position.board[mutare.origRow][mutare.origCol];
			if (mutare.promotion)
				piece = mutare.promotion;

			ExecuteMove(mutare, piece, size);
			fromSelected = -1;
			toSelected = -1;

			if (playground::joc.state == playing)
			{
				mutare = playground::joc.ComputerPlays();
				if (playground::joc.inBook && !playground::joc.IsValidMove(mutare))
				{
					playground::joc.inBook = false;
					mutare = playground::joc.ComputerPlays();
				}

				if (prevFromSquare != -1 && prevToSquare != -1)
				{
					WipeLastMove();
				}

				piece = playground::joc.position.board[mutare.origRow][mutare.origCol];
				if (mutare.promotion)
					piece = mutare.promotion;

				ExecuteMove(mutare, piece, size);
				fromSelected = -1;
				toSelected = -1;
			}
		}
		else
		{
			i = fromSelected;
			if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
			{
				squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
			}
			squareControls[i]->Refresh();
			i = toSelected;
			if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
			{
				squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
			}
			squareControls[i]->Refresh();
			fromSelected = -1;
			toSelected = -1;
		}
	}

	event.Skip();
}

void SpertFrame::WipeLastMove()
{
	int i = prevFromSquare;
	if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
	{
		squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
	}
	else
	{
		squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
	}
	squareControls[i]->Refresh();

	i = prevToSquare;
	if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
	{
		squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
	}
	else
	{
		squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
	}
	squareControls[i]->Refresh();
}

void SpertFrame::ExecuteMove(Mutare mutare, int piece, int size)
{
	if (playground::joc.drawOffered)
	{
		if (playground::joc.position.side == white && playground::joc.wPlayer == computer)
		{
			if (playground::joc.position.evaluation < -50)
			{
				playground::joc.result = draw;
				playground::joc.state = finished;
				wxMessageBox("Agreed Draw!");
				sideColour = BLACK_SQUARE;
				ResetToolBar();
				return;
			}
			else
			{
				playground::joc.drawOffered = false;
			}
		}
		else if (playground::joc.position.side == black && playground::joc.bPlayer == computer)
		{
			if (playground::joc.position.evaluation > 50)
			{
				playground::joc.result = draw;
				playground::joc.state = finished;
				wxMessageBox("Agreed Draw!");
				sideColour = BLACK_SQUARE;
				ResetToolBar();
				return;
			}
			else
			{
				playground::joc.drawOffered = false;
			}
		}
	}
	else if (playground::joc.position.side == white && playground::joc.wPlayer == computer
		&& playground::joc.position.evaluation < -500 && playground::joc.position.wQueens == 0)
	{
		playground::joc.result = lose;
		playground::joc.state = finished;
		wxMessageBox("Spert resigns:   Black wins!");
		sideColour = BLACK_SQUARE;
		ResetToolBar();
		return;
	}
	else if (playground::joc.position.side == black && playground::joc.bPlayer == computer 
		&& playground::joc.position.evaluation > 500 && playground::joc.position.bQueens == 0)
	{
		playground::joc.result = win;
		playground::joc.state = finished;
		wxMessageBox("Spert resigns:   White wins!");
		sideColour = BLACK_SQUARE;
		ResetToolBar();
		return;
	}

	wxImage image;
	wxBitmap bitmap;
	int castleR = 0;
	int corner = 0;
	bool castle = false;
	int i = 0;
	if (piece == wK && mutare.origCol == e && mutare.destCol == g)
	{
		i = 61;
		castleR = wR;
		corner = 63;
		castle = true;
	}
	else if (piece == wK && mutare.origCol == e && mutare.destCol == c)
	{
		i = 59;
		castleR = wR;
		corner = 56;
		castle = true;
	}
	else if (piece == bK && mutare.origCol == e && mutare.destCol == g)
	{
		i = 5;
		castleR = bR;
		corner = 7;
		castle = true;
	}
	else if (piece == bK && mutare.origCol == e && mutare.destCol == c)
	{
		i = 3;
		castleR = bR;
		corner = 0;
		castle = true;
	}
	if (castle == true)
	{
		if (playground::joc.wPlayer == computer)
		{
			i = 63 - i;
			corner = 63 - corner;
		}
		PaintSquare(i, castleR, size, image, bitmap);
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
		squareControls[i]->Refresh();
		if ((playground::joc.position.side == white && playground::joc.wPlayer == human)
			|| (playground::joc.position.side == black && playground::joc.bPlayer == human))
		{
			squareControls[i]->Update();
			squareControls[i]->Refresh();
		}
		i = corner;
		PaintSquare(i, 0, size, image, bitmap);
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
		squareControls[i]->Refresh();
		if ((playground::joc.position.side == white && playground::joc.wPlayer == human)
			|| (playground::joc.position.side == black && playground::joc.bPlayer == human))
		{
			squareControls[i]->Update();
			squareControls[i]->Refresh();
		}
	}

	if ((piece == wP && playground::joc.position.enPassant == mutare.destCol && mutare.origRow == 5)
		|| (piece == bP && playground::joc.position.enPassant == mutare.destCol && mutare.origRow == 4))
	{
		i = (8 - mutare.origRow) * 8 + playground::joc.position.enPassant - 1;
		if (playground::joc.wPlayer == computer)
			i = 63 - i;
		PaintSquare(i, 0, size, image, bitmap);
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
		squareControls[i]->Refresh();
		if ((playground::joc.position.side == white && playground::joc.wPlayer == human)
			|| (playground::joc.position.side == black && playground::joc.bPlayer == human))
		{
			squareControls[i]->Update();
			squareControls[i]->Refresh();
		}
	}

	i = (8 - mutare.destRow) * 8 + mutare.destCol - 1;
	if (playground::joc.wPlayer == computer)
		i = 63 - i;
	PaintSquare(i, piece, size, image, bitmap);
	squareControls[i]->SetBackgroundColour(CLICKED_SQUARE);
	squareControls[i]->Refresh();
	if ((playground::joc.position.side == white && playground::joc.wPlayer == human)
		|| (playground::joc.position.side == black && playground::joc.bPlayer == human))
	{
		squareControls[i]->Update();
	}
	prevToSquare = i;

	piece = 0;
	i = (8 - mutare.origRow) * 8 + mutare.origCol - 1;
	if (playground::joc.wPlayer == computer)
		i = 63 - i;
	PaintSquare(i, piece, size, image, bitmap);
	squareControls[i]->SetBackgroundColour(CLICKED_SQUARE);
	squareControls[i]->Refresh();
	if ((playground::joc.position.side == white && playground::joc.wPlayer == human)
		|| (playground::joc.position.side == black && playground::joc.bPlayer == human))
	{
		squareControls[i]->Update();
	}
	prevFromSquare = i;

	playground::joc.depth = static_cast<size_t>(DEPTH);
	if (playground::joc.speed == rapid)
		playground::joc.depth = static_cast<size_t>(DEPTH) - 1;
	else if (playground::joc.speed == slow)
		playground::joc.depth = static_cast<size_t>(DEPTH) + 1;

	if (playground::joc.position.figureCount == 0)
		playground::joc.depth +=  3;
	else if (playground::joc.position.figureCount == 1)
		playground::joc.depth += 2;
	else if (playground::joc.position.figureCount == 2)
		playground::joc.depth++;

	wxSound moveSound("sound_move.wav");
	if (moveSound.IsOk())
	{
		if (playground::joc.wPlayer == computer && playground::joc.position.side == white)
			moveSound.Play();
		else if (playground::joc.bPlayer == computer && playground::joc.position.side == black)
			moveSound.Play();
	}

	playground::joc.position = mutare.MakeMove(playground::joc.position);

	playground::moveList.push_back(playground::joc.position);
	playground::moveIndex++;
	for (Position& p : playground::repeatedPositions)
	{
		if (p == playground::joc.position)
		{
			playground::joc.result = draw;
			playground::joc.state = finished;
			wxMessageBox("Three-fold repetition:   Draw!");
			sideColour = BLACK_SQUARE;
			ResetToolBar();
			return;
		}
	}
	for (Position& p : playground::lastPositions)
	{
		if (p == playground::joc.position)
		{
			playground::repeatedPositions[playground::joc.position.halfMoves % 12] = playground::joc.position;
			break;
		}
	}
	playground::lastPositions[playground::joc.position.halfMoves % 6] = playground::joc.position;

	if (playground::joc.position.figureCount < 2)
	{
		bool low_material = true;
		for (size_t i = 1; i < 9; ++i)
		{
			for (size_t j = 1; j < 9; ++j)
			{
				int p = playground::joc.position.board[j][i];
				if (p == wP || p == bP || p == wQ || p == bQ || p == wR || p == bR)
				{
					low_material = false;
					break;
				}
			}
			if (low_material == false)
				break;
		}
		if (low_material == true)
		{
			playground::joc.result = draw;
			playground::joc.state = finished;
			wxMessageBox("Insufficient material:   Draw!");
			sideColour = BLACK_SQUARE;
			ResetToolBar();
			return;
		}
	}

	playground::joc.gameChess.moves.clear();
	playground::joc.gameChess.GenerateMoves(playground::joc.position);
	wxString endMessage = "";
	if (playground::joc.gameChess.moves.empty())
	{
		if (playground::joc.position.inCheck)
		{
			if (playground::joc.position.side == black)
			{
				playground::joc.result = win;
				playground::joc.state = finished;
				endMessage = "Checkmate:   White wins!";
			}
			else if (playground::joc.position.side == white)
			{
				playground::joc.result = lose;
				playground::joc.state = finished;
				endMessage = "Checkmate:   Black wins!";
			}
		}
		else
		{
			playground::joc.result = draw;
			playground::joc.state = finished;
			endMessage = "Stalemate:   Draw!";
		}
	}
	else if (playground::joc.position.fiftyMoveChecker > 99)
	{
		playground::joc.result = draw;
		playground::joc.state = finished;
		endMessage = "Fifty-move rule:   Draw!";
	}

	if (playground::joc.state == finished)
	{
		wxMessageBox(endMessage);
		sideColour = BLACK_SQUARE;
	}
	else
	{
		if (playground::joc.position.side == white)
			sideColour = *wxWHITE;
		else
			sideColour = *wxBLACK;

		if (playground::joc.inBook == true)
		{
			bool in_b = false;
			for (size_t i = 0; i < playground::bookPtr->children.size(); ++i)
			{
				if (mutare == playground::bookPtr->children[i].node)
				{
					playground::bookPtr = &playground::bookPtr->children[i];
					in_b = true;
					break;
				}
			}
			playground::joc.inBook = in_b;
		}
	}
	ResetToolBar();
}

void SpertFrame::OnExit(wxCommandEvent& event)
{
	if (playground::bookRoot != nullptr)
	{
		delete playground::bookRoot;
		playground::bookRoot = nullptr;
		playground::bookPtr = nullptr;
	}
	Close();
}

void SpertFrame::OnNewGame(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;

	playground::joc = Game();

	playground::bookPtr = playground::bookRoot;
	playground::joc.inBook = (playground::bookPtr->children.size() != 0);

	sideColour = *wxWHITE;
	ResetToolBar();

	int width = GetClientSize().GetWidth();
	int height = GetClientSize().GetHeight();
	int size = height;
	if (width > height)
	{
		size = width;
	}
	size /= 8;
	for (int i = 0; i < 64; ++i)
	{
		wxImage image;
		wxBitmap bitmap;
		int row = 8 - i / 8;
		int col = 1 + i % 8;
		int piece = initBOARD[row][col];

		PaintSquare(i, piece, size, image, bitmap);

		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
	}
	Refresh();
}

void SpertFrame::OnResign(wxCommandEvent& event)
{
	if (playground::joc.state != playing)
		return;
	if (playground::joc.position.side == white && playground::joc.wPlayer == computer)
		return;
	if (playground::joc.position.side == black && playground::joc.bPlayer == computer)
		return;
	if (playground::joc.position.halfMoves == 0)
		return;
	
	if (playground::joc.position.side == white && playground::joc.wPlayer == human)
	{
		playground::joc.result = lose;
		playground::joc.state = finished;
		wxMessageBox("Human has resigned:   Black wins!");
		sideColour = BLACK_SQUARE;
		ResetToolBar();
		return;
	}
	if (playground::joc.position.side == black && playground::joc.bPlayer == human)
	{
		playground::joc.result = win;
		playground::joc.state = finished;
		wxMessageBox("Human has resigned:   White wins!");
		sideColour = BLACK_SQUARE;
		ResetToolBar();
		return;
	}
}

void SpertFrame::OnDrawOffer(wxCommandEvent& event)
{
	if (playground::joc.state != playing)
		return;
	if (playground::joc.isComputing)
		return;
	if (playground::joc.position.halfMoves < 60)
		return;

	playground::joc.drawOffered = true;
}

void SpertFrame::OnRapidPlay(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;

	playground::joc.speed = rapid;
	ResetToolBar();
}

void SpertFrame::OnNormalPlay(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;

	playground::joc.speed = normal;
	ResetToolBar();
}

void SpertFrame::OnLongPlay(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;

	playground::joc.speed = slow;
	ResetToolBar();
}

void SpertFrame::OnComputerToMove(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;
	if (playground::joc.state == finished)
	{
		wxLogMessage("Current game is finished. Start a new one.");
		return;
	}

	if (playground::joc.position.side == white)
	{
		playground::joc.wPlayer = computer;
		playground::joc.bPlayer = human;
	}
	else if (playground::joc.position.side == black)
	{
		playground::joc.wPlayer = human;
		playground::joc.bPlayer = computer;
	}
	prevFromSquare = -1;
	prevToSquare = -1;

	int width = GetClientSize().GetWidth();
	int height = GetClientSize().GetHeight();
	int size = height;
	if (width > height)
	{
		size = width;
	}
	size /= 8;

	if (playground::joc.wPlayer == human)
	{
		for (int i = 0; i < 64; ++i)
		{
			wxImage image;
			wxBitmap bitmap;
			int row = 8 - i / 8;
			int col = 1 + i % 8;
			int piece = playground::joc.position.board[row][col];

			PaintSquare(i, piece, size, image, bitmap);

			if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
			{
				squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
			}
		}
	}
	else
	{
		for (int i = 0; i < 64; ++i)
		{
			int j = 63 - i;

			wxImage image;
			wxBitmap bitmap;
			int row = 8 - i / 8;
			int col = 1 + i % 8;
			int piece = playground::joc.position.board[row][col];

			PaintSquare(j, piece, size, image, bitmap);

			if (((j % 2 == 0 && (j / 8) % 2 == 0)) || (j % 2 == 1 && (j / 8) % 2 == 1))
			{
				squareControls[j]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[j]->SetBackgroundColour(BLACK_SQUARE);
			}
		}
	}
	Refresh();
	Update();

	if (prevFromSquare != -1 && prevToSquare != -1)
	{
		WipeLastMove();
	}

	Mutare mutare = playground::joc.ComputerPlays();
	
	int piece = playground::joc.position.board[mutare.origRow][mutare.origCol];
	if (mutare.promotion)
		piece = mutare.promotion;

	ExecuteMove(mutare, piece, size);
	fromSelected = -1;
	toSelected = -1;
	Refresh();
}

void SpertFrame::OnSaveGame(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;

	std::ofstream Save("save.txt");
	if (!Save.is_open())
	{
		wxLogMessage("Can't open save file.");
		return;
	}
	for (size_t i = 0; i < 10; ++i)
	{
		for (size_t j = 0; j < 10; ++j)
		{
			Save << playground::joc.position.board[i][j] << ' ';
		}
		Save << '\n';
	}
	Save << playground::joc.position.side << '\n';
	Save << playground::joc.position.wCastle << '\n';
	Save << playground::joc.position.bCastle << '\n';
	Save << playground::joc.position.enPassant << '\n';
	Save << playground::joc.position.inCheck << '\n';
	Save << playground::joc.position.wKingCol << '\n';
	Save << playground::joc.position.wKingRow << '\n';
	Save << playground::joc.position.bKingCol << '\n';
	Save << playground::joc.position.bKingRow << '\n';
	Save << playground::joc.position.material << '\n';
	Save << playground::joc.position.figureCount << '\n';
	Save << playground::joc.position.fiftyMoveChecker << '\n';
	Save << playground::joc.position.wBishops << '\n';
	Save << playground::joc.position.bBishops << '\n';
	Save << playground::joc.position.wPawnsOnLight << '\n';
	Save << playground::joc.position.wPawnsOnDark << '\n';
	Save << playground::joc.position.bPawnsOnLight << '\n';
	Save << playground::joc.position.bPawnsOnDark << '\n';
	Save << playground::joc.position.halfMoves << '\n';

	Save.close();
}

void SpertFrame::OnLoadGame(wxCommandEvent& event)
{
	if (playground::joc.isComputing)
		return;

	std::ifstream Load("save.txt");
	if (!Load.is_open())
	{
		wxLogMessage("Can't open save file.");
		return;
	}

	Position load_position = Position();
	std::string line;
	for (size_t i = 0; i < 10; ++i)
	{
		std::getline(Load, line);
		std::istringstream iss(line);
		for (size_t j = 0; j < 10; ++j)
			iss >> load_position.board[i][j];
	}

	int loadValue;
	Load >> loadValue;
	load_position.side = (SideToMove)loadValue;
	Load >> loadValue;
	load_position.wCastle = (Castle)loadValue;
	Load >> loadValue;
	load_position.bCastle = (Castle)loadValue;
	Load >> load_position.enPassant;
	Load >> load_position.inCheck;
	Load >> load_position.wKingCol;
	Load >> load_position.wKingRow;
	Load >> load_position.bKingCol;
	Load >> load_position.bKingRow;
	Load >> load_position.material;
	Load >> load_position.figureCount;
	Load >> load_position.fiftyMoveChecker;
	Load >> load_position.wBishops;
	Load >> load_position.bBishops;
	Load >> load_position.wPawnsOnLight;
	Load >> load_position.wPawnsOnDark;
	Load >> load_position.bPawnsOnLight;
	Load >> load_position.bPawnsOnDark;
	Load >> load_position.halfMoves;

	Load.close();

	if (load_position.wKingCol < 1 || load_position.wKingCol > 8 
		|| load_position.wKingRow < 1 || load_position.wKingRow > 8
		|| load_position.bKingCol < 1 || load_position.bKingCol > 8 
		|| load_position.bKingRow < 1 || load_position.bKingRow > 8)
	{
		wxLogMessage("Cant find a valid position.");
		return;
	}

	if (!load_position.IsLegal())
	{
		wxLogMessage("Cant find a valid position.");
		return;
	}

	playground::joc = Game();
	playground::joc.position = load_position;
	playground::joc.inBook = false;
	if (playground::joc.position.side == white)
	{
		playground::joc.wPlayer = human;
		playground::joc.bPlayer = computer;
	}
	else if (playground::joc.position.side == black)
	{
		playground::joc.wPlayer = computer;
		playground::joc.bPlayer = human;
	}

	if (playground::joc.position.side == white)
		sideColour = *wxWHITE;
	else if (playground::joc.position.side == black)
		sideColour = *wxBLACK;
	ResetToolBar();

	int width = GetClientSize().GetWidth();
	int height = GetClientSize().GetHeight();
	int size = height;
	if (width > height)
	{
		size = width;
	}
	size /= 8;
	if (playground::joc.wPlayer == human)
	{
		for (int i = 0; i < 64; ++i)
		{
			wxImage image;
			wxBitmap bitmap;
			int row = 8 - i / 8;
			int col = 1 + i % 8;
			int piece = playground::joc.position.board[row][col];

			PaintSquare(i, piece, size, image, bitmap);

			if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
			{
				squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
			}
		}
	}
	else
	{
		for (int i = 0; i < 64; ++i)
		{
			int j = 63 - i;

			wxImage image;
			wxBitmap bitmap;
			int row = 8 - i / 8;
			int col = 1 + i % 8;
			int piece = playground::joc.position.board[row][col];

			PaintSquare(j, piece, size, image, bitmap);

			if (((j % 2 == 0 && (j / 8) % 2 == 0)) || (j % 2 == 1 && (j / 8) % 2 == 1))
			{
				squareControls[j]->SetBackgroundColour(WHITE_SQUARE);
			}
			else
			{
				squareControls[j]->SetBackgroundColour(BLACK_SQUARE);
			}
		}
	}
	Refresh();
}

void SpertFrame::OnAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName("Spert");
	aboutInfo.SetCopyright("                   (C) 2024 by Valentin Lavric");
	aboutInfo.SetDescription(//"\n"
		//"                                 __________\n\n\n"
		"                 ______________________________\n\n"
		"                 Spert Chess is an application\n"
		"                    for playing chess games\n"
		"                     against the computer."
		//"                                 __________"
		);

	wxAboutBox(aboutInfo);
}

void SpertFrame::OnLeftArrow(wxCommandEvent& event)
{
	if (playground::moveIndex < 1 || playground::moveIndex > playground::moveList.size())
		return;
	
	playground::moveIndex--;
	Position& pos = playground::moveList[playground::moveIndex];

	int width = GetClientSize().GetWidth();
	int height = GetClientSize().GetHeight();
	int size = height;
	if (width > height)
	{
		size = width;
	}
	size /= 8;
	for (int j = 0; j < 64; ++j)
	{
		int i = j;
		if (playground::joc.wPlayer == computer)
			i = 63 - j;
		wxImage image;
		wxBitmap bitmap;
		int row = 8 - i / 8;
		int col = 1 + i % 8;
		int piece = pos.board[row][col];

		if (piece == playground::moveList[playground::moveIndex + 1].board[row][col])
			continue;

		PaintSquare(j, piece, size, image, bitmap);
		i = j;
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
	}
	Refresh();
}

void SpertFrame::OnRightArrow(wxCommandEvent& event)
{
	if (playground::moveIndex < 0 || playground::moveIndex > playground::moveList.size() - 2)
		return;

	playground::moveIndex++;
	Position& pos = playground::moveList[playground::moveIndex];

	int width = GetClientSize().GetWidth();
	int height = GetClientSize().GetHeight();
	int size = height;
	if (width > height)
	{
		size = width;
	}
	size /= 8;
	for (int j = 0; j < 64; ++j)
	{
		int i = j;
		if (playground::joc.wPlayer == computer)
			i = 63 - j;
		wxImage image;
		wxBitmap bitmap;
		int row = 8 - i / 8;
		int col = 1 + i % 8;
		int piece = pos.board[row][col];

		if (piece == playground::moveList[playground::moveIndex - 1].board[row][col])
			continue;

		PaintSquare(j, piece, size, image, bitmap);
		i = j;
		if (((i % 2 == 0 && (i / 8) % 2 == 0)) || (i % 2 == 1 && (i / 8) % 2 == 1))
		{
			squareControls[i]->SetBackgroundColour(WHITE_SQUARE);
		}
		else
		{
			squareControls[i]->SetBackgroundColour(BLACK_SQUARE);
		}
	}
	Refresh();
}
#pragma once
#include "SpertApp.h"

const wxColour BACKGROUND(45, 60, 45);			// 48 96 72		
const wxColour WHITE_SQUARE(160, 240, 160);		// 255 206 158
const wxColour BLACK_SQUARE(90, 135, 90);		// 209 139 71
const wxColour CLICKED_SQUARE(72, 144, 180);	// 255 175 71

enum
{
	ID_NewGame = 100,
	ID_ComputerToMove = 101,
	ID_Rapid = 102,
	ID_Normal = 103,
	ID_Long = 104,
	ID_Left_Arrow = 105,
	ID_Right_Arrow = 106,
	ID_Resign = 107,
	ID_Draw_Offer = 108,
	ID_Save = 109,
	ID_Load = 110
};

class SpertFrame : public wxFrame
{
public:
	SpertFrame(const wxString& title);

private:
	void CreateControls();
	void SetupSizers();
	void SetSquareControls(int i);			
	void PaintSquare(int i, int piece, int size, wxImage& image, wxBitmap& bitmap);
	void ResetToolBar();					// toolbar for showing side to move and game speed
	void OnFrameSize(wxSizeEvent& event);	// resizing the main frame
	void OnResize(wxSizeEvent& event);		// resizing the squares of the board and pieces
	void OnSquareClicked(wxMouseEvent& event);
	void WipeLastMove();		// wipe previous move indication on the board (reset the colour of the 2 squares)
	void ExecuteMove(Mutare mutare, int piece, int size);// visual part of executing moves + game adjudecation +...
	void OnExit(wxCommandEvent& event);
	void OnNewGame(wxCommandEvent& event);
	void OnRapidPlay(wxCommandEvent& event);	// turn game speed to Rapid
	void OnNormalPlay(wxCommandEvent& event);	// turn game speed to Normal
	void OnLongPlay(wxCommandEvent& event);		// turn game speed to Long
	void OnComputerToMove(wxCommandEvent& event);// make computer compute current position and switch view (in case)
	void OnSaveGame(wxCommandEvent& event);		// save current position to disk
	void OnLoadGame(wxCommandEvent& event);		// load last saved position from disk and start a game with it
	void OnAbout(wxCommandEvent& event);
	void OnResign(wxCommandEvent& event);		// human resigns the game
	void OnDrawOffer(wxCommandEvent& event);	// human offers a draw
	void OnLeftArrow(wxCommandEvent& event);	// backward replay the moves of a finished game
	void OnRightArrow(wxCommandEvent& event);	// forward replay the moves of a finished game

	wxPanel* panel;				// main panel
	wxPanel* squares[64];		// the panels for every square of the board
	wxBoxSizer* sizer;
	wxGridSizer* boardSizer;
	wxBitmap bitmap[64];
	wxStaticBitmap* squareControls[64];
	wxToolBar* toolBar = CreateToolBar();
	wxColour sideColour = *wxWHITE;

	// info about last move on the board and mouse selected squares (-1 means none)
	int prevFromSquare = -1;
	int prevToSquare = -1;
	int fromSelected = -1;
	int toSelected = -1;
};
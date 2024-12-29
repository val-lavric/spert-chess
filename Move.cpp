#include "SpertApp.h"

Mutare::Mutare(int pieceA, size_t origColA, size_t origRowA, size_t destColA, size_t destRowA)
    : piece(pieceA), origCol(origColA), origRow(origRowA),
    destCol(destColA), destRow(destRowA), promotion(0) {}

bool Mutare::operator==(const Mutare& mut) const
{
    if (piece != mut.piece)
        return false;
    if (origCol != mut.origCol)
        return false;
    if (origRow != mut.origRow)
        return false;
    if (destCol != mut.destCol)
        return false;
    if (destRow != mut.destRow)
        return false;

    return true;
}

bool Mutare::operator!=(const Mutare& mut) const
{
    return !(*this == mut);
}

Position Mutare::MakeMove(Position& position) const
{
    Position newPosition(position);

    newPosition.board[origRow][origCol] = 0;
    newPosition.board[destRow][destCol] = piece;
    newPosition.wCastle = position.wCastle;
    newPosition.bCastle = position.bCastle;

    // material
    newPosition.material = position.material - position.board[destRow][destCol];
    // (see pawn specials for en-passant and promotion)

    // pawn specials:
    // promotion:
    if (promotion)
    {
        newPosition.board[destRow][destCol] = promotion;
        newPosition.material += promotion - piece;      // material update
    }
    // 2-square move generates en-passant possibility:
    newPosition.enPassant = 0; // premise that no en-passant is possible untill checking
    if (piece == wP && origRow == 2 && destRow == 4)
        newPosition.enPassant = destCol;
    else if (piece == bP && origRow == 7 && destRow == 5)
        newPosition.enPassant = destCol;
    // remove en-passant captured pawn:
    if (piece == wP && position.enPassant == destCol && origRow == 5)
    {
        newPosition.board[5][destCol] = 0;
        newPosition.material -= bP;                 // material update
        newPosition.board[9][destCol]--;            // pawn distribution update
    }
    else if (piece == bP && position.enPassant == destCol && origRow == 4)
    {
        newPosition.board[4][destCol] = 0;
        newPosition.material -= wP;                 // material update
        newPosition.board[0][destCol]--;            // pawn distribution update
    }
    // update pawn distribution
    if (piece == wP)
    {
        newPosition.board[0][origCol]--;
        newPosition.board[0][destCol]++;
    }
    else if (piece == bP)
    {
        newPosition.board[9][origCol]--;
        newPosition.board[9][destCol]++;
    }
    // update pawn colours
    if (piece == wP)
    {
        if ((origRow + origCol) % 2)
            newPosition.wPawnsOnLight--;        // white pawns on light squares
        else
            newPosition.wPawnsOnDark--;         // white pawns on dark squares
        if (promotion == 0)
        {
            if ((destRow + destCol) % 2)
                newPosition.wPawnsOnLight++;
            else
                newPosition.wPawnsOnDark++;
        }
        if (position.enPassant == destCol && origRow == 5)  // en-passant capture pawn
        {
            if ((6 + destCol) % 2)
                newPosition.bPawnsOnLight--;
            else
                newPosition.bPawnsOnDark--;
        }
    }
    else if (piece == bP)
    {
        if ((origRow + origCol) % 2)
            newPosition.bPawnsOnLight--;        // black pawns on light squares
        else
            newPosition.bPawnsOnDark--;         // black pawns on dark squares
        if (promotion == 0)
        {
            if ((destRow + destCol) % 2)
                newPosition.bPawnsOnLight++;
            else
                newPosition.bPawnsOnDark++;
        }
        if (position.enPassant == destCol && origRow == 4)
        {
            if ((3 + destCol) % 2)
                newPosition.wPawnsOnLight--;
            else
                newPosition.wPawnsOnDark--;
        }
    }
    if (position.board[destRow][destCol] == bP)         // captured pawn; 
    {
        if ((destRow + destCol) % 2)
            newPosition.bPawnsOnLight--;
        else
            newPosition.bPawnsOnDark--;
    }
    if (position.board[destRow][destCol] == wP)
    {
        if ((destRow + destCol) % 2)
            newPosition.wPawnsOnLight--;
        else
            newPosition.wPawnsOnDark--;
    }

    // modifying castle possibility by moving rook or king
    if (piece == wR)
    {
        if (origRow == 1 && origCol == a)
        {
            if (position.wCastle == both)
                newPosition.wCastle = kingside;
            else if (position.wCastle == queenside)
                newPosition.wCastle = none;
        }
        else if (origRow == 1 && origCol == h)
        {
            if (position.wCastle == both)
                newPosition.wCastle = queenside;
            else if (position.wCastle == kingside)
                newPosition.wCastle = none;
        }
    }
    else if (piece == wK && origCol == e)
    {
        if (destCol == g || destCol == c)
            newPosition.wCastle = done;
        else if (newPosition.wCastle != done)
            newPosition.wCastle = none;
    }
    else if (piece == bR)
    {
        if (origRow == 8 && origCol == a)
        {
            if (position.bCastle == both)
                newPosition.bCastle = kingside;
            else if (position.bCastle == queenside)
                newPosition.bCastle = none;
        }
        else if (origRow == 8 && origCol == h)
        {
            if (position.bCastle == both)
                newPosition.bCastle = queenside;
            else if (position.bCastle == kingside)
                newPosition.bCastle = none;
        }
    }
    else if (piece == bK && origCol == e)
    {
        if (destCol == g || destCol == c)
            newPosition.bCastle = done;
        else if (newPosition.bCastle != done)
            newPosition.bCastle = none;
    }
    // modifying castle possibility by capturing rook
    if (position.board[destRow][destCol] == wR)
    {
        if (destRow == 1 && destCol == h)
        {
            if (position.wCastle == both)
                newPosition.wCastle = queenside;
            else if (position.wCastle == kingside)
                newPosition.wCastle = none;
        }
        else if (destRow == 1 && destCol == a)
        {
            if (position.wCastle == both)
                newPosition.wCastle = kingside;
            else if (position.wCastle == queenside)
                newPosition.wCastle = none;
        }
    }
    else if (position.board[destRow][destCol] == bR)
    {
        if (destRow == 8 && destCol == h)
        {
            if (position.bCastle == both)
                newPosition.bCastle = queenside;
            else if (position.bCastle == kingside)
                newPosition.bCastle = none;
        }
        else if (destRow == 8 && destCol == a)
        {
            if (position.bCastle == both)
                newPosition.bCastle = kingside;
            else if (position.bCastle == queenside)
                newPosition.bCastle = none;
        }
    }

    // complete castle move by moving rook:
    if (piece == wK && origCol == e && destCol == g)
    {
        newPosition.board[1][h] = 0;
        newPosition.board[1][f] = wR;
    }
    else if (piece == wK && origCol == e && destCol == c)
    {
        newPosition.board[1][a] = 0;
        newPosition.board[1][d] = wR;
    }
    else if (piece == bK && origCol == e && destCol == g)
    {
        newPosition.board[8][h] = 0;
        newPosition.board[8][f] = bR;
    }
    else if (piece == bK && origCol == e && destCol == c)
    {
        newPosition.board[8][a] = 0;
        newPosition.board[8][d] = bR;
    }

    // update number of figures (knights + bishops + rooks + queens)
    if (position.board[destRow][destCol] > wP || position.board[destRow][destCol] < bP)
        newPosition.figureCount--;
    if (promotion != 0)
        newPosition.figureCount++;

    // fifty-moves update
    if (piece == wP || piece == bP)
            newPosition.fiftyMoveChecker = 0;
    else if (position.board[destRow][destCol] != 0)
        newPosition.fiftyMoveChecker = 0;
    else
        newPosition.fiftyMoveChecker++;

    // set king positions:
    if (piece == wK)
    {
        newPosition.wKingRow = destRow;
        newPosition.wKingCol = destCol;
    }
    else if (piece == bK)
    {
        newPosition.bKingRow = destRow;
        newPosition.bKingCol = destCol;
    }

    // bishop pairs and queens update
    if (position.board[destRow][destCol] == wB)
        newPosition.wBishops--;
    else if (position.board[destRow][destCol] == bB)
        newPosition.bBishops--;
    else if (position.board[destRow][destCol] == wQ)
        newPosition.wQueens--;
    else if (position.board[destRow][destCol] == bQ)
        newPosition.bQueens--;
    if (promotion == wB)
        newPosition.wBishops++;
    else if (promotion == bB)
        newPosition.bBishops++;
    else if (promotion == wQ)
        newPosition.wQueens++;
    else if (promotion == bQ)
        newPosition.bQueens++;

    // check if it gives check:
    newPosition.SwitchSide();
    newPosition.inCheck = false;
    if (newPosition.side == black && newPosition.WhiteAttack(newPosition.bKingCol, newPosition.bKingRow))
        newPosition.inCheck = true;
    else if (newPosition.side == white && newPosition.BlackAttack(newPosition.wKingCol, newPosition.wKingRow))
        newPosition.inCheck = true;

    newPosition.halfMoves++;

    return newPosition;
}
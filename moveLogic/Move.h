//
// Created by Tobi on 02.02.2025.
//

#ifndef TCHESS_MOVE_H
#define TCHESS_MOVE_H


#include "../pieces/Piece.h"
/**
 * Class holds all information to perform a move and undo it later only by its given information
 */
class Move {

public:

private:
    int from;
    int to;
    Piece* piece;
    Piece* pieceToBeat;

    bool isCastlingMove;
    bool isEnPassantMove;
    bool isPawnConversion;

    Piece convertPawnTo;


};


#endif //TCHESS_MOVE_H

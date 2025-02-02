//
// Created by Tobi on 25.01.2025.
//

#ifndef TCHESS_PIECE_H
#define TCHESS_PIECE_H

#include <string>


class Piece {


public:

    enum Type {
        EMPTY,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING,
        PAWN
    };

    int value;
    enum Type type;
    int color;

    Piece() : color(0), type(EMPTY), value(0) {}

    Piece(int value, enum Type type, int color);


};


#endif //TCHESS_PIECE_H

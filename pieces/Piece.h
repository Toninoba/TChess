//
// Created by Tobi on 25.01.2025.
//

#ifndef TCHESS_PIECE_H
#define TCHESS_PIECE_H

#include <string>

/**
 * The Piece class is more or less used as a data structure to make it easier for move generation and evaluation
 * I kept it all in one Class and didnt make a Class for each one since i want to keep it compactor
 */
class Piece {


public:

    enum Type {
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
    int pos;

    Piece(int value, enum Type type, int color, int pos) : color(color), type(type), value(value), pos(pos) {}
};


#endif //TCHESS_PIECE_H

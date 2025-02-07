//
// Created by Tobi on 25.01.2025.
//

#ifndef TCHESS_PIECE_H
#define TCHESS_PIECE_H

#include <string>
#include <iostream>

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

    friend std::ostream& operator<<(std::ostream& os, const Piece& p){

        std::string typeString;

        switch (p.type) {
            case Piece::PAWN:
                typeString = "Pawn";
                break;
            case Piece::QUEEN:
                typeString = "Queen";
                break;
            case Piece::KING:
                typeString = "King";
                break;
            case Piece::ROOK:
                typeString = "Rook";
                break;
            case Piece::BISHOP:
                typeString = "Bishop";
                break;
            case Piece::KNIGHT:
                typeString = "Knight";
                break;
        }



        os << "Piece: " << typeString << " pos: " << p.pos << " color: " << p.color;
        return os;
    }
};


#endif //TCHESS_PIECE_H

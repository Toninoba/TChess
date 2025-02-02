//
// Created by Tobi on 25.01.2025.
//

#ifndef TCHESS_BOARD_H
#define TCHESS_BOARD_H


#include <array>
#include "../pieces/Piece.h"
#include "../moveLogic/Move.h"
#include <string>
#include <vector>

class Board {

public:

    Piece board[64];
    int turnToMove;
    bool castlingWhiteK;
    bool castlingWhiteQ;
    bool castlingBlackK;
    bool castlingBlackQ;
    std::vector<Move> possibleEnPassant;
    int halfMoveClock;
    int fullMoveClock;


    Board();

    explicit Board(std::string fen);

    static void printBoard(Board b);

private:

    void parseFen(std::string fen);


};


#endif //TCHESS_BOARD_H

//
// Created by Tobi on 07.02.2025.
//

#ifndef TCHESS_MOVEGENERATOR_H
#define TCHESS_MOVEGENERATOR_H


#include <vector>
#include "Move.h"
#include "../Board/Board.h"

class MoveGenerator {

public:

    MoveGenerator(){
        calculateAllEdgeTiles();
    };

    std::shared_ptr<std::vector<Move>> generateLegalMoves(Board* board);

    int getEdgeTile(int pos, int dir);

private:

    int edgeTiles[64][8]{};

    const std::vector<std::vector<int>> PIECEDIRECTIONS = {
            {1, -1, 8, -8},                    // Rook
            {7, -7, 9, -9},                    // Bishop
            {1, -1, 8, -8, 7, -7, 9, -9},      // Queen
            {-17, -15, -10, -6, 6, 10, 15, 17} // Knight
    };
    void generateMovesForPiece(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* board);

    void generateSlidingMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard);

    void generateKingMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard);

    void generatePawnMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard);

    void generateKnightMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard);

    void calculateAllEdgeTiles();

};


#endif //TCHESS_MOVEGENERATOR_H

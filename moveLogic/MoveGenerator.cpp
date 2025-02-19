//
// Created by Tobi on 07.02.2025.
//

#include "MoveGenerator.h"
#define DEBUG


std::shared_ptr<std::vector<Move>> MoveGenerator::generateLegalMoves(Board *board) {

#ifdef DEBUG
    std::cout << "Starting Move Generation Routine" << std::endl;
#endif
    // Create a Pointer to a Vector that contains Moves, and return the Pointer to that vector in memory
    std::shared_ptr<std::vector<Move>> legalMoves = std::make_shared<std::vector<Move>>();
    legalMoves->reserve(40);

    // Pointer to Piece List in Board object
    std::vector<std::unique_ptr<Piece>>* playerPieceList = (board->turnToMove == 1) ? board->getWhitePieceList() : board->getBlackPieceList();
    std::vector<std::unique_ptr<Piece>>* opponentPieceList = (board->turnToMove == 1) ? board->getBlackPieceList() : board->getWhitePieceList();

    // iterate over piece list and give the uniquePointer reference to generation method
    for(auto& piecePtr : *playerPieceList){
        generateMovesForPiece(piecePtr, *legalMoves, board);
    }


#ifdef DEBUG
    std::cout << "Finished Move Generation\n Move List:" << std::endl;
    std::cout << legalMoves->size() << std::endl;
    for (const auto& move : *legalMoves) {
        std::cout << move << std::endl;
    }

#endif

    return legalMoves;
}

void MoveGenerator::generateMovesForPiece(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* board) {


    switch (piece->type) {
        case Piece::PAWN:
            break;
        case Piece::QUEEN:
            generateSlidingMoves(piece, moveList, board);
            break;
        case Piece::KING:
            generateKingMoves(piece, moveList, board);
            break;
        case Piece::ROOK:
            generateSlidingMoves(piece, moveList, board);
            break;
        case Piece::BISHOP:
            generateSlidingMoves(piece, moveList, board);
            break;
        case Piece::KNIGHT:
            generateKnightMoves(piece, moveList, board);
            break;
    }
}

void MoveGenerator::calculateAllEdgeTiles() {

    int directions[] = {1, -1, 8, -8, 7, -7, 9, -9};

    for (int i = 0; i < 64; ++i) {
        for(int j = 0; j < 8; j++) {

            int dir = directions[j];
            int pos = i;
            bool hitEdge = false;

            while (!hitEdge) {
                if (pos > 63 || pos < 0) {
                    hitEdge = true;
                    pos -= dir;
                } else if (((dir == 1 || dir == 9 || dir == -7) && pos % 8 == 7) ||
                           ((dir == -1 || dir == -9 || dir == 7) && pos % 8 == 0)) {
                    hitEdge = true;
                } else {
                    pos += dir;
                }

            }
            edgeTiles[i][j] = pos;
        }
    }
}

int MoveGenerator::getEdgeTile(int pos, int dir) {

    int read;

    switch (dir) {
        case 1:
            read = 0;
            break;
        case -1:
            read = 1;
            break;
        case 8:
            read = 2;
            break;
        case -8:
            read = 3;
            break;
        case 7:
            read = 4;
            break;
        case -7:
            read = 5;
            break;
        case 9:
            read = 6;
            break;
        case -9:
            read = 7;
            break;
    }


    return this->edgeTiles[pos][read];
}

void MoveGenerator::generateSlidingMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard) {

#ifdef DEBUG
    std::cout << "Starting Sliding Move Generation Routine" << std::endl;
#endif

    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;


    // Since Rook = 0 in Piece enum, access directly via type is possible
    for(const int& dir : this->PIECEDIRECTIONS[piece->type]){
#ifdef DEBUG
        std::cout << "Testing for direction: " << dir << std::endl;
#endif
        int edgeTile = getEdgeTile(piece->pos, dir);

        // Iterate from Piece position + direction to edge tile to that direction
        for (int i = piece->pos + dir; (dir > 0) ? (i <= edgeTile) : (i >= edgeTile); i+=dir) {

            // Break if position in direction dir hits a Piece of same color
            if(boardRef[i] && boardRef[i].value()->color == piece->color){
#ifdef DEBUG
                std::cout << "Found Piece block of same color at postion " << i << std::endl;
#endif
                break;
            }
            else if (boardRef[i] && boardRef[i].value()->color != piece->color){
#ifdef DEBUG
                std::cout << "Found legal move" << std::endl;
#endif
                // Break if piece exists since a Rook cannot jump over other Pieces
                moveList.emplace_back(piece->pos, i, piece, boardRef[i]);
                break;
            }
            else {
#ifdef DEBUG
                std::cout << "Found legal move" << std::endl;
#endif
                // Else i is empty Tile
                moveList.emplace_back(piece->pos, i, piece, std::optional<Piece*>{});
            }
        }
    }
}

void MoveGenerator::generateKingMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard) {

#ifdef DEBUG
    std::cout << "Starting King Move Generation Routine" << std::endl;
#endif

    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;


    // Since King Direction is equal to Queen Movement, use Queen direction array
    for(const int& dir : this->PIECEDIRECTIONS[2]){
#ifdef DEBUG
        std::cout << "Testing for direction: " << dir << std::endl;
#endif
        int nextPos = piece->pos + dir;

        if (nextPos >= 0 && nextPos < 64){

            if((piece->pos % 8 == 7 && nextPos % 8 == 0) || (piece->pos % 8 == 0 && nextPos % 8 == 7)){
                // Pass direction since king is on edge Tile and cannot jump over edge
                continue;
            }
            else {
                if (boardRef[nextPos] && boardRef[nextPos].value()->color != piece->color){
#ifdef DEBUG
                    std::cout << "Found Move at " << nextPos << std::endl;
#endif
                    moveList.emplace_back(piece->pos, nextPos, piece, boardRef[nextPos]);
                }
                else if(!boardRef[nextPos]){
#ifdef DEBUG
                    std::cout << "Found Move at " << nextPos << std::endl;
#endif
                    moveList.emplace_back(piece->pos, nextPos, piece, std::optional<Piece*>{});
                }
                else {
                    continue;
                }
            }

        }

    }






}

void MoveGenerator::generateKnightMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard) {
#ifdef DEBUG
    std::cout << "Starting Knight Move Generation Routine" << std::endl;
#endif

    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;

    for(const int& dir : PIECEDIRECTIONS[3]){
#ifdef DEBUG
        std::cout << "Testing for direction: " << dir << std::endl;
#endif

        int gotoPos = piece->pos + dir;

        if(gotoPos >= 0 && gotoPos <= 63){

            if((piece->pos % 8 == 0 || piece->pos % 8 == 1) && (gotoPos % 8 == 6 || gotoPos % 8 == 7)){
                continue;
            }
            if((gotoPos % 8 == 0 || gotoPos % 8 == 1) && (piece->pos % 8 == 6 || piece->pos % 8 == 7)){
                continue;
            }
            else {
                if(!boardRef[gotoPos]){
#ifdef DEBUG
                    std::cout << "Found Move at " << gotoPos << std::endl;
#endif
                    moveList.emplace_back(piece->pos, gotoPos, piece, std::optional<Piece*>{});
                }
                else{
                    if(boardRef[gotoPos].value()->color != piece->color){
#ifdef DEBUG
                        std::cout << "Found Move at " << gotoPos << std::endl;
#endif
                        moveList.emplace_back(piece->pos, gotoPos, piece, boardRef[gotoPos]);
                    }
                }
            }


        }
    }

}

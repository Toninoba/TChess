//
// Created by Tobi on 07.02.2025.
//

#include "MoveGenerator.h"
#define DEBUG


std::shared_ptr<std::vector<Move>> MoveGenerator::generateLegalMoves(Board *board) {

#ifdef DEBUG
    std::cout << "Starting Move Generation Routine" << std::endl;
#endif
    // Create a Pointer to a Vector that contains Moves, and return the Pointer to that vector has in memory
    std::shared_ptr<std::vector<Move>> playerLegalMoves = std::make_shared<std::vector<Move>>();
    std::shared_ptr<std::vector<Move>> enemyLegalMoves = std::make_shared<std::vector<Move>>();
    playerLegalMoves->reserve(40);

    // Pointer to Piece List in Board object
    std::vector<std::unique_ptr<Piece>>* playerPieceList = (board->turnToMove == 1) ? board->getWhitePieceList() : board->getBlackPieceList();
    std::vector<std::unique_ptr<Piece>>* opponentPieceList = (board->turnToMove == 1) ? board->getBlackPieceList() : board->getWhitePieceList();

    std::array<int, 64> playerAttackMap{};
    std::array<int, 64> enemyAttackMap{};

    std::array<int, 64> pinnedPlayerMap{};
    std::array<int, 64> pinnedEnemyMap{};

    // iterate over piece list and give the uniquePointer reference to generation method
    // generate all pseudo legal moves for player pieces
    for(auto& piecePtr : *playerPieceList){
        generateMovesForPiece(piecePtr, *playerLegalMoves, board, playerAttackMap, pinnedPlayerMap);
    }

    // generate all pseudo legal moves for enemy pieces for generation of legal moves
    // creates a map of squares that are attacked by the opponent
    for(auto& piecePtr : *opponentPieceList){
        generateMovesForPiece(piecePtr, *enemyLegalMoves, board, enemyAttackMap, pinnedEnemyMap);
    }

#ifdef DEBUG
    std::cout << "Mirrored Enemy Attack Map:" << std::endl;
    for (int i = 0; i < 64; ++i) {
        if(i % 8 == 0){
            std::cout << "\n";
        }
        std::cout << enemyAttackMap[i];
    }
    std::cout << std::endl;

    std::cout << "Pinned Pieces:" << std::endl;
    for (int i = 0; i < 64; ++i) {
        if(i % 8 == 0){
            std::cout << "\n";
        }
        std::cout << pinnedPlayerMap[i];
    }
    std::cout << std::endl;

#endif

    removeIllegalMoves(playerLegalMoves, enemyLegalMoves, playerAttackMap, enemyAttackMap);

#ifdef DEBUG
    std::cout << "Finished Move Generation\n Move List:" << std::endl;
    std::cout << playerLegalMoves->size() << std::endl;
    for (const auto& move : *playerLegalMoves) {
        std::cout << move << std::endl;
    }

#endif

    return playerLegalMoves;
}

void MoveGenerator::generateMovesForPiece
    (std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* board, std::array<int, 64>& attackMap, std::array<int, 64>& pinnedMap) {


    switch (piece->type) {
        case Piece::PAWN:
            break;
        case Piece::QUEEN:
            generateSlidingMoves(piece, moveList, board, attackMap);
            break;
        case Piece::KING:
            generateKingMoves(piece, moveList, board, attackMap, pinnedMap);
            break;
        case Piece::ROOK:
            generateSlidingMoves(piece, moveList, board, attackMap);
            break;
        case Piece::BISHOP:
            generateSlidingMoves(piece, moveList, board, attackMap);
            break;
        case Piece::KNIGHT:
            generateKnightMoves(piece, moveList, board, attackMap);
            break;
    }
}


void MoveGenerator::generateSlidingMoves
    (std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard, std::array<int, 64>& attackMap) {

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
            else if (boardRef[i] && boardRef[i].value()->color != piece->color && boardRef[i].value()->type == Piece::KING){
                // Edge Case for attack map creation, handle king as nonexistent, so a xray line can be formed through the king
                // necessary for removal of illegal moves, if king moves away from attacked pos even if it would be in the ray of the sliding
                // piece
                moveList.emplace_back(piece->pos, i, piece, boardRef[i]);
                attackMap[i] += 1;
            }
            else if (boardRef[i] && boardRef[i].value()->color != piece->color){
#ifdef DEBUG
                std::cout << "Found legal move" << std::endl;
#endif
                // Break if piece exists since a Rook cannot jump over other Pieces
                moveList.emplace_back(piece->pos, i, piece, boardRef[i]);
                attackMap[i] += 1;
                break;
            }
            else {
#ifdef DEBUG
                std::cout << "Found legal move" << std::endl;
#endif
                // Else i is empty Tile
                moveList.emplace_back(piece->pos, i, piece, std::optional<Piece*>{});
                attackMap[i] += 1;
            }
        }
    }
}

void MoveGenerator::generateKingMoves(std::unique_ptr<Piece>& piece, std::vector<Move>& moveList,
                                        const Board* playingBoard, std::array<int, 64>& attackMap,
                                        std::array<int, 64>& pinnedMap) {


    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;


    // Since King Direction is equal to Queen Movement, use Queen direction array
    for(const int& dir : this->PIECEDIRECTIONS[2]){


        int edgeTile = getEdgeTile(piece->pos, dir);
        bool isKingMove = true;
        int potentialPinnedPiecePos = -1;

        for (int i = piece->pos + dir; (dir > 0) ? (i <= edgeTile) : (i >= edgeTile); i+=dir) {

            // Add the move for the first iteration in a given direction after that only search for pinned pieces
            if (isKingMove){

                if(!boardRef[i]){
                    // Move exists and can be added to the Movelist, change bool to false to search for pinned pieces
                    moveList.emplace_back(piece->pos, i, piece, std::optional<Piece*>{});
                    attackMap[i] += 1;
                    isKingMove = false;
                }
                else if(boardRef[i] && boardRef[i].value()->color != piece->color){
                    moveList.emplace_back(piece->pos, i, piece, boardRef[i]);
                    attackMap[i] += 1;
                    isKingMove = false;
                }
                else if(boardRef[i] && boardRef[i].value()->color == piece->color){
                    // Edge Case, pinned piece is directly next to King, save its position
                    potentialPinnedPiecePos = i;
                    isKingMove = false;
                }
                else {
                    // No Move exists but we still continue searching for a pinned piece
                    isKingMove = false;
                }

            }
            else {
                if(boardRef[i] && boardRef[i].value()->color == piece->color){
                    // Save pinned piece position, but only if nothing has been found before
                    if(potentialPinnedPiecePos == -1){
                        potentialPinnedPiecePos = i;
                    }
                    else {
                        // Break from the loop since a second pinned piece has been found
                        break;
                    }

                }
                else if(boardRef[i] && boardRef[i].value()->color != piece->color){

                    if(potentialPinnedPiecePos != -1 && isAttackingSlidingPiece(boardRef[i].value(), dir)){
                        // if the direction ray hits a piece that could attack in direction ray then save pinned piece position on map
                        pinnedMap[potentialPinnedPiecePos] = dir;
                    }

                }
            }


        }



    }






}

void MoveGenerator::generateKnightMoves
    (std::unique_ptr<Piece>& piece, std::vector<Move>& moveList, const Board* playingBoard, std::array<int, 64>& attackMap) {

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
                    attackMap[gotoPos] += 1;
                }
                else{
                    if(boardRef[gotoPos].value()->color != piece->color){
#ifdef DEBUG
                        std::cout << "Found Move at " << gotoPos << std::endl;
#endif
                        moveList.emplace_back(piece->pos, gotoPos, piece, boardRef[gotoPos]);
                        attackMap[gotoPos] += 1;
                    }
                }
            }


        }
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

void MoveGenerator::removeIllegalMoves(std::shared_ptr<std::vector<Move>>& playerMoveList,
                                       std::shared_ptr<std::vector<Move>> &enemyMoveList,
                                       std::array<int, 64> &playerAttackMap, std::array<int, 64> &enemyAttackMap) {

    directKingChecks(playerMoveList, enemyMoveList, playerAttackMap, enemyAttackMap);

    pinnedPiecesChecks(playerMoveList, enemyMoveList, playerAttackMap, enemyAttackMap);


}

void MoveGenerator::directKingChecks(std::shared_ptr<std::vector<Move>> &playerMoveList,
                                     std::shared_ptr<std::vector<Move>> &enemyMoveList,
                                     std::array<int, 64> &playerAttackMap, std::array<int, 64> &enemyAttackMap) {



}

void MoveGenerator::pinnedPiecesChecks(std::shared_ptr<std::vector<Move>> &playerMoveList,
                                       std::shared_ptr<std::vector<Move>> &enemyMoveList,
                                       std::array<int, 64> &playerAttackMap, std::array<int, 64> &enemyAttackMap) {

}

bool MoveGenerator::isAttackingSlidingPiece(Piece* piece, int dir) {

    if(piece->type == Piece::ROOK && (dir == 8 || dir == -8 || dir == 1 || dir == -1)){
        return true;
    }
    else if(piece->type == Piece::BISHOP && (dir == 7 || dir == -7 || dir == 9 || dir == -9)){
        return true;
    }
    else if(piece->type == Piece::QUEEN && (dir == 7 || dir == -7 || dir == 9 || dir == -9 ||
                                            dir == 8 || dir == -8 || dir == 1 || dir == -1)) {
        return true;
    }
    else {
        return false;
    }
}


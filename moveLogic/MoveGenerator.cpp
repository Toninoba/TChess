//
// Created by Tobi on 07.02.2025.
//

#include "MoveGenerator.h"



std::vector<Move> MoveGenerator::generateLegalMoves(Board* board) {

    // Get reference to piece list of current player and King pieces
    auto& playerPieceList = board->getPlayerPieceList();
    auto& enemyPieceList = board->getEnemyPieceList();

    auto& playerKing = board->getPlayerKing();
    auto& enemyKing = board->getEnemyKing();

    // Create move list to return
    std::vector<Move> playerMoves;
    playerMoves.reserve(40);

    std::vector<Move> enemyMoves;
    enemyMoves.reserve(40);

    // Create Attack and Pinned Pieces Map

    std::array<int, 64> pinnedMapPlayer = {};
    std::array<int, 64> pinnedMapEnemy = {};

    std::array<int, 64> attackMapPlayer = {};
    std::array<int, 64> attackMapEnemy = {};

    // start by generating enemy King Moves
    generateKingMoves(enemyKing, enemyMoves, board, attackMapEnemy, pinnedMapEnemy);

    // then generate every other enemy Piece Moves
    for(auto& enemyPiece : enemyPieceList){
        // TODO implement move ordering while generation (insertion)
        if(enemyPiece->alive){
            generateMovesForPiece(enemyPiece, enemyMoves, board, attackMapEnemy, pinnedMapEnemy);
        }
    }



    for (int i = 0; i < 64; ++i) {
        if(i % 8 == 0){
            std::cout << "\n";
        }
        std::cout << " " << attackMapEnemy[i];
    }
    std::cout << std::endl;

    for (int i = 0; i < 64; ++i) {
        if(i % 8 == 0){
            std::cout << "\n";
        }
        std::cout << " " << pinnedMapEnemy[i];
    }
    std::cout << std::endl;





    return enemyMoves;
}



void MoveGenerator::generateMovesForPiece(std::unique_ptr<Piece>& piece,
                                          std::vector<Move>& moveList,
                                          const Board* board,
                                          std::array<int, 64>& attackMap,
                                          std::array<int, 64>& pinnedMap) {


    switch (piece->type) {
        case Piece::PAWN:
            generatePawnMoves(piece, moveList, board, attackMap, pinnedMap);
            break;
        case Piece::QUEEN:
            generateSlidingMoves(piece, moveList, board, attackMap, pinnedMap);
            break;
        case Piece::ROOK:
            generateSlidingMoves(piece, moveList, board, attackMap, pinnedMap);
            break;
        case Piece::BISHOP:
            generateSlidingMoves(piece, moveList, board, attackMap, pinnedMap);
            break;
        case Piece::KNIGHT:
            generateKnightMoves(piece, moveList, board, attackMap, pinnedMap);
            break;
    }
}


void MoveGenerator::generateSlidingMoves(std::unique_ptr<Piece>& piece,
                                         std::vector<Move>& moveList,
                                         const Board* playingBoard,
                                         std::array<int, 64>& attackMap,
                                         std::array<int, 64>& pinnedMap) {

    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;

    // init directions for pin check
    const std::vector<int>* directions = nullptr;
    std::vector<int> pinnedDirection;

    // if pin exists
    if(pinnedMap[piece->pos] != 0){
        // find direction of pin in piece directions and save it if it exists, else no direction is viable
        if(std::find(this->PIECEDIRECTIONS[piece->type].begin(),
                     this->PIECEDIRECTIONS[piece->type].end(),
                     pinnedMap[piece->pos]) != this->PIECEDIRECTIONS[piece->type].end()){

            pinnedDirection = { pinnedMap[piece->pos], -pinnedMap[piece->pos] };

        }
        directions = &pinnedDirection;
    }
    else {
        directions = &this->PIECEDIRECTIONS[piece->type];
    }



    // Since Rook = 0 in Piece enum, access directly via type is possible, same functions for other piece types
    for(const int& dir : *directions){

        int edgeTile = getEdgeTile(piece->pos, dir);

        // Iterate from Piece position + direction to edge tile to that direction
        for (int i = piece->pos + dir; (dir > 0) ? (i <= edgeTile) : (i >= edgeTile); i+=dir) {

            // Break if position in direction dir hits a Piece of same color
            if(boardRef[i] && boardRef[i].value()->color == piece->color){
                break;
            }
            else if (boardRef[i] && boardRef[i].value()->color != piece->color && boardRef[i].value()->type == Piece::KING){
                // Edge Case for attack map creation, handle king as nonexistent, so a xray line can be formed through the king
                // necessary for removal of illegal moves, if king moves away from attacked pos even if it would be in the ray of the sliding
                // piece
                moveList.emplace_back(piece->pos, i, piece.get(), boardRef[i]);
                attackMap[i] += 1;
            }
            else if (boardRef[i] && boardRef[i].value()->color != piece->color){
                // Break if piece exists since a Rook cannot jump over other Pieces
                moveList.emplace_back(piece->pos, i, piece.get(), boardRef[i]);
                attackMap[i] += 1;
                break;
            }
            else {
                // Else i is empty Tile
                moveList.emplace_back(piece->pos, i, piece.get(), std::optional<Piece*>{});
                attackMap[i] += 1;
            }
        }
    }
}

void MoveGenerator::generateKingMoves(Piece& piece,
                                      std::vector<Move>& moveList,
                                      const Board* playingBoard,
                                      std::array<int, 64>& attackMap,
                                      std::array<int, 64>& pinnedMap) {


    // Create reference to array board representation
    const auto& boardRef = playingBoard->board;


    // Since King Direction is equal to Queen Movement, use Queen direction array
    for(const int& dir : this->PIECEDIRECTIONS[2]){


        int edgeTile = getEdgeTile(piece.pos, dir);
        bool isKingMove = true;
        int potentialPinnedPiecePos = -1;

        for (int i = piece.pos + dir; (dir > 0) ? (i <= edgeTile) : (i >= edgeTile); i+=dir) {

            // Add the move for the first iteration in a given direction after that only search for pinned pieces
            if (isKingMove){

                if(!boardRef[i]){
                    // Move exists and can be added to the Movelist, change bool to false to search for pinned pieces
                    moveList.emplace_back(piece.pos, i, &piece, std::nullopt);
                    attackMap[i] += 1;
                    isKingMove = false;
                }
                else if(boardRef[i] && boardRef[i].value()->color != piece.color){
                    moveList.emplace_back(piece.pos, i, &piece, boardRef[i]);
                    attackMap[i] += 1;
                    isKingMove = false;
                }
                else if(boardRef[i] && boardRef[i].value()->color == piece.color){
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
                if(boardRef[i] && boardRef[i].value()->color == piece.color){
                    // Save pinned piece position, but only if nothing has been found before
                    if(potentialPinnedPiecePos == -1){
                        potentialPinnedPiecePos = i;
                    }
                    else {
                        // Break from the loop since a second pinned piece has been found
                        break;
                    }

                }
                else if(boardRef[i] && boardRef[i].value()->color != piece.color){

                    if(potentialPinnedPiecePos != -1 && isAttackingSlidingPiece(boardRef[i].value(), dir)){
                        // if the direction ray hits a piece that could attack in direction ray then save pinned piece position on map
                        pinnedMap[potentialPinnedPiecePos] = dir;
                    }
                }
            }
        }
    }
}

void MoveGenerator::generateKnightMoves(std::unique_ptr<Piece>& piece,
                                        std::vector<Move>& moveList,
                                        const Board* playingBoard,
                                        std::array<int, 64>& attackMap,
                                        std::array<int, 64>& pinnedMap) {


    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;


    // Since a Knight cannot move from a pin to capture Piece or move to different Position we can simply break from loop
    if(pinnedMap[piece->pos] != 0){
        return;
    }

    for(const int& dir : PIECEDIRECTIONS[3]){


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

                    moveList.emplace_back(piece->pos, gotoPos, piece.get(), std::optional<Piece*>{});
                    attackMap[gotoPos] += 1;
                }
                else{
                    if(boardRef[gotoPos].value()->color != piece->color){

                        moveList.emplace_back(piece->pos, gotoPos, piece.get(), boardRef[gotoPos]);
                        attackMap[gotoPos] += 1;
                    }
                }
            }
        }
    }
}

void MoveGenerator::generatePawnMoves(std::unique_ptr<Piece> &piece,
                                      std::vector<Move> &moveList,
                                      const Board *playingBoard,
                                      std::array<int, 64> &attackMap,
                                      std::array<int, 64> &pinnedMap) {


    // Create reference to array board representation
    const std::array<std::optional<Piece*>, 64>& boardRef = playingBoard->board;

    // init directions for pin check


    // if pin exists
    if(pinnedMap[piece->pos] != 0) {
        // if pinned direction ray does not equal pawn move direction(+8, -8) return
        if(abs(pinnedMap[piece->pos]) != 8){
            return;
        }
    }

    // check for pawn double move
    // (doesnt cover edge case when white pawn is on first row and then goes onto second row, cant exist in normal game)
    if(piece->color == 1 && piece->pos / 8 == 1 && !boardRef[piece->pos + 16]){
        moveList.emplace_back(piece->pos, piece->pos + 16, piece.get(), std::optional<Piece*>{});
    }
    else if(piece->color == -1 && piece->pos / 8 == 6 && !boardRef[piece->pos - 16]){
        moveList.emplace_back(piece->pos, piece->pos - 16, piece.get(), std::optional<Piece*>{});
    }

    // check for normal pawn move
    if(!boardRef[piece->pos + (8 * piece->color)]){
        // Check for Pawn conversions (white)
        if(piece->color == 1 && (piece->pos + 8) / 8 == 7){
            // iterate over enum to get all 4 types of pawn conversion
            for (int i = 0; i <= 3; ++i) {
                moveList.emplace_back(piece->pos, piece->pos + 8,
                                      piece.get(),
                                      std::optional<Piece*>{},
                                      false, false, true, Piece::Type(i));
            }
        }
        // Check for Pawn conversions (black)
        else if(piece->color == -1 && (piece->pos - 8) / 8 == 0){
            for (int i = 0; i <= 3; ++i) {
                moveList.emplace_back(piece->pos, piece->pos - 8,
                                      piece.get(),
                                      std::optional<Piece*>{},
                                      false, false, true, Piece::Type(i));
            }
        }
        // No Pawn conversion possible, normal move
        else {
            moveList.emplace_back(piece->pos, piece->pos + (8 * piece->color), piece.get(), std::optional<Piece*>{});
        }
    }

    // check for normal pawn attacks
    if(boardRef[piece->pos + (9 * piece->color)] &&
       boardRef[piece->pos + (9 * piece->color)].value()->color != piece->color){

        attackMap[piece->pos + (9 * piece->color)] += 1;
        moveList.emplace_back(piece->pos, piece->pos + (9 * piece->color),
                              piece.get(), boardRef[piece->pos + (9 * piece->color)]);

    }
    else if(boardRef[piece->pos + (7 * piece->color)] &&
            boardRef[piece->pos + (7 * piece->color)].value()->color != piece->color){

        attackMap[piece->pos + (7 * piece->color)] += 1;
        moveList.emplace_back(piece->pos, piece->pos + (7 * piece->color),
                              piece.get(), boardRef[piece->pos + (7 * piece->color)]);
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

void MoveGenerator::removeIllegalMoves(std::vector<Move>& playerMoveList,
                                       std::vector<Move>& enemyMoveList,
                                       std::array<int, 64> &playerAttackMap,
                                       std::array<int, 64> &enemyAttackMap) {

    directKingChecks(playerMoveList, enemyMoveList, playerAttackMap, enemyAttackMap);




}

void MoveGenerator::directKingChecks(std::vector<Move>& playerMoveList,
                                     std::vector<Move>& enemyMoveList,
                                     std::array<int, 64> &playerAttackMap,
                                     std::array<int, 64> &enemyAttackMap) {



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




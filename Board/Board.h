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
#include <memory>
#include <optional>
#include <algorithm>

/**
 * Board stores one Object of type array. This 64 piece long array stores pointers to Pieces (nullptr for emtpy tiles) and a pointer to
 * a Object of type Piece. This Piece will be created by a FEN and stored inside a vector corresponding to the color
 * The vector itself stores the object and not its reference and the board array uses a reference to the object inside the vector
 * !! Do not copy the board itself for move generation, only change it (TODO implement move, undo move and never let someone acces the board directly)
 * The FEN parser does not check for legality of FEN and could result in undefined behaviour so make sure to always use legit fen
 * I made the printBoard() method static to create more boards with different fens and only use one method to print each.
 *
 * The whiteKing and blackKing pointers are used for checking if either is checked and are initialized to the Piece inside the vector
 */

class Board {

public:

    std::array<std::optional<Piece*>, 64> board{};
    int turnToMove{};
    bool castlingWhiteK{};
    bool castlingWhiteQ{};
    bool castlingBlackK{};
    bool castlingBlackQ{};
    std::vector<Move> possibleEnPassant;
    int halfMoveClock{};
    int fullMoveClock{};


    // Constructor calls parseFen with the standard Chess setup
    Board();

    // Overload Constructor calls parseFen with the given FEN
    explicit Board(const std::string& fen);

    static void printBoard(const Board& b);

    void removePiece(int pos);

    std::vector<std::unique_ptr<Piece>>& getWhitePieceList(){
        return whitePieces;
    }

    std::vector<std::unique_ptr<Piece>>& getBlackPieceList(){
        return blackPieces;
    }

    Piece& getBlackKing(){
        return *blackKing;
    }

    Piece& getWhiteKing(){
        return *whiteKing;
    }

    Piece& getPlayerKing(){
        if(turnToMove == 1){
            return *whiteKing;
        }
        else{
            return *blackKing;
        }
    }

    Piece& getEnemyKing(){
        if(turnToMove == 1){
            return *blackKing;
        }
        else {
            return *whiteKing;
        }
    }

    std::vector<std::unique_ptr<Piece>>& getPlayerPieceList(){
        if(turnToMove == 1){
            return whitePieces;
        }
        else {
            return blackPieces;
        }
    }

    std::vector<std::unique_ptr<Piece>>& getEnemyPieceList(){
        if(turnToMove == 1){
            return blackPieces;
        }
        else {
            return whitePieces;
        }
    }

    void move(Move& move);

    void undoMove(Move& move);

private:

    Piece* whiteKing{};
    Piece* blackKing{};

    std::vector<std::unique_ptr<Piece>> whitePieces;
    std::vector<std::unique_ptr<Piece>> blackPieces;

    void parseFen(const std::string& fen);


};


#endif //TCHESS_BOARD_H

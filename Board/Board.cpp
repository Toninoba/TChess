//
// Created by Tobi on 25.01.2025.
//

#include <iostream>
#include "Board.h"
#include "../pieces/Piece.h"


Board::Board() {
    board = {std::nullopt};
    parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::cout << "Created Board Sucessfully\n" << std::endl;
}

Board::Board(const std::string &fen) {
    board = {std::nullopt};
    parseFen(fen);
    std::cout << "Created Board Sucessfully\n" << std::endl;
}

void Board::printBoard(const Board &b) {
    char rowChar = 'h';

    // To print the board in order that board[0] = "a1" we will use two loops that go top to bottom, left to right

    for (int i = 7; i >= 0; --i) {
        std::cout << rowChar--;
        for (int j = 0; j <= 7; ++j) {

            int pos = i * 8 + j;
            char pieceChar;

            if (b.board[pos]) {
                Piece &p = *b.board[pos].value();

                switch (p.type) {

                    case Piece::ROOK:
                        pieceChar = (p.color == -1) ? 'r' : 'R';
                        break;
                    case Piece::KNIGHT:
                        pieceChar = (p.color == -1) ? 'n' : 'N';
                        break;
                    case Piece::BISHOP:
                        pieceChar = (p.color == -1) ? 'b' : 'B';
                        break;
                    case Piece::QUEEN:
                        pieceChar = (p.color == -1) ? 'q' : 'Q';
                        break;
                    case Piece::KING:
                        pieceChar = (p.color == -1) ? 'k' : 'K';
                        break;
                    case Piece::PAWN:
                        pieceChar = (p.color == -1) ? 'p' : 'P';
                        break;
                }
            } else {
                pieceChar = '-';
            }
            std::cout << " " << pieceChar << " ";
        }
        std::cout << "\n";
    }
    std::cout << "  1  2  3  4  5  6  7  8" << std::endl;
}

/**
 * Parses a fen and initializes the board array, piece lists and other variables
 * @param fen the string to be parsed
 */
void Board::parseFen(const std::string &fen) {

    int parserPos = 0;
    int fieldPos = 56;

    // Position parsing
    while (fen[parserPos] != ' ' && parserPos < fen.size()) {
        char c = fen[parserPos];

        if (c >= '1' && c <= '8') {
            fieldPos += (c - '0');
        } else if (c == '/') {
            fieldPos -= 16;
        } else {
            switch (c) {
                case 'r':
                    blackPieces.emplace_back(std::make_unique<Piece>(5, Piece::ROOK, -1, fieldPos));
                    board[fieldPos] = blackPieces.back().get();
                    break;
                case 'R':
                    whitePieces.emplace_back(std::make_unique<Piece>(5, Piece::ROOK, 1, fieldPos));
                    board[fieldPos] = whitePieces.back().get();
                    break;
                case 'n':
                    blackPieces.emplace_back(std::make_unique<Piece>(3, Piece::KNIGHT, -1, fieldPos));
                    board[fieldPos] = blackPieces.back().get();
                    break;
                case 'N':
                    whitePieces.emplace_back(std::make_unique<Piece>(3, Piece::KNIGHT, 1, fieldPos));
                    board[fieldPos] = whitePieces.back().get();
                    break;
                case 'b':
                    blackPieces.emplace_back(std::make_unique<Piece>(3, Piece::BISHOP, -1, fieldPos));
                    board[fieldPos] = blackPieces.back().get();
                    break;
                case 'B':
                    whitePieces.emplace_back(std::make_unique<Piece>(3, Piece::BISHOP, 1, fieldPos));
                    board[fieldPos] = whitePieces.back().get();
                    break;
                case 'q':
                    blackPieces.emplace_back(std::make_unique<Piece>(9, Piece::QUEEN, -1, fieldPos));
                    board[fieldPos] = blackPieces.back().get();
                    break;
                case 'Q':
                    whitePieces.emplace_back(std::make_unique<Piece>(9, Piece::QUEEN, 1, fieldPos));
                    board[fieldPos] = whitePieces.back().get();
                    break;
                case 'k':
                    blackPieces.emplace_back(std::make_unique<Piece>(10, Piece::KING, -1, fieldPos));
                    board[fieldPos] = blackPieces.back().get();
                    this->blackKing = blackPieces.back().get();
                    break;
                case 'K':
                    whitePieces.emplace_back(std::make_unique<Piece>(10, Piece::KING, 1, fieldPos));
                    board[fieldPos] = whitePieces.back().get();
                    this->whiteKing = whitePieces.back().get();
                    break;
                case 'p':
                    blackPieces.emplace_back(std::make_unique<Piece>(1, Piece::PAWN, -1, fieldPos));
                    board[fieldPos] = blackPieces.back().get();
                    break;
                case 'P':
                    whitePieces.emplace_back(std::make_unique<Piece>(1, Piece::PAWN, 1, fieldPos));
                    board[fieldPos] = whitePieces.back().get();
                    break;
            }
            fieldPos++;
        }
        parserPos++;
    }
    parserPos++;

    // turnToMove parsing
    while (fen[parserPos] != ' ' && parserPos < fen.size()) {
        if (fen[parserPos] == 'w') this->turnToMove = 1;
        else this->turnToMove = -1;
        parserPos++;
    }
    parserPos++;

    // Castling Rights Parsing
    while (fen[parserPos] != ' ' && parserPos < fen.size()) {
        switch (fen[parserPos]) {
            case 'Q':
                this->castlingWhiteQ = true;
                break;
            case 'q':
                this->castlingBlackQ = true;
                break;
            case 'K':
                this->castlingWhiteK = true;
                break;
            case 'k':
                this->castlingBlackK = true;
                break;
            default:
                this->castlingBlackK = false;
                this->castlingWhiteK = false;
                this->castlingBlackQ = false;
                this->castlingWhiteQ = false;
                break;
        }
        parserPos++;
    }
    parserPos++;

    // possible En Passant Targets parsing
    while (fen[parserPos] != ' ' && parserPos < fen.size()) {
        // TODO implement
        parserPos++;
    }
    parserPos++;

    // Half Move Clock parsing
    std::string halfMoveClockString;
    while (fen[parserPos] != ' ' && parserPos < fen.size()) {
        halfMoveClockString.push_back(fen[parserPos]);
        parserPos++;
    }
    this->halfMoveClock = std::stoi(halfMoveClockString);

    parserPos++;

    // Full Move Clock Parsing
    std::string fullMoveClockString;
    while (fen[parserPos] != ' ' && parserPos < fen.size()) {
        fullMoveClockString.push_back(fen[parserPos]);
        parserPos++;
    }
    this->fullMoveClock = std::stoi(fullMoveClockString);
}

void Board::removePiece(int pos) {
    if(pos < 0 || pos > 63){
        return;
    }

    if(!board[pos]) return;

    std::optional<Piece*> toRemove = board[pos];
    // Delete Piece reference from board
    board[pos] = std::nullopt;

    std::vector<std::unique_ptr<Piece>>* toRemoveList = nullptr;
    // Decide which List the Piece should be removed from
    if(toRemove.value()->color == -1){
        toRemoveList = &blackPieces;
    }
    else {
        toRemoveList = &whitePieces;
    }

    // Remove Piece from List if it matches the pointer from the board array
    auto it = std::remove_if(toRemoveList->begin(), toRemoveList->end(),
                             [toRemove](const std::unique_ptr<Piece>& p) {
                                 return p.get() == toRemove;
                             });

    toRemoveList->erase(it, toRemoveList->end());
}


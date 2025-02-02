//
// Created by Tobi on 25.01.2025.
//

#include <iostream>
#include "Board.h"
#include "../pieces/Piece.h"


Board::Board(){
    std::cout << "Created Board Sucessfully\n" << std::endl;
}

void Board::printBoard(Board b) {
    char rowChar = 'h';

    // To print the board in order that board[0] = "a1" we will use two loops that go top to bottom, left to right

    for (int i = 7; i >= 0; --i) {
        std::cout << rowChar--;
        for (int j = 0; j <= 7; ++j) {

            int pos = i*8 + j;

            char pieceChar;

            Piece p = b.board[pos];

            switch (p.type) {

                case Piece::ROOK:
                    pieceChar = (p.color == 1) ? 'r' : 'R';
                    break;
                case Piece::KNIGHT:
                    pieceChar = (p.color == 1) ? 'n' : 'N';
                    break;
                case Piece::BISHOP:
                    pieceChar = (p.color == 1) ? 'b' : 'B';
                    break;
                case Piece::QUEEN:
                    pieceChar = (p.color == 1) ? 'q' : 'Q';
                    break;
                case Piece::KING:
                    pieceChar = (p.color == 1) ? 'k' : 'K';
                    break;
                case Piece::PAWN:
                    pieceChar = (p.color == 1) ? 'p' : 'P';
                    break;
                case Piece::EMPTY:
                    pieceChar = '-';
                    break;
            }

            std::cout << " " << pieceChar << " ";

        }
        std::cout << "\n";
    }
    std::cout << "  1  2  3  4  5  6  7  8" << std::endl;
}

Board::Board(std::string fen) {
    parseFen(fen);
}

void Board::parseFen(std::string fen) {
    int parserPos = 0;


    // Position parsing
    while(fen[parserPos] != ' ' && parserPos < fen.size()){
        std::cout << fen[parserPos];
        parserPos++;
    }

    std::cout << "|" << std::endl;
    parserPos++;

    // turnToMove parsing
    while(fen[parserPos] != ' ' && parserPos < fen.size()){
        if(fen[parserPos] == 'w') this->turnToMove = 1;
        else this->turnToMove = -1;
        parserPos++;
    }

    std::cout << "|" << std::endl;
    parserPos++;

    // Castling Rights Parsing
    while(fen[parserPos] != ' ' && parserPos < fen.size()){
        std::cout << fen[parserPos];
        switch (fen[parserPos]) {
            case 'Q':
                break;
            case 'q':
                break;
            case 'K':
                break;
            case 'k':
                break;
        }
        parserPos++;
    }

    std::cout << "|" << std::endl;
    parserPos++;

    // possible En Passant Targets parsing
    while(fen[parserPos] != ' ' && parserPos < fen.size()){
        std::cout << fen[parserPos];
        parserPos++;
    }

    std::cout << "|" << std::endl;
    parserPos++;

    // Half Move Clock parsing
    while(fen[parserPos] != ' ' && parserPos < fen.size()){
        std::cout << fen[parserPos];
        parserPos++;
    }

    std::cout << "|" << std::endl;
    parserPos++;

    // Full Move Clock Parsing
    while(fen[parserPos] != ' ' && parserPos < fen.size()){
        std::cout << fen[parserPos];
        parserPos++;
    }







}


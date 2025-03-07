//
// Created by Tobi on 02.02.2025.
//

#ifndef TCHESS_MOVE_H
#define TCHESS_MOVE_H


#include <optional>
#include <memory>
#include "../pieces/Piece.h"
/**
 * Class holds all information to perform a move and undo it later only by its given information
 */
class Move {

public:
    int from;
    int to;
    Piece* piece;
    std::optional<Piece*> pieceToBeat;

    bool isCastlingMove;
    bool isEnPassantMove;
    bool isPawnConversion;

    Piece::Type convertPawnTo;

    Move(int from, int to, Piece* piece, const std::optional<Piece*>& pieceToBeat) :
        from(from), to(to), piece(piece), pieceToBeat(pieceToBeat),
        isCastlingMove(false), isEnPassantMove(false),
        isPawnConversion(false), convertPawnTo(Piece::KING){};
        // convertPawnTo contains dummy variable, maybe fixing later

    Move(int from, int to, Piece* piece, const std::optional<Piece*>& pieceToBeat, bool isCastlingMove,
         bool isEnPassantMove, bool isPawnConversion, Piece::Type convertPawnTo) :
            from(from), to(to), piece(piece), pieceToBeat(pieceToBeat), isCastlingMove(isCastlingMove),
            isEnPassantMove(isEnPassantMove), isPawnConversion(isPawnConversion), convertPawnTo(convertPawnTo){};

    Move(const Move& m) :
            from(m.from), to(m.to), piece(m.piece), pieceToBeat(m.pieceToBeat),
            isCastlingMove(m.isCastlingMove), isEnPassantMove(m.isEnPassantMove),
            isPawnConversion(m.isPawnConversion), convertPawnTo(m.convertPawnTo)
            {
                std::cout << "Copied Move" << std::endl;
            }

    Move(Move&& m)  noexcept :
            from(m.from), to(m.to), piece(m.piece), pieceToBeat(m.pieceToBeat),
            isCastlingMove(m.isCastlingMove), isEnPassantMove(m.isEnPassantMove),
            isPawnConversion(m.isPawnConversion), convertPawnTo(m.convertPawnTo)
            {
                std::cout << "Moved Move" << std::endl;
            }

    friend std::ostream& operator<<(std::ostream& os, const Move& p) {

        if(p.pieceToBeat){
            os << "Move from: " << p.from << " to: " << p.to  << *p.piece << " beats:" << *p.pieceToBeat.value();
        }
        else {
            if(p.isPawnConversion){
                os << "Move from: " << p.from << " to: " << p.to << *p.piece << " converts to: " << p.convertPawnTo;
            }
            else {
                os << "Move from: " << p.from << " to: " << p.to << *p.piece;
            }
        }
        return os;
    }

private:



};


#endif //TCHESS_MOVE_H

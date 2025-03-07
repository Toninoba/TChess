//
// Created by Tobi on 25.12.2024.
//

#include "iostream"
#include "../RenderWindow/RenderWindow.h"
#include <SDL.h>
#include "../Board/Board.h"
#include "../moveLogic/MoveGenerator.h"
#include <windows.h>
#include <cstdlib>
#include "../moveLogic/Move.h"


int main(int argc, char* argv[]){
#ifdef _WIN32
    // Konsole aktivieren
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Bindet std::cout an die Konsole
    freopen("CONOUT$", "w", stderr);  // Bindet std::cerr an die Konsole
    freopen("CONIN$", "r", stdin);   // Bindet std::cin an die Konsole
#endif

    SDL_Init(SDL_INIT_EVERYTHING);

    std::string fen = "n7/q7/3p2p1/3pR3/p3pR2/pP2B2K/1k3P2/2N5 w - - 0 1";

    Board playingBoard = Board("k1p5/5p2/8/1p6/1P6/8/6p1/K7 w - - 0 1");
    MoveGenerator gen;
    Board::printBoard(playingBoard);

    auto moves = gen.generateLegalMoves(&playingBoard);

    for (auto& item : moves) {
        std::cout << item << std::endl;
    }
















    RenderWindow window("Test1", 800, 800);

    bool gameRunning = true;

    SDL_Event event;

    while (gameRunning){

        while (SDL_PollEvent(&event)){

            if(event.type == SDL_QUIT){
                gameRunning = false;
            }
        }

    }

    window.cleanUp();

    SDL_Quit();
    return 0;

}
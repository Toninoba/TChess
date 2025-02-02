//
// Created by Tobi on 25.12.2024.
//

#include "iostream"
#include "../RenderWindow/RenderWindow.h"
#include <SDL.h>
#include "../Board/Board.h"
#include <windows.h>


int main(int argc, char* argv[]){
#ifdef _WIN32
    // Konsole aktivieren
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Bindet std::cout an die Konsole
    freopen("CONOUT$", "w", stderr);  // Bindet std::cerr an die Konsole
    freopen("CONIN$", "r", stdin);   // Bindet std::cin an die Konsole
#endif

    SDL_Init(SDL_INIT_EVERYTHING);

    std::string fen = "4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 0 1";

    Board playingBoard = Board(fen);

    //playingBoard.printBoard(playingBoard);











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
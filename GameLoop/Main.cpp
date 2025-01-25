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

    Board playingBoard = Board();

    for (int i = 0; i < playingBoard.board.size(); ++i) {
        if(i % 10 == 0){
            std::cout << "" <<std::endl;
        }
        std::cout << playingBoard.board[i] << "  ";

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
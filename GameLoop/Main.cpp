//
// Created by Tobi on 25.12.2024.
//

#include "iostream"
#include "../RenderWindow/RenderWindow.h"
#include <SDL.h>


int main(int argc, char* argv[]){

    SDL_Init(SDL_INIT_EVERYTHING);

    RenderWindow window("Test1", 1280, 720);

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
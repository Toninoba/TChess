//
// Created by Tobi on 25.12.2024.
//

#include "RenderWindow.h"
#include <SDL.h>
#include <SDL_test_images.h>
#include <iostream>

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    :window(NULL), renderer(NULL)
{

    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL){
        std::cout << "Window failed to init. Error" << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void RenderWindow::cleanUp() {

    SDL_DestroyWindow(window);

}



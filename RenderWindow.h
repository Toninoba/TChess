//
// Created by Tobi on 25.12.2024.
//
#pragma once
#ifndef TCHESS_RENDERWINDOW_H
#define TCHESS_RENDERWINDOW_H
#include <SDL.h>
#include <SDL_test_images.h>


class RenderWindow {
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    void cleanUp();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

};


#endif //TCHESS_RENDERWINDOW_H

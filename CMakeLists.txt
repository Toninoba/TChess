cmake_minimum_required(VERSION 3.28)
project(TChess)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/SDL2-2.30.8/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})


add_executable(TChess
        GameLoop/Main.cpp
        RenderWindow/RenderWindow.cpp
        Board/Board.cpp
        pieces/Piece.h
        pieces/Piece.cpp
        moveLogic/Move.h
        moveLogic/Move.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

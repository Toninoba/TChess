//
// Created by Tobi on 25.01.2025.
//

#include <iostream>
#include "Board.h"


Board::Board() {
    std::cout << "Created Board Sucessfully" << std::endl;

    for (int i = 0; i < 120; ++i) {

        if(i <= 20 || i >= 100){
            board[i] = -1;
        }
        else if(i % 10 == 0 || i % 10 == 9){
            board[i] = -1;
        }
        else {
            board[i] = 0;
        }

    }
}

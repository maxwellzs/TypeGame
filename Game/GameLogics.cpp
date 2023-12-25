//
// Created by maxwellzs on 2023/12/21.
//

#include "GameLogics.h"

TypeGame::GameTick TypeGame::TypeGameLogics::generateTick(clock_t tickTime) {
    size_t x,pressed = 27;
    x = getCharIndex(current);
    char distance = velocity * tickTime / tickTimePerFrame;
    counter += distance;
    if(counter >= threadHold) {
        // missed
        miss++;
        counter = 0; // reset height
        current = generateChar(); // random another char
    }

    Qt::Key last = getLastPressedKey();
    if(last >= Qt::Key_A && last <= Qt::Key_Z) {
        pressed = last - Qt::Key_A;
        if(last == current) {
            // hit
            score ++;
            counter = 0;
            current = generateChar();
        }
    }

    return GameTick{current, x, counter, score, miss, pressed};
}

size_t TypeGame::TypeGameLogics::getCharIndex(char c) {
    return c - 'A';
}

char TypeGame::TypeGameLogics::generateChar() {
    std::srand(time(nullptr));
    int val = std::rand();
    return 'A' + (val % ('Z' - 'A'));
}

//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMELOGICS_H
#define TYPEGAME_GAMELOGICS_H

#include "Game/GameEngine.h"
#include <random>

namespace TypeGame {

    class TypeGameLogics : public GameLogics {
    private:
        size_t getCharIndex(char c);

        char generateChar();

        char current = generateChar();
        size_t counter = 0;     // height counter
        size_t score = 0;
        size_t miss = 0;

        const size_t charSize = 600 / 26 + 1;
        const size_t boxHeight = 600;
        const size_t boxWidth = 400;

        const size_t threadHold = boxHeight - charSize;

        size_t velocity = 4;
        const size_t tickTimePerFrame = TICK(60);
    public:
        GameTick generateTick(clock_t tickTime) override;

    };

};

#endif //TYPEGAME_GAMELOGICS_H

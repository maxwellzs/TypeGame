//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMETICK_H
#define TYPEGAME_GAMETICK_H

#include <cstddef>
#include <string>

namespace TypeGame {

    class GameTick {
    private:
        char movingChar;
        size_t posX;
        size_t posY;
        size_t score;
        size_t miss;
        size_t pressed;
    public:
        /**
         * create a game tick object
         * @param moving the char that is falling down
         * @param posX the position x the falling char
         * @param posY the position y of the falling char
         * @param score the score of the player
         * @param miss miss times
         */
        GameTick(char moving, size_t posX, size_t posY, size_t score, size_t miss, size_t pressed);

        /**
         * create an empty tick
         */
        GameTick();

        char getMovingChar() const;

        size_t getPosX() const;

        size_t getPosY() const;

        size_t getScore() const;

        size_t getMiss() const;

        size_t getPressed() const;

        std::string toString();

    };

};

#endif //TYPEGAME_GAMETICK_H

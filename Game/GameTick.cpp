//
// Created by maxwellzs on 2023/12/21.
//

#include "GameTick.h"

TypeGame::GameTick::GameTick(char moving, size_t posX, size_t posY, size_t score, size_t miss, size_t pressed)
        : movingChar(moving)
, posX(posX), posY(posY), score(score), miss(miss), pressed(pressed){

}

char TypeGame::GameTick::getMovingChar() const {
    return movingChar;
}

size_t TypeGame::GameTick::getPosX() const {
    return posX;
}

size_t TypeGame::GameTick::getPosY() const {
    return posY;
}

size_t TypeGame::GameTick::getScore() const {
    return score;
}

size_t TypeGame::GameTick::getMiss() const {
    return miss;
}

TypeGame::GameTick::GameTick() {
    this->movingChar = '?';
    posX = posY = score = miss = 0;
    pressed = 27;
}

size_t TypeGame::GameTick::getPressed() const {
    return pressed;
}

std::string TypeGame::GameTick::toString() {
    return "[x=" + std::to_string(posX) + ",y=" + std::to_string(posY) + ",char=" + std::string().assign(1,movingChar) +  "]";
}

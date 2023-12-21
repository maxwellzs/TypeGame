//
// Created by maxwellzs on 2023/12/21.
//

#include "GameTickPacket.h"

std::shared_ptr<VNet::Packet> TypeGame::GameTickPacket::unMarshal(std::shared_ptr<VNet::DataBuffer> inBuf, sockaddr_in sender) {
    std::shared_ptr<GameTickPacket> tick = std::make_shared<GameTickPacket>();
    uint8_t packId = inBuf->getUint8();
    if(!packId == getPacketID()) return nullptr;
    uint16_t x,y,press,score,miss;
    uint8_t moving;
    x = inBuf->getUint16();
    y = inBuf->getUint16();
    press = inBuf->getUint16();
    score = inBuf->getUint16();
    miss = inBuf->getUint16();
    moving = inBuf->getUint8();
    tick->setGameTick(GameTick{static_cast<char>(moving),x,y,score,miss,press});
    return tick;
}

void TypeGame::GameTickPacket::marshal(std::shared_ptr<VNet::DataBuffer> outBuf) {
    Packet::marshal(outBuf);
    uint16_t x = tick.getPosX(),y = tick.getPosY(),
    pressed = tick.getPressed(),score = tick.getScore(),miss = tick.getMiss();
    uint8_t moving = tick.getMovingChar();
    outBuf->putUint16(x);
    outBuf->putUint16(y);
    outBuf->putUint16(pressed);
    outBuf->putUint16(score);
    outBuf->putUint16(miss);
    outBuf->putUint8(moving);
}

void TypeGame::GameTickPacket::setGameTick(TypeGame::GameTick tick) {
    this->tick = tick;
}

TypeGame::GameTick TypeGame::GameTickPacket::getGameTick() {
    return tick;
}

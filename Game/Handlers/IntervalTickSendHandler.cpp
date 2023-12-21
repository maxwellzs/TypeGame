//
// Created by maxwellzs on 2023/12/21.
//

#include "IntervalTickSendHandler.h"
#include "Game/GameEngine.h"
#include "Game/Packets/GameTickPacket.h"
#include "Game/Packets/GameCreatePacket.h"

void TypeGame::IntervalTickSendHandler::onInterval(VNet::PacketController &parent) {
    if(!sentGameCreate) {
        // send first pack
        auto createGame = std::make_shared<GameCreatePacket>();
        createGame->setPort(port);
        parent.broadcastPacket(createGame);
        sentGameCreate = true;
        return;
    }
    parent.broadcastPacket(wrapTick(),2);
}

void TypeGame::IntervalTickSendHandler::registerTick(TypeGame::GameTick t) {
    pending = t;
}

TypeGame::IntervalTickSendHandler::IntervalTickSendHandler(uint16_t port) : port(port),VNet::IntervalEventHandler(TICK(24)){

}

std::shared_ptr<VNet::Packet> TypeGame::IntervalTickSendHandler::wrapTick() {
    std::shared_ptr<GameTickPacket> packet = std::make_shared<GameTickPacket>();
    packet->setGameTick(pending);
    return packet;
}

void TypeGame::IntervalTickSendHandler::registerCreateGame() {
    this->sentGameCreate = false;
}

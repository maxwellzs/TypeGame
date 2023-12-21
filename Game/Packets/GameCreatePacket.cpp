//
// Created by maxwellzs on 2023/12/21.
//

#include "GameCreatePacket.h"

std::shared_ptr<VNet::Packet> TypeGame::GameCreatePacket::unMarshal(std::shared_ptr<VNet::DataBuffer> inBuf, sockaddr_in sender) {
    uint8_t packet_id = inBuf->getUint8();
    if(packet_id != this->getPacketID()) return nullptr;
    auto newPacket = std::make_shared<GameCreatePacket>();
    newPacket->port = inBuf->getUint16();
    return newPacket;
}

void TypeGame::GameCreatePacket::marshal(std::shared_ptr<VNet::DataBuffer> outBuf) {
    VNet::Packet::marshal(outBuf);
    outBuf->putUint16(port);
}

void TypeGame::GameCreatePacket::setPort(uint16_t port) {
    this->port = port;
}

uint16_t TypeGame::GameCreatePacket::getPort() {
    return port;
}

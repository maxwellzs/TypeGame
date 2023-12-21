//
// Created by maxwellzs on 2023/12/21.
//

#include "GameCreateListener.h"
#include "Game/Packets/GameCreatePacket.h"
#include "Game/GameEngine.h"

TypeGame::GameCreatePacketListener::GameCreatePacketListener(TypeGame::GameEngine &parent) : parent(parent) {

}

void TypeGame::GameCreatePacketListener::onPacketReceiveCallback(VNet::PacketController &parent,
                                                                 std::shared_ptr<VNet::Packet> packet, sockaddr_in from) {
    if(packet.get() && packet->getPacketID() == 0x4) {
        std::cout << "received create game packet" << std::endl;
        if(this->parent.getEngineState() != ESTABLISHED) {
            auto pack = std::static_pointer_cast<GameCreatePacket>(packet);
            sockaddr_in addr = {0};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(pack->getPort());
            addr.sin_addr = from.sin_addr;
            // other player try to connect in
            this->parent.startAndChangeWindow(addr);
        }
    }
}

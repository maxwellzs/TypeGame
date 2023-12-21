//
// Created by maxwellzs on 2023/12/21.
//

#include "GameTickListener.h"
#include "Game/Packets/GameTickPacket.h"
#include "Game/GameEngine.h"

void TypeGame::GameTickListener::onPacketReceiveCallback(VNet::PacketController &parent, std::shared_ptr<VNet::Packet> packet,
                                                         sockaddr_in from) {
    if(packet.get() && packet->getPacketID() == 0x5) {
        auto tickPack = std::static_pointer_cast<GameTickPacket>(packet);
        this->parent.setRemoteTick(tickPack->getGameTick());
    }
}

TypeGame::GameTickListener::GameTickListener(TypeGame::GameEngine &parent) : parent(parent){

}

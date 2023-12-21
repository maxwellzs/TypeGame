//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMETICKPACKET_H
#define TYPEGAME_GAMETICKPACKET_H

#include "Controller/PacketController.h"
#include "Game/GameTick.h"

namespace TypeGame {

    class GameTickPacket : public VNet::Packet {
    private:
        GameTick tick;
    public:
        PACKET_ID_DES(GameTickPacket,0x5,"[Game Tick]")

        std::shared_ptr<Packet> unMarshal(std::shared_ptr<VNet::DataBuffer> inBuf, sockaddr_in sender) override;

        void marshal(std::shared_ptr<VNet::DataBuffer> outBuf) override;

        void setGameTick(GameTick tick);

        GameTick getGameTick();
    };

    REG_PACKET_PROTOTYPE_NAMESPACE(TypeGame,GameTickPacket);

};

#endif //TYPEGAME_GAMETICKPACKET_H

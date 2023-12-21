//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMECREATEPACKET_H
#define TYPEGAME_GAMECREATEPACKET_H

#include "Packet/PacketBuilder.h"
#include "Packet/Packet.h"
#include "Utils/DataBuffer.h"

namespace TypeGame {

    class GameCreatePacket : public VNet::Packet {
    private:
        uint16_t port;
    public:
        void setPort(uint16_t port);
        uint16_t getPort();

        PACKET_ID_DES(GameCreatePacket,0x4,"[Game On Create Packet]");

        std::shared_ptr<Packet> unMarshal(std::shared_ptr<VNet::DataBuffer> inBuf, sockaddr_in sender) override;

        void marshal(std::shared_ptr<VNet::DataBuffer> outBuf) override;
    };

    REG_PACKET_PROTOTYPE_NAMESPACE(TypeGame,GameCreatePacket);

};

#endif //TYPEGAME_GAMECREATEPACKET_H

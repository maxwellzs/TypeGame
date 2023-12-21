//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMECREATELISTENER_H
#define TYPEGAME_GAMECREATELISTENER_H

#include "Controller/PacketController.h"


namespace TypeGame {

    class GameEngine;

    class GameCreatePacketListener : public VNet::EventHandler {
    private:
        GameEngine& parent;
    public:
        GameCreatePacketListener(GameEngine& parent);

        void onPacketReceiveCallback(VNet::PacketController &parent, std::shared_ptr<VNet::Packet> packet,
                                     sockaddr_in from) override;

    };

};

#endif //TYPEGAME_GAMECREATELISTENER_H

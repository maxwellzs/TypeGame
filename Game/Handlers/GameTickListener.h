//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMETICKLISTENER_H
#define TYPEGAME_GAMETICKLISTENER_H

#include "Controller/PacketController.h"

namespace TypeGame {

    class GameEngine;

    class GameTickListener : public VNet::EventHandler {
    private:
        GameEngine& parent;
    public:
        GameTickListener(GameEngine& parent);

        void onPacketReceiveCallback(VNet::PacketController &parent, std::shared_ptr<VNet::Packet> packet,
                                     sockaddr_in from) override;
    };

};

#endif //TYPEGAME_GAMETICKLISTENER_H

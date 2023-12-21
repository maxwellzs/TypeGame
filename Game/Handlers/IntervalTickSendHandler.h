//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_INTERVALTICKSENDHANDLER_H
#define TYPEGAME_INTERVALTICKSENDHANDLER_H

#include "Controller/PacketController.h"
#include "Game/GameTick.h"

namespace TypeGame {

    class IntervalTickSendHandler : public VNet::IntervalEventHandler {
    private:
        GameTick pending;
        /**
         * parse the tick to network packet
         * @return the pointer
         */
        std::shared_ptr<VNet::Packet> wrapTick();
        uint16_t port; // the port will be declared in the game create packet
        bool sentGameCreate = true; // this handler will send hello on first loop
    public:
        /**
         * attach a tick to this sender
         * @param t the tick data
         */
        void registerTick(GameTick t);
        void registerCreateGame();
        IntervalTickSendHandler(uint16_t port);
        void onInterval(VNet::PacketController &parent) override;
    };

};

#endif //TYPEGAME_INTERVALTICKSENDHANDLER_H

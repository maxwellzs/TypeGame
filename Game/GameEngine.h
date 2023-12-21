//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMEENGINE_H
#define TYPEGAME_GAMEENGINE_H

#include <QObject>
#include <QThread>
#include <QtWidgets>
#include <QKeyEvent>
#include <thread>
#include <cstdint>
#include <ctime>
#include <atomic>
#include "Game/GameTick.h"
#include "Controller/UDPController.h"
#include "Game/Handlers/GameCreateListener.h"
#include "Game/Handlers/GameTickListener.h"
#include "Game/Handlers/IntervalTickSendHandler.h"
#include "UI/GameWindow.h"


#define TICK(x) (1000/x)

namespace TypeGame {

    const uint16_t DEFAULT_GAME_PORT = 9999;
    const clock_t TIME_OUT_VAL = 3000; // 3 seconds

    enum ENGINE_STATE {
        IDLE,           // the engine has no tasks
        LOCAL,        // the engine is waiting for network
        ESTABLISHED     // the game has started
    };

    class GameEngine;

    /**
     * this class is called by the game engine
     * put all the game logics in here
     */
    class GameLogics {
    protected:
        Qt::Key lastPressedKey = Qt::Key_Escape;
        Qt::Key getLastPressedKey();
    public:
        /**
         * the game engine will inform you of the user input via this funciton
         * @param k the last pressed key
         */
        void setLastPressedKey(Qt::Key k);
        /**
         * generate a tick
         * @param tickTime the milliseconds between two ticks
         * @return the generated tick
         */
        virtual GameTick generateTick(clock_t tickTime) = 0;
    };

    class GameState {
    private:
        double frameRate = 0.0;
        double frameRecv = 0.0;
    public:
        GameState(double frameRate,double frameRecv);

        double getFrameRate() const;

        double getFrameRecv() const;
    };

    class GameEngine : public QObject{
        Q_OBJECT
    private:
        clock_t tickInterval;   // how much millisecond between two ticks
        clock_t lastTick;       // the last executed game tick
        std::shared_ptr<GameLogics> tickGenerator;

        std::shared_ptr<std::thread> ioWorker;
        std::shared_ptr<std::thread> engineWorker;
        std::shared_ptr<VNet::EventHandler> onCreateListener;
        std::shared_ptr<VNet::EventHandler> onTickReceiveListener;
        std::shared_ptr<VNet::IntervalEventHandler> sendInterval;

        std::shared_ptr<VNet::UDPController> controller;
        uint16_t controllerPort;

        bool stopped = false;   // signal this engine to stop
        ENGINE_STATE state = IDLE;

        clock_t remoteLastTickTime; // the last time the remote client sent packet
        GameTick remoteLastTick;
        size_t remoteCounter = 0;

        void engineLoop();


    public:
    signals: // use those signals to contact the front page
        // the game tick output by the local engine
        void localGameTick(GameTick tick);
        void remoteGameTick(GameTick tick);
        // this signal is sent when there are errors that are not recoverable
        void engineQuits();
        void engineInit(sockaddr_in addr);
        // monitor this engine
        void engineState(GameState s);

    public slots:
        void onPlayerKeyPressed(int key);
    public:
        /**
         * this function happens when another player connect in
         */
        void startAndChangeWindow(sockaddr_in addr);
        /**
         * attach a tick to this engine, and the tick time out will be refreshed at the same time
         * @param tick the income tick
         */
        void setRemoteTick(GameTick tick);
        /**
         * create a game engine with the tick rate
         * @param tickInterval milliseconds between two ticks, use macro TICK(x)
         * @param port the port the inner udp controller will be running
         */
        GameEngine(clock_t tickInterval = TICK(60),uint16_t port = DEFAULT_GAME_PORT);

        void setEngineState(ENGINE_STATE newState);

        /**
         * detach the VNet Daemon and engine Daemon
         */
        void detachEngine();
        /**
         * inform the game to start
         */
        void startEngine();

        /**
         * try to connect to another client
         * @param target the target client
         */
        void connect(sockaddr_in target);

        uint16_t getEnginePort();

        /**
         * get the current state of the engine that is idle,occupied or pending for a key packet
         * @return the state
         */
        ENGINE_STATE getEngineState();

        void setGameLogicsHandler(std::shared_ptr<GameLogics> generator);
    };

};

#endif //TYPEGAME_GAMEENGINE_H

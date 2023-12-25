//
// Created by maxwellzs on 2023/12/21.
//

#include "GameEngine.h"

TypeGame::GameEngine::GameEngine(clock_t tickInterval,uint16_t port) : tickInterval(tickInterval), controllerPort(port) {
    controller = std::make_shared<VNet::UDPController>(controllerPort);
}

void TypeGame::GameEngine::detachEngine() {
    // initialize the listeners
    onCreateListener = std::make_shared <GameCreatePacketListener>(*this);
    onTickReceiveListener = std::make_shared <GameTickListener>(*this);
    sendInterval = std::make_shared <IntervalTickSendHandler>(controllerPort);
    // deploy controller
    ioWorker = std::make_shared<std::thread>([this] {
        // register handlers
        controller->addHandler(onCreateListener);
        controller->addHandler(onTickReceiveListener);
        controller->addInterval(sendInterval);
        controller->runController();
    });
    engineWorker = std::make_shared<std::thread>(std::thread(&GameEngine::engineLoop,this));
    ioWorker->detach();
    engineWorker->detach();
}

void TypeGame::GameEngine::engineLoop() {
    std::cout << "game engine logic thread begin" << std::endl;

    double frameInterval;
    double frameRate = 0.0;
    double frameRecv;

    while(!stopped) {
        clock_t now,passed;
        //idle
        switch (state) {
            case IDLE:
                // yield time slice
                std::this_thread::sleep_for(std::chrono::milliseconds (1));
                break;
            case LOCAL:
                now = clock();
                passed = now - lastTick;
                if(passed > tickInterval) {
                    // reached next tick
                    GameTick t = (tickGenerator.get() ? tickGenerator->generateTick(passed) : GameTick());
                    emit localGameTick(t);
                    // send tick to remote
                    // std::static_pointer_cast<IntervalTickSendHandler>(sendInterval)->registerTick(t);
                    lastTick = now;
                }

                localFrameCounter += passed;

                if(localFrameCounter > 500) {
                    localFrameCounter += passed;
                    // calculate game data
                    frameInterval = passed;
                    frameRate = 1000/frameInterval;
                    // calculate every 1s
                    localFrameCounter = 0;
                    emit engineState(GameState{frameRate,0.0});
                }

                std::this_thread::sleep_for(std::chrono::milliseconds (1));
                break;
            case ESTABLISHED:

                now = clock();
                passed = now - lastTick;
                if(passed > tickInterval) {
                    // reached next tick
                    GameTick t = (tickGenerator.get() ? tickGenerator->generateTick(passed) : GameTick());
                    emit localGameTick(t);
                    // send tick to remote
                    std::static_pointer_cast<IntervalTickSendHandler>(sendInterval)->registerTick(t);
                    lastTick = now;
                }
                localFrameCounter += passed;

                emit remoteGameTick(remoteLastTick);
                if(now - remoteLastTickTime >= TIME_OUT_VAL) {
                    emit engineQuits();
                    return;
                }
                // calculate game data
                frameInterval = passed;
                frameRate = 1000/frameInterval;
                frameRecv = lastReceiveRate;
                if(localFrameCounter > 500) {
                    // calculate every 1s
                    frameRecv = remoteCounter * 2.0;
                    lastReceiveRate = frameRecv;
                    remoteCounter = 0; // clear remote counter
                    localFrameCounter = 0;
                }
                emit engineState(GameState{frameRate,frameRecv});

                std::this_thread::sleep_for(std::chrono::milliseconds (1));
                break;
        }
    }
}

void TypeGame::GameEngine::startEngine() {
    lastTick = clock();
    setEngineState(ESTABLISHED);
}

uint16_t TypeGame::GameEngine::getEnginePort() {
    return controllerPort;
}

TypeGame::ENGINE_STATE TypeGame::GameEngine::getEngineState() {
    return state;
}

void TypeGame::GameEngine::setGameLogicsHandler(std::shared_ptr<GameLogics> generator) {
    this->tickGenerator = generator;
}

void TypeGame::GameEngine::onPlayerKeyPressed(int key) {
    this->tickGenerator->setLastPressedKey(static_cast<Qt::Key>(key));
}

void TypeGame::GameEngine::setRemoteTick(TypeGame::GameTick tick) {
    this->remoteLastTick = tick;
    this->remoteLastTickTime = clock();
    remoteCounter ++;
}

void TypeGame::GameEngine::connect(sockaddr_in target) {
    controller->connect(target);
    std::static_pointer_cast<IntervalTickSendHandler>(sendInterval)->registerCreateGame();
    remoteLastTickTime = clock();
}

void TypeGame::GameEngine::startAndChangeWindow(sockaddr_in addr) {
    emit engineInit(addr);
}

void TypeGame::GameEngine::setEngineState(TypeGame::ENGINE_STATE newState) {
    switch (newState) {
        case IDLE:
            std::cout << "set state -> IDLE" << std::endl;
            break;
        case LOCAL:
            std::cout << "set state -> LOCAL" << std::endl;
            break;
        case ESTABLISHED:
            std::cout << "set state -> ESTABLISHED" << std::endl;
            break;
    }
    this->state = newState;
}

void TypeGame::GameLogics::setLastPressedKey(Qt::Key k) {
    this->lastPressedKey = k;
}

Qt::Key TypeGame::GameLogics::getLastPressedKey() {
    Qt::Key key = lastPressedKey;
    // clear last pressed
    lastPressedKey = Qt::Key::Key_Escape;
    return key;
}

TypeGame::GameState::GameState(double frameRate, double frameRecv) :frameRate(frameRate),frameRecv(frameRecv){

}

double TypeGame::GameState::getFrameRate() const {
    return frameRate;
}

double TypeGame::GameState::getFrameRecv() const {
    return frameRecv;
}

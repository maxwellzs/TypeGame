//
// Created by maxwellzs on 2023/12/21.
//

#include <iostream>
#include "MainWindow.h"

TypeGame::TypeGameMainWindow::TypeGameMainWindow(uint16_t port) : engine(TICK(60),port){
    setFixedSize(700,800);
    setWindowTitle("Type Game");
    initMainMenu();
    QObject::connect(this,&TypeGameMainWindow::changeWindow,this,&TypeGameMainWindow::onWindowChange);
    QObject::connect(&engine, &GameEngine::engineInit,this,&TypeGameMainWindow::onWindowChange);
    engine.setGameLogicsHandler(std::make_shared <TypeGameLogics>());
    engine.detachEngine();
}

TypeGame::TypeGameMainWindow::~TypeGameMainWindow() {
    delete confirmButton;
    delete input;
    delete singlePlayerButton;
}

void TypeGame::TypeGameMainWindow::initMainMenu() {
    confirmButton = new ConfirmIPButton(this);
    input = new IPInputBox(this);
    // button on click events
    QObject::connect(confirmButton,&ConfirmIPButton::clicked,this,&TypeGameMainWindow::onConfirmButtonClicked);
    // add title
    mainTitle = new QLabel(this);
    mainTitle->setText("加入一个对局或让对手加入你");
    mainTitle->setFont(QFont("黑体",20));
    mainTitle->setAlignment(Qt::AlignHCenter);  // align all text to center
    size_t titleWidth = size().width();
    size_t titleHeight = 50;
    mainTitle->setGeometry(0,50,titleWidth,titleHeight);

    singlePlayerButton = new QPushButton(this);
    size_t height = size().height();
    size_t width = size().width();
    singlePlayerButton->setGeometry(width/2 - 50,height/2 + 90,100,70);
    singlePlayerButton->setText("单人游戏");
    QObject::connect(singlePlayerButton,&QPushButton::clicked,this,&TypeGameMainWindow::onEnterSinglePlayer);
}

void TypeGame::TypeGameMainWindow::onConfirmButtonClicked() {
    std::string msg = input->text().toStdString();
    size_t pos = msg.find(':');
    if(pos == std::string::npos) {
        informInvalidAddressInput();
        return;
    }
    std::string ipString,portString;
    ipString = msg.substr(0,pos);
    portString = msg.substr(pos+1,msg.length()-pos-1);
    std::cout << "ip : " << ipString << " port : " << portString << std::endl;
    sockaddr_in inAddress;
    uint32_t inIp = inet_addr(ipString.c_str());
    if(inIp == INADDR_NONE) {
        informInvalidAddressInput();
        return;
    }
    try {
        uint16_t targetPort = std::atoi(portString.c_str());
        inAddress.sin_port = htons(targetPort);
    } catch (std::exception& e) {
        informInvalidAddressInput();
        return;
    }
    inAddress.sin_family = AF_INET;
    inAddress.sin_addr.S_un.S_addr = inIp;

    onWindowChange(inAddress);
}

void TypeGame::TypeGameMainWindow::informInvalidAddressInput() {
    QMessageBox box;
    box.setWindowTitle("警告");
    box.setText("输入的地址无效\r\n"
                "输入示例: 10.49.0.12:9999");
    box.setButtonText(QMessageBox::Ok,"确定");
    box.exec();
}

void TypeGame::TypeGameMainWindow::bindQuitSignal(QApplication *app) {
    QObject::connect(&engine,&GameEngine::engineQuits,this,&TypeGameMainWindow::onGameEngineQuit);
    QObject::connect(this,&TypeGameMainWindow::applicationQuit,app,&QApplication::quit);
}

void TypeGame::TypeGameMainWindow::onGameEngineQuit() {
    QMessageBox box;
    box.setWindowTitle("警告");
    box.setText("你的对手离开了游戏");
    box.setButtonText(QMessageBox::Ok,"确定");
    box.exec();
    emit applicationQuit();
}

void TypeGame::TypeGameMainWindow::onWindowChange(sockaddr_in target) {
    close();
    gameFrame = new GameWindow();
    gameFrame->show();

    // bind the game engine with front page
    QObject::connect(&engine,&GameEngine::localGameTick,gameFrame,&GameWindow::onLocalGameTick);
    QObject::connect(&engine,&GameEngine::remoteGameTick,gameFrame,&GameWindow::onRemoteGameTick);
    QObject::connect(&engine,&GameEngine::engineState,gameFrame,&GameWindow::onReceiveStatics);

    engine.connect(target);
    QObject::connect(gameFrame,&GameWindow::keyPressed,&engine,&GameEngine::onPlayerKeyPressed);
    engine.startEngine();
}

void TypeGame::TypeGameMainWindow::onEnterSinglePlayer() {
    close();
    single = new SinglePlayerWindow();
    single->show();
    QObject::connect(&engine,&GameEngine::localGameTick,single,&SinglePlayerWindow::onReceiveFrame);
    QObject::connect(&engine,&GameEngine::engineState,single,&SinglePlayerWindow::onReceiveStatics);
    QObject::connect(single,&SinglePlayerWindow::onPlayerKeyPressed,&engine,&GameEngine::onPlayerKeyPressed);
    engine.setEngineState(LOCAL);
}


TypeGame::ConfirmIPButton::ConfirmIPButton(QWidget *parent) : QPushButton(parent) {
    size_t parentWidth,parentHeight;
    parentWidth = parent->size().width();
    parentHeight = parent->size().height();
    size_t buttonWidth,buttonHeight;
    buttonHeight = 70;
    buttonWidth = 100;
    setGeometry(parentWidth/2 - buttonWidth/2,
                (parentHeight)/2,
                buttonWidth,buttonHeight);
    setText("连接");
}

TypeGame::IPInputBox::IPInputBox(QWidget *parent) : QLineEdit(parent) {
    size_t parentWidth,parentHeight;
    parentWidth = parent->size().width();
    parentHeight = parent->size().height();
    size_t lineWidth = parentWidth/3;
    size_t lineHeight = 50;
    setGeometry(parentWidth/2 - lineWidth/2,
                (parentHeight)/2 - lineHeight * 2,
                lineWidth,lineHeight);
    setPlaceholderText("输入对手的内网IP");
}

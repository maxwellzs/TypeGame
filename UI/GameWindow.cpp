//
// Created by maxwellzs on 2023/12/21.
//

#include "GameWindow.h"

TypeGame::GameWindow::GameWindow() {
    setUpFrame();
}

void TypeGame::GameWindow::onLocalGameTick(TypeGame::GameTick tick) {
    local->setGameTick(tick);
    local->update();
    std::string msg = "你的得分:\r\n" +
            std::to_string(tick.getScore()) +
            "\r\n你的失误\r\n" +
            std::to_string(tick.getMiss());
   scoreBoardLocal->setText(QString::fromStdString(msg));
}

void TypeGame::GameWindow::onRemoteGameTick(TypeGame::GameTick tick) {
    remote->setGameTick(tick);
    remote->update();
    std::string msg = "对手的得分:\r\n" +
                      std::to_string(tick.getScore()) +
                      "\r\n对手的失误\r\n" +
                      std::to_string(tick.getMiss());
    scoreBoardRemote->setText(QString::fromStdString(msg));
}

void TypeGame::GameWindow::setUpFrame() {
    setFixedSize(1200,1200);
    local = new CharBox(this);
    remote = new CharBox(this);
    size_t boxHeight = 600;
    size_t boxWidth = 400;
    local->setGeometry(100,300,boxWidth,boxHeight);
    remote->setGeometry(600,300,boxWidth,boxHeight);
    local->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;background-color: white;");
    remote->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;background-color: white;");

    scoreBoardLocal = new QLabel(this);
    scoreBoardRemote = new QLabel(this);

    scoreBoardLocal->setGeometry(100,100,200,100);
    scoreBoardRemote->setGeometry(600,100,200,100);
    scoreBoardLocal->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;color: black;background-color: white;");
    scoreBoardRemote->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;color: black;background-color: white;");
    scoreBoardLocal->setFont(QFont("黑体",12));
    scoreBoardRemote->setFont(QFont("黑体",12));

    gameState = new QLabel(this);
    gameState->setGeometry(300,1000,500,100);
    gameState->setFont(QFont("黑体",15));
    gameState->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;color: black;background-color: white;");

}

TypeGame::GameWindow::~GameWindow() {
    delete scoreBoardLocal;
    delete scoreBoardRemote;
    delete local;
    delete remote;
    delete gameState;
}

void TypeGame::GameWindow::keyPressEvent(QKeyEvent *event) {
    int k = event->key();
    emit keyPressed(k);
    QWidget::keyPressEvent(event);
}

void TypeGame::GameWindow::onReceiveStatics(TypeGame::GameState state) {
    const static size_t PKG_SIZE = META_DATA_SIZE + 1 + sizeof(uint16_t) * 5 + 1;
    std::ostringstream v1,v2,v3;
    v1 << std::fixed << std::setprecision(2) << state.getFrameRate();
    v2 << std::fixed << std::setprecision(2) << state.getFrameRecv();
    v3 << std::fixed << std::setprecision(2) << PKG_SIZE * state.getFrameRecv();
    std::string msg = "帧率=" + v1.str() +
            " 收包速度=" + v2.str() +
            " pkt/s 传输速度=" + v3.str() +" bytes/s";
    gameState->setText(QString::fromStdString(msg));
}

void TypeGame::CharBox::paintEvent(QPaintEvent *event) {
    // static resources
    static QString baseline = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static size_t charSize = size().width()/26;
    static QFont font("Consolas",charSize);

    Q_UNUSED(event)
    QPainter painter(this);
    // clear canvas
    painter.eraseRect(rect());
    painter.setPen(QPen(Qt::black));
    painter.setFont(font);
    size_t height = size().height();
    painter.drawText(currentTick.getPosX() * charSize,currentTick.getPosY(),QString(currentTick.getMovingChar()));
    // draw bottom
    for(size_t i = 0;i < baseline.length();i ++) {
        painter.drawText(i * charSize,height-charSize,QString(baseline.at(i)));
    }
    // draw line
    if(currentTick.getPressed() < 27) {
        size_t linePos = currentTick.getPressed() * charSize + charSize / 2;
        painter.setPen(QPen(Qt::red));
        painter.drawLine(linePos,0,linePos,height);
    }
}

void TypeGame::CharBox::setGameTick(TypeGame::GameTick tick) {
    this->currentTick = tick;
}

TypeGame::CharBox::CharBox(QWidget *parent) : QWidget(parent){

}

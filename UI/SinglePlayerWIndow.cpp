//
// Created by maxwellzs on 2023/12/21.
//

#include "SinglePlayerWIndow.h"

TypeGame::SinglePlayerWindow::SinglePlayerWindow() {
    setFixedSize(800,1000);
    box = new CharBox(this);
    box->setGeometry(200,300,400,600);
    box->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;color: black;background-color: white;");
    state = new QLabel(this);
    scoreBoard = new QLabel(this);
    scoreBoard->setGeometry(200,100,400,100);
    scoreBoard->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;color: black;background-color: white;");
    scoreBoard->setFont(QFont("黑体",12));
    state->setGeometry(200,900,400,100);
    state->setStyleSheet("border-width: 3px;border-style: solid;border-color: black;color: black;background-color: white;");
    state->setFont(QFont("黑体",12));
}

void TypeGame::SinglePlayerWindow::keyPressEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    int key = event->key();
    emit onPlayerKeyPressed(key);
    QWidget::keyPressEvent(event);
}

void TypeGame::SinglePlayerWindow::onReceiveStatics(TypeGame::GameState s) {
    std::ostringstream v;
    v << std::fixed << std::setprecision(2) << s.getFrameRate();
    std::string msg = "帧率=" + v.str() + "fps";
    state->setText(QString::fromStdString(msg));
}

void TypeGame::SinglePlayerWindow::onReceiveFrame(TypeGame::GameTick t) {
    box->setGameTick(t);
    box->update();
    std::string msg = "你的得分:\r\n" +
                      std::to_string(t.getScore()) +
                      "\r\n你的失误\r\n" +
                      std::to_string(t.getMiss());
    scoreBoard->setText(QString::fromStdString(msg));
}

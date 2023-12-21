//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_GAMEWINDOW_H
#define TYPEGAME_GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPainter>
#include <QKeyEvent>
#include "Game/GameTick.h"
#include "Game/GameEngine.h"

namespace TypeGame {

    class GameState;

    class CharBox : public QWidget {
    private:
        GameTick currentTick;
    protected:
        void paintEvent(QPaintEvent *event) override;

    public:
        void setGameTick(GameTick tick);

        CharBox(QWidget *parent);
    };

    class GameWindow : public QMainWindow {
        Q_OBJECT
    private:
        CharBox *local;
        CharBox *remote;
        QLabel *scoreBoardLocal;
        QLabel *scoreBoardRemote;
        QLabel *gameState;

        void setUpFrame();

        void paintBaseChar(QLabel *label);

    protected:
        // capture the keyboard event
        void keyPressEvent(QKeyEvent *event) override;

    public:
    signals:
        void keyPressed(int key);

    public slots:

        void onLocalGameTick(GameTick tick);

        void onRemoteGameTick(GameTick tick);

        void onReceiveStatics(GameState state);

    public:
        ~GameWindow();

        GameWindow();

    };

};

#endif //TYPEGAME_GAMEWINDOW_H

//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_SINGLEPLAYERWINDOW_H
#define TYPEGAME_SINGLEPLAYERWINDOW_H

#include "UI/GameWindow.h"

namespace TypeGame {

    class SinglePlayerWindow : public QMainWindow {
        Q_OBJECT;
    private:
        CharBox *box = nullptr;
        QLabel *state = nullptr;
        QLabel *scoreBoard = nullptr;
    protected:
        void keyPressEvent(QKeyEvent *event) override;

    public slots:

        // collect the game engine state
        void onReceiveStatics(GameState s);
        // collect the game frame and render
        void onReceiveFrame(GameTick t);

    public:
    signals:
        void onPlayerKeyPressed(int key);

    public:
        SinglePlayerWindow();
    };

}

#endif //TYPEGAME_SINGLEPLAYERWINDOW_H

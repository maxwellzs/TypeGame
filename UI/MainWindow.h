//
// Created by maxwellzs on 2023/12/21.
//

#ifndef TYPEGAME_MAINWINDOW_H
#define TYPEGAME_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <winsock2.h>
#include <QMessageBox>
#include <QLabel>
#include "UI/GameWindow.h"
#include "Game/GameEngine.h"
#include "Game/GameLogics.h"
#include "UI/SinglePlayerWIndow.h"

namespace TypeGame {

    class ConfirmIPButton : public QPushButton {
    Q_OBJECT

    public:
        ConfirmIPButton(QWidget *parent);
    };


    class IPInputBox : public QLineEdit {
    Q_OBJECT

    public:
        IPInputBox(QWidget *parent);
    };

    class TypeGameMainWindow : public QMainWindow {
    Q_OBJECT

    private:
        ConfirmIPButton *confirmButton = nullptr;
        QPushButton *singlePlayerButton = nullptr;
        IPInputBox *input = nullptr;
        QLabel *mainTitle = nullptr;
        GameWindow *gameFrame = nullptr;
        SinglePlayerWindow *single = nullptr;
        GameEngine engine;

        void initMainMenu();

        void informInvalidAddressInput();

    public:
    signals:
        void applicationQuit();

        void changeWindow(sockaddr_in target);
    public slots:

        void onWindowChange(sockaddr_in target);

        void onConfirmButtonClicked();

        void onGameEngineQuit();

        void onEnterSinglePlayer();

    public:
        void bindQuitSignal(QApplication *app);

        ~TypeGameMainWindow();

        TypeGameMainWindow(uint16_t port = 9999);

    };

};

#endif //TYPEGAME_MAINWINDOW_H

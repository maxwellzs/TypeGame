#include <QCoreApplication>
#include <QDebug>
#include <QApplication>
#include "UI/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc,argv);
    using namespace TypeGame;

    if(argc < 2) throw std::invalid_argument("no port provided");

    // create main window
    TypeGameMainWindow window(std::stoi(argv[1]));
    window.show();
    window.bindQuitSignal(&app);

    return app.exec();
}

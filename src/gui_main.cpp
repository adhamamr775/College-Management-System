#include <QApplication>
#include "../include/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("College Management System");

    MainWindow window;
    window.show();

    return app.exec();
}

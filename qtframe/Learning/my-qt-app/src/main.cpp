#include <QApplication>
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Week 1 - Hello Qt");
    window.resize(400, 200);
    window.show();

    return app.exec();
}

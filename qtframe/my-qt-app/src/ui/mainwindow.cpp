#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) 
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    auto *label = new QLabel("<h2>Hello, Qt (Week 1)</h2>");
    layout->addWidget(label);

    setCentralWidget(central);
}

#include "mainwindow.h"

#include "map_widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new map_widget);
}

MainWindow::~MainWindow()
{
}


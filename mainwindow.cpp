#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ANTButton_clicked()
{
    myANTWidget = new ANTWidget();
    myANTWidget->show();
}

void MainWindow::on_BLEButton_clicked()
{
    myBLEWidget = new BLEWidget();
    myBLEWidget->show();

}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

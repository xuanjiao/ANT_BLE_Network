#ifndef BLEWIDGET_H
#define BLEWIDGET_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QtSerialPort>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
namespace Ui {
class BLEWidget;
}

class BLEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BLEWidget(QWidget *parent = nullptr);
    ~BLEWidget();
    QSerialPort* port;
    QFile* file;
    QTextStream* out;
private slots:
    void on_AvailablePortButtom_clicked();

    void on_ConnectPortButtom_clicked();

    void getSerialMessage();

    void on_NextBotton_clicked();

    void on_QuitButton_clicked();

    void on_DisconnectButton_clicked();

    void serialPortError(QSerialPort::SerialPortError error);
private:
    Ui::BLEWidget *ui;
};

#endif // BLEWIDGET_H

#ifndef ANTWIDGET_H
#define ANTWIDGET_H

#include <QWidget>

#include <QMainWindow>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <QStandardItemModel>
#include <QListView>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>

#include "definition.h"
#include "device.h"
namespace Ui {
class ANTWidget;
}

class ANTWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ANTWidget(QWidget *parent = nullptr);
    ~ANTWidget();
    void printDeviceInfo();
    Device *myDevice = NULL;
    QStandardItemModel* myDeviceModel;
    QStandardItemModel* beaconTableModel;

    QFile* file = NULL;
    QTextStream *out = NULL;
    QMap<QString,int> beaconMap;
    int MaxMeasurementNum;
public slots:
    void displayText(const QString &);
    void handleBeaconInfo(int, QString, int);
    void handleBeaconDisconnect(int);

private slots:
    void on_QuitButton_clicked();

    void on_SendButton_clicked();

    void on_DisconnectButton_clicked();

    void on_NewDevice_clicked();

    void handleGoToSearch(int);
    void handleNewBeacon(int);
signals:
    void recv_new_msg();
    void send_new_msg(QString msg);
    void new_text(QString);


private:
    Ui::ANTWidget *ui;
};

#endif // ANTWIDGET_H

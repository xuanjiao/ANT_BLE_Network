#ifndef DEVICE_H
#define DEVICE_H

#include "types.h"
#include "dsi_framer_ant.hpp"
#include "dsi_thread.h"
#include "dsi_serial_generic.hpp"
#include "dsi_debug.hpp"

#include "definition.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <QDebug>


class Device:public QObject
{
    Q_OBJECT
public:

    UCHAR ucUSBDeviceNum;
    USHORT usDevicePID;
    USHORT usDeviceVID;
    UCHAR aucDeviceDescription[USB_MAX_STRLEN];
    UCHAR aucDeviceSerial[USB_MAX_STRLEN];
    UCHAR ucDeviceNum;
    UCHAR ucDeviceType;
    UCHAR ucTransType;
    UCHAR ucRF;

    BOOL isMultiChannel = false;
    BOOL isConnect = FALSE;
    BOOL connectDevice();
    void storeConfig(UCHAR ucChType, UCHAR ucUSBDeviceNum, UCHAR ucDeviceNum, UCHAR ucDeviceType, UCHAR ucTransType,UCHAR ucRF);
    BOOL configure();
    void disconnect();

    Device();
    ~Device();

    BOOL Init(UCHAR ucDeviceNumber_, UCHAR ucChannelType_);
    void Start();
    void Close();

 private:
    BOOL InitResetANT();

    //Starts the Message thread.
    static DSI_THREAD_RETURN RunMessageThread(void *pvParameter_);

    //Listens for a response from the module
    void MessageThread();
    //Decodes the received message
    void ProcessMessage(ANT_MESSAGE stMessage, USHORT usSize_);

    // print user menu
    void PrintMenu();

    BOOL bBursting; //holds whether the bursting phase of the test has started
    BOOL bBroadcasting;
    BOOL bMyDone;
    BOOL bDone;
    UCHAR ucChannelType;
    DSISerialGeneric* pclSerialObject = NULL;
    DSIFramerANT* pclMessageObject = NULL;
    DSI_THREAD_ID uiDSIThread;
    DSI_CONDITION_VAR condTestDone;
    DSI_MUTEX mutexTestDone;
    BOOL bDisplay;
    UCHAR aucTransmitBuffer[ANT_STANDARD_DATA_PAYLOAD_SIZE];

public slots:
    void sendAMessageAndStoreInBuffer(QString qstr);
signals:
    void new_text(QString qstr);
    void beaconInfo(int,QString,int);
    void goToSearch(int);
    void newBeacon(int);
};


#endif // DEVICE_H

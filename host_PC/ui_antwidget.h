/********************************************************************************
** Form generated from reading UI file 'antwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANTWIDGET_H
#define UI_ANTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ANTWidget
{
public:
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QPushButton *SendButton;
    QLabel *MessageLabel;
    QLineEdit *MessageEdit;
    QLabel *USBDeviceNumberLabel;
    QPushButton *DisconnectButton;
    QGroupBox *groupBox;
    QLineEdit *RFEdit;
    QLabel *HzLabel;
    QLabel *DeviceTypeLabel;
    QLabel *TransmissionTypeLabel;
    QLabel *DeviceNumberLabel;
    QLabel *ChannelTypeLabel;
    QLabel *RFLabel;
    QComboBox *ChannelTypeComboBox;
    QLineEdit *DeviceNumberEdit;
    QLineEdit *TransmissionTypeEdit;
    QLineEdit *DeviceTypeEdit;
    QPushButton *NewDevice;
    QLabel *label;
    QLineEdit *USBDeviceNumberEdit;
    QPushButton *QuitButton;
    QTextBrowser *textBrowser;
    QGroupBox *groupBox_4;
    QLineEdit *BaseFileNameEdit;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *FileDirectoryLabel;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_7;
    QLineEdit *StartDistanceEdit;
    QLabel *IncrementLabel;
    QLineEdit *IncrementEdits;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_8;
    QVBoxLayout *verticalLayout;
    QLineEdit *NumOfMeasurementsEdit;
    QLineEdit *CountEdit;
    QGroupBox *groupBox_3;
    QListView *listView;
    QTableView *tableView;

    void setupUi(QWidget *ANTWidget)
    {
        if (ANTWidget->objectName().isEmpty())
            ANTWidget->setObjectName(QStringLiteral("ANTWidget"));
        ANTWidget->resize(1033, 565);
        label_2 = new QLabel(ANTWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(390, 320, 101, 16));
        groupBox_2 = new QGroupBox(ANTWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(360, 50, 251, 131));
        SendButton = new QPushButton(groupBox_2);
        SendButton->setObjectName(QStringLiteral("SendButton"));
        SendButton->setGeometry(QRect(80, 90, 101, 32));
        MessageLabel = new QLabel(groupBox_2);
        MessageLabel->setObjectName(QStringLiteral("MessageLabel"));
        MessageLabel->setGeometry(QRect(70, 20, 111, 41));
        MessageEdit = new QLineEdit(groupBox_2);
        MessageEdit->setObjectName(QStringLiteral("MessageEdit"));
        MessageEdit->setGeometry(QRect(40, 50, 191, 31));
        MessageEdit->setInputMask(QStringLiteral("00-00-00-00-00-00-00-00"));
        MessageLabel->raise();
        SendButton->raise();
        MessageEdit->raise();
        USBDeviceNumberLabel = new QLabel(ANTWidget);
        USBDeviceNumberLabel->setObjectName(QStringLiteral("USBDeviceNumberLabel"));
        USBDeviceNumberLabel->setGeometry(QRect(70, 300, 55, 16));
        DisconnectButton = new QPushButton(ANTWidget);
        DisconnectButton->setObjectName(QStringLiteral("DisconnectButton"));
        DisconnectButton->setGeometry(QRect(490, 190, 121, 51));
        groupBox = new QGroupBox(ANTWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 50, 301, 201));
        RFEdit = new QLineEdit(groupBox);
        RFEdit->setObjectName(QStringLiteral("RFEdit"));
        RFEdit->setGeometry(QRect(230, 70, 21, 21));
        HzLabel = new QLabel(groupBox);
        HzLabel->setObjectName(QStringLiteral("HzLabel"));
        HzLabel->setGeometry(QRect(153, 72, 125, 16));
        DeviceTypeLabel = new QLabel(groupBox);
        DeviceTypeLabel->setObjectName(QStringLiteral("DeviceTypeLabel"));
        DeviceTypeLabel->setGeometry(QRect(21, 132, 76, 16));
        TransmissionTypeLabel = new QLabel(groupBox);
        TransmissionTypeLabel->setObjectName(QStringLiteral("TransmissionTypeLabel"));
        TransmissionTypeLabel->setGeometry(QRect(21, 162, 115, 16));
        DeviceNumberLabel = new QLabel(groupBox);
        DeviceNumberLabel->setObjectName(QStringLiteral("DeviceNumberLabel"));
        DeviceNumberLabel->setGeometry(QRect(21, 101, 94, 16));
        ChannelTypeLabel = new QLabel(groupBox);
        ChannelTypeLabel->setObjectName(QStringLiteral("ChannelTypeLabel"));
        ChannelTypeLabel->setGeometry(QRect(21, 41, 84, 16));
        RFLabel = new QLabel(groupBox);
        RFLabel->setObjectName(QStringLiteral("RFLabel"));
        RFLabel->setGeometry(QRect(21, 71, 102, 16));
        ChannelTypeComboBox = new QComboBox(groupBox);
        ChannelTypeComboBox->addItem(QString());
        ChannelTypeComboBox->addItem(QString());
        ChannelTypeComboBox->addItem(QString());
        ChannelTypeComboBox->setObjectName(QStringLiteral("ChannelTypeComboBox"));
        ChannelTypeComboBox->setGeometry(QRect(150, 40, 131, 26));
        ChannelTypeComboBox->setFocusPolicy(Qt::TabFocus);
        DeviceNumberEdit = new QLineEdit(groupBox);
        DeviceNumberEdit->setObjectName(QStringLiteral("DeviceNumberEdit"));
        DeviceNumberEdit->setGeometry(QRect(153, 97, 125, 21));
        TransmissionTypeEdit = new QLineEdit(groupBox);
        TransmissionTypeEdit->setObjectName(QStringLiteral("TransmissionTypeEdit"));
        TransmissionTypeEdit->setGeometry(QRect(153, 159, 125, 21));
        DeviceTypeEdit = new QLineEdit(groupBox);
        DeviceTypeEdit->setObjectName(QStringLiteral("DeviceTypeEdit"));
        DeviceTypeEdit->setGeometry(QRect(153, 128, 125, 21));
        DeviceTypeLabel->raise();
        TransmissionTypeLabel->raise();
        DeviceNumberLabel->raise();
        ChannelTypeLabel->raise();
        RFLabel->raise();
        ChannelTypeComboBox->raise();
        DeviceNumberEdit->raise();
        TransmissionTypeEdit->raise();
        DeviceTypeEdit->raise();
        HzLabel->raise();
        RFEdit->raise();
        NewDevice = new QPushButton(ANTWidget);
        NewDevice->setObjectName(QStringLiteral("NewDevice"));
        NewDevice->setGeometry(QRect(360, 190, 121, 51));
        label = new QLabel(ANTWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 300, 151, 16));
        USBDeviceNumberEdit = new QLineEdit(ANTWidget);
        USBDeviceNumberEdit->setObjectName(QStringLiteral("USBDeviceNumberEdit"));
        USBDeviceNumberEdit->setGeometry(QRect(150, 300, 125, 21));
        QuitButton = new QPushButton(ANTWidget);
        QuitButton->setObjectName(QStringLiteral("QuitButton"));
        QuitButton->setGeometry(QRect(870, 490, 68, 32));
        textBrowser = new QTextBrowser(ANTWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(690, 260, 261, 211));
        groupBox_4 = new QGroupBox(ANTWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(360, 260, 271, 211));
        BaseFileNameEdit = new QLineEdit(groupBox_4);
        BaseFileNameEdit->setObjectName(QStringLiteral("BaseFileNameEdit"));
        BaseFileNameEdit->setGeometry(QRect(132, 60, 91, 21));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 90, 91, 16));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 30, 91, 16));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(230, 60, 60, 16));
        FileDirectoryLabel = new QLabel(groupBox_4);
        FileDirectoryLabel->setObjectName(QStringLiteral("FileDirectoryLabel"));
        FileDirectoryLabel->setGeometry(QRect(130, 20, 141, 31));
        FileDirectoryLabel->setWordWrap(true);
        layoutWidget = new QWidget(groupBox_4);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 110, 174, 23));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout->addWidget(label_7);

        StartDistanceEdit = new QLineEdit(layoutWidget);
        StartDistanceEdit->setObjectName(QStringLiteral("StartDistanceEdit"));
        StartDistanceEdit->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(StartDistanceEdit);

        IncrementLabel = new QLabel(layoutWidget);
        IncrementLabel->setObjectName(QStringLiteral("IncrementLabel"));

        horizontalLayout->addWidget(IncrementLabel);

        IncrementEdits = new QLineEdit(layoutWidget);
        IncrementEdits->setObjectName(QStringLiteral("IncrementEdits"));
        IncrementEdits->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(IncrementEdits);

        layoutWidget_2 = new QWidget(groupBox_4);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(30, 140, 211, 56));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_4 = new QLabel(layoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_2->addWidget(label_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_8 = new QLabel(layoutWidget_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_8);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        NumOfMeasurementsEdit = new QLineEdit(layoutWidget_2);
        NumOfMeasurementsEdit->setObjectName(QStringLiteral("NumOfMeasurementsEdit"));
        NumOfMeasurementsEdit->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(NumOfMeasurementsEdit);

        CountEdit = new QLineEdit(layoutWidget_2);
        CountEdit->setObjectName(QStringLiteral("CountEdit"));
        CountEdit->setAlignment(Qt::AlignCenter);
        CountEdit->setReadOnly(true);

        verticalLayout->addWidget(CountEdit);


        horizontalLayout_3->addLayout(verticalLayout);

        groupBox_3 = new QGroupBox(ANTWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(30, 260, 291, 211));
        listView = new QListView(groupBox_3);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(30, 90, 241, 91));
        tableView = new QTableView(ANTWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(680, 30, 271, 211));
        tableView->horizontalHeader()->setCascadingSectionResizes(false);

        retranslateUi(ANTWidget);

        QMetaObject::connectSlotsByName(ANTWidget);
    } // setupUi

    void retranslateUi(QWidget *ANTWidget)
    {
        ANTWidget->setWindowTitle(QApplication::translate("ANTWidget", "Form", nullptr));
        label_2->setText(QApplication::translate("ANTWidget", "Base file name", nullptr));
        groupBox_2->setTitle(QApplication::translate("ANTWidget", "Message", nullptr));
        SendButton->setText(QApplication::translate("ANTWidget", "Send", nullptr));
        MessageLabel->setText(QApplication::translate("ANTWidget", "Message to send", nullptr));
        MessageEdit->setText(QApplication::translate("ANTWidget", "00-00-00-00-00-00-00-00", nullptr));
        USBDeviceNumberLabel->setText(QApplication::translate("ANTWidget", "USB port", nullptr));
        DisconnectButton->setText(QApplication::translate("ANTWidget", "Disconnect", nullptr));
        groupBox->setTitle(QApplication::translate("ANTWidget", "ANT Configuration", nullptr));
        RFEdit->setText(QApplication::translate("ANTWidget", "66", nullptr));
        HzLabel->setText(QApplication::translate("ANTWidget", "2400Mhz+         Mhz", nullptr));
        DeviceTypeLabel->setText(QApplication::translate("ANTWidget", "Device Type", nullptr));
        TransmissionTypeLabel->setText(QApplication::translate("ANTWidget", "Transmission Type", nullptr));
        DeviceNumberLabel->setText(QApplication::translate("ANTWidget", "Device Number", nullptr));
        ChannelTypeLabel->setText(QApplication::translate("ANTWidget", "Channel Type", nullptr));
        RFLabel->setText(QApplication::translate("ANTWidget", "Radio Frequency", nullptr));
        ChannelTypeComboBox->setItemText(0, QApplication::translate("ANTWidget", "Slave (channel 0 only)", nullptr));
        ChannelTypeComboBox->setItemText(1, QApplication::translate("ANTWidget", "Slave Multi Channel", nullptr));
        ChannelTypeComboBox->setItemText(2, QApplication::translate("ANTWidget", "Master", nullptr));

        ChannelTypeComboBox->setCurrentText(QApplication::translate("ANTWidget", "Slave (channel 0 only)", nullptr));
        DeviceNumberEdit->setText(QApplication::translate("ANTWidget", "0", nullptr));
        TransmissionTypeEdit->setText(QApplication::translate("ANTWidget", "0", nullptr));
        DeviceTypeEdit->setText(QApplication::translate("ANTWidget", "0", nullptr));
        NewDevice->setText(QApplication::translate("ANTWidget", "Connect", nullptr));
        label->setText(QString());
        USBDeviceNumberEdit->setText(QApplication::translate("ANTWidget", "0", nullptr));
        QuitButton->setText(QApplication::translate("ANTWidget", "Quit", nullptr));
        groupBox_4->setTitle(QApplication::translate("ANTWidget", "File", nullptr));
        BaseFileNameEdit->setText(QApplication::translate("ANTWidget", "myResult", nullptr));
        label_3->setText(QApplication::translate("ANTWidget", "Distance", nullptr));
        label_6->setText(QApplication::translate("ANTWidget", "File directory", nullptr));
        label_5->setText(QApplication::translate("ANTWidget", ".txt", nullptr));
        FileDirectoryLabel->setText(QString());
        label_7->setText(QApplication::translate("ANTWidget", "start", nullptr));
        StartDistanceEdit->setText(QApplication::translate("ANTWidget", "20", nullptr));
        IncrementLabel->setText(QApplication::translate("ANTWidget", "Incr.", nullptr));
        IncrementEdits->setText(QApplication::translate("ANTWidget", "20", nullptr));
        label_4->setText(QApplication::translate("ANTWidget", "Number of measurements", nullptr));
        label_8->setText(QApplication::translate("ANTWidget", "count", nullptr));
        NumOfMeasurementsEdit->setText(QApplication::translate("ANTWidget", "1", nullptr));
        groupBox_3->setTitle(QApplication::translate("ANTWidget", "USB Configuration", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ANTWidget: public Ui_ANTWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANTWIDGET_H

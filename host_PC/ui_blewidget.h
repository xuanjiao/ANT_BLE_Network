/********************************************************************************
** Form generated from reading UI file 'blewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLEWIDGET_H
#define UI_BLEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BLEWidget
{
public:
    QLabel *label_2;
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
    QPushButton *ConnectPortButtom;
    QComboBox *AvailablePortComboBox;
    QPushButton *AvailablePortButtom;
    QListView *listView;
    QPushButton *NextBotton;
    QPushButton *QuitButton;
    QPushButton *DisconnectButton;

    void setupUi(QWidget *BLEWidget)
    {
        if (BLEWidget->objectName().isEmpty())
            BLEWidget->setObjectName(QStringLiteral("BLEWidget"));
        BLEWidget->resize(616, 460);
        label_2 = new QLabel(BLEWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 140, 101, 16));
        groupBox_4 = new QGroupBox(BLEWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(30, 80, 271, 211));
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

        ConnectPortButtom = new QPushButton(BLEWidget);
        ConnectPortButtom->setObjectName(QStringLiteral("ConnectPortButtom"));
        ConnectPortButtom->setGeometry(QRect(110, 360, 101, 41));
        AvailablePortComboBox = new QComboBox(BLEWidget);
        AvailablePortComboBox->setObjectName(QStringLiteral("AvailablePortComboBox"));
        AvailablePortComboBox->setGeometry(QRect(180, 310, 101, 22));
        AvailablePortButtom = new QPushButton(BLEWidget);
        AvailablePortButtom->setObjectName(QStringLiteral("AvailablePortButtom"));
        AvailablePortButtom->setGeometry(QRect(40, 310, 121, 23));
        listView = new QListView(BLEWidget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(330, 90, 256, 192));
        NextBotton = new QPushButton(BLEWidget);
        NextBotton->setObjectName(QStringLiteral("NextBotton"));
        NextBotton->setGeometry(QRect(230, 360, 111, 41));
        QuitButton = new QPushButton(BLEWidget);
        QuitButton->setObjectName(QStringLiteral("QuitButton"));
        QuitButton->setGeometry(QRect(500, 410, 75, 23));
        DisconnectButton = new QPushButton(BLEWidget);
        DisconnectButton->setObjectName(QStringLiteral("DisconnectButton"));
        DisconnectButton->setGeometry(QRect(370, 360, 101, 41));

        retranslateUi(BLEWidget);

        QMetaObject::connectSlotsByName(BLEWidget);
    } // setupUi

    void retranslateUi(QWidget *BLEWidget)
    {
        BLEWidget->setWindowTitle(QApplication::translate("BLEWidget", "Form", nullptr));
        label_2->setText(QApplication::translate("BLEWidget", "Base file name", nullptr));
        groupBox_4->setTitle(QApplication::translate("BLEWidget", "File", nullptr));
        BaseFileNameEdit->setText(QApplication::translate("BLEWidget", "myResult", nullptr));
        label_3->setText(QApplication::translate("BLEWidget", "Distance", nullptr));
        label_6->setText(QApplication::translate("BLEWidget", "File directory", nullptr));
        label_5->setText(QApplication::translate("BLEWidget", ".txt", nullptr));
        FileDirectoryLabel->setText(QString());
        label_7->setText(QApplication::translate("BLEWidget", "start", nullptr));
        StartDistanceEdit->setText(QApplication::translate("BLEWidget", "20", nullptr));
        IncrementLabel->setText(QApplication::translate("BLEWidget", "Incr.", nullptr));
        IncrementEdits->setText(QApplication::translate("BLEWidget", "20", nullptr));
        label_4->setText(QApplication::translate("BLEWidget", "Number of measurements", nullptr));
        label_8->setText(QApplication::translate("BLEWidget", "count", nullptr));
        NumOfMeasurementsEdit->setText(QApplication::translate("BLEWidget", "1", nullptr));
        ConnectPortButtom->setText(QApplication::translate("BLEWidget", "connect port", nullptr));
        AvailablePortButtom->setText(QApplication::translate("BLEWidget", "show available port", nullptr));
        NextBotton->setText(QApplication::translate("BLEWidget", "next measurement", nullptr));
        QuitButton->setText(QApplication::translate("BLEWidget", "Quit", nullptr));
        DisconnectButton->setText(QApplication::translate("BLEWidget", "Disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BLEWidget: public Ui_BLEWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLEWIDGET_H

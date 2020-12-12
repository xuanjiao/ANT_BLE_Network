#include "blewidget.h"
#include "ui_blewidget.h"

static int measurementNum = 1;
QDir static directoryOf(const QString &subdir);

BLEWidget::BLEWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BLEWidget)
{
    ui->setupUi(this);
}

BLEWidget::~BLEWidget()
{
    delete ui;
}

void BLEWidget::on_AvailablePortButtom_clicked()
{
    ui->AvailablePortComboBox->clear();
    const QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for(int i = 0;i<list.size();i++){
        ui->AvailablePortComboBox->addItem(list[i].portName());
    }
}

void BLEWidget::on_ConnectPortButtom_clicked()
{
    QString portName = ui->AvailablePortComboBox->currentText();
    if(portName.isEmpty()){
        QMessageBox::warning(this,"warning","port name is empty","OK");
        return;
    }
    qDebug()<<"start connecting to port "<<portName;
    port->setPortName(portName);
    qDebug()<<"port name set: "<<port->portName();
    port->setBaudRate(QSerialPort::Baud115200);
    qDebug()<<"baudrate set: "<<port->baudRate();
    port->setParity(QSerialPort::NoParity);
    qDebug()<<"parity set: "<<port->parity();
    port->setStopBits(QSerialPort::OneStop);
    qDebug()<<"stop bits set"<<port->stopBits();
    port->setDataBits(QSerialPort::Data8);
    qDebug()<<"data bit set: "<<port->dataBits();
    port->setFlowControl(QSerialPort::NoFlowControl);
    qDebug()<<"flow bit set: "<<port->flowControl();
    QObject::connect(this,SIGNAL(error(QSerialPort::SerialPortError)),
        this, SLOT(serialPortError(QSerialPort::SerialPortError)));
    if(!port->open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"warning","open port failed","OK");
        qInfo()<<"open port "<<portName<<" failed";
    }else{
        qInfo()<<"open port"<<portName;
        on_NextBotton_clicked();
        connect(port,SIGNAL(readyRead),this,SLOT(getSerialMessage()));
    }

}

void BLEWidget::serialPortError(QSerialPort::SerialPortError error)
{
    qDebug()<<"Eorror: "<<error;
}

void BLEWidget::getSerialMessage(){

    QString RSSI;

   /*
    port->readLine();
    *out <<RSSI;
    * */
    bool bytesAvailable;
    int n = port->bytesAvailable();
    QString readData = port->readAll();
    bytesAvailable = port->waitForReadyRead(10);
    if( bytesAvailable != 0){
        n += port->bytesAvailable();
        readData += port->readAll();
    }
    if(n!= 0){
        qDebug()<<"read "<< n <<" bytes RSSI : "<<readData;
        RSSI = readData;
        *out <<RSSI;
    }
}

void BLEWidget::on_NextBotton_clicked()
{
    int distance = ui->StartDistanceEdit->text().toInt() + ui->IncrementEdits->text().toInt()*(measurementNum-1);
    QString fileName = ui->BaseFileNameEdit->text() + QString::number(distance) + ".txt";
    QString subFolderName = "file";
    QString fileDirectoryAndName = directoryOf(subFolderName).absoluteFilePath(fileName);
    file = new QFile(fileDirectoryAndName);
    file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate );
    if(!file->isOpen()){
                QMessageBox::warning(this,"open file error",fileDirectoryAndName + file->errorString(),"OK");
    }else{
                out = new QTextStream(file);
                qInfo()<<"detect new beacon, open file "<<fileDirectoryAndName<<" succeed";
    }
    ui->FileDirectoryLabel->setText("../"+subFolderName+"/"+fileName);
    ui->CountEdit->setText(QString::number(measurementNum));
    measurementNum++;
}

QDir static directoryOf(const QString &subdir)
{
QDir dir(QApplication::applicationDirPath());

#if defined(Q_OS_WIN)
if (dir.dirName().toLower() == "debug"
        || dir.dirName().toLower() == "release"
        || dir.dirName().toLower() == "bin")
    dir.cdUp();
#elif defined(Q_OS_MAC)
if (dir.dirName() == "MacOS") {
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
}
#endif
dir.cd(subdir);
return dir;
}

void BLEWidget::on_QuitButton_clicked()
{
    this->close();

}

void BLEWidget::on_DisconnectButton_clicked()
{
    if(port->isOpen()){
      port->close();
    }
    if(file->isOpen()){
        file->close();
    }
}

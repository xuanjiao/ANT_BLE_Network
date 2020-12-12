#include "antwidget.h"
#include "ui_antwidget.h"

static int measurementNum = 1;
QDir static directoryOf(const QString &subdir);

ANTWidget::ANTWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ANTWidget)
{
    ui->setupUi(this);
    connect(this,SIGNAL(new_text(QString)),this,SLOT(displayText(QString)));
    myDeviceModel = new QStandardItemModel(this);
    ui->listView->setModel(myDeviceModel);
    ui->listView->setIconSize(QSize(40,40));
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setWrapping(true);

    beaconTableModel = new QStandardItemModel(this);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(beaconTableModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,50);

    QStringList headers = {QString("Channel"),QString("Serial numer"),QString("RSSI value")};
    beaconTableModel->setHorizontalHeaderLabels(headers);
    ui->tableView->horizontalHeader()->setVisible(true);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(90);
}


ANTWidget::~ANTWidget()
{
    delete ui;
}

//disconnect ANT node and close window

void ANTWidget::on_DisconnectButton_clicked()
{
qDebug()<<"quit buttom clicked";
QString str;
if(myDevice != NULL){
    if(myDevice->isConnect){
        myDevice->disconnect();
        myDevice->isConnect = FALSE;
        str.sprintf("disconnect ANT node  (serial string: %s)\n",(char*)myDevice->aucDeviceSerial);
        delete myDevice;
        myDevice = NULL;
        myDeviceModel->removeRow(0);
        beaconTableModel->clear();
 }
}else{

    //if user clicked disconnect button, there is no myDevice object
    str = "no device connect\n";
}
emit new_text(str);
qDebug()<<str;

if(file){
    file->close();
}
}

void ANTWidget::on_QuitButton_clicked()
{

QString str;
if(myDevice != NULL){
    str.sprintf("disconnect ANT node (serial string: %s)\n",(char*)myDevice->aucDeviceSerial);
    myDevice->disconnect();
    myDevice->isConnect = FALSE;
    delete myDevice;
    myDevice = NULL;
}else{
    //if user clicked disconnect button, there is no myDevice object
    str = "no device connect\n";
}
qDebug()<<str<<"close window";

if(file){
    file->close();
}
this->close();



}
//store message in transmit buffer
void ANTWidget::on_SendButton_clicked()
{
if(myDevice == NULL){
    qDebug()<<"no device";
}else{
    QString qstr = ui->MessageEdit->text();
    emit send_new_msg(qstr);
}
}

void ANTWidget::printDeviceInfo(){
QString info;
info.sprintf("VID: 0x%04x\nPID:0x%04x\nDescription:%s\nSerial String:%s",
             myDevice->usDeviceVID,
             myDevice->usDevicePID,
             (char*)myDevice->aucDeviceDescription,
             (char*)myDevice->aucDeviceSerial);
QString iconPath = QString(":/images/ANTicon.png");
QStandardItem *item = new QStandardItem(QIcon(iconPath),info);
myDeviceModel->appendRow(item);
qDebug()<<info;
}

void ANTWidget::displayText(const QString &str){
ui->textBrowser->moveCursor(QTextCursor::End);
ui->textBrowser->insertPlainText(str);
}


/*
* initialize steps:
* 1. delete previous ANT node
* 2. store configuration in device class
* 3. connrct ANT node according to USB port
* 4. configure ANT node
* 5. print device information in list view
* 6. start
*/
void ANTWidget::on_NewDevice_clicked()
{
MaxMeasurementNum = ui->NumOfMeasurementsEdit->text().toInt();

if(myDevice != NULL){
    QString str;
    str.sprintf("disconnect ANT node (serial string: %s)\n",(char*)myDevice->aucDeviceSerial);
    myDevice->disconnect();
    delete myDevice;
    myDevice = NULL;
    myDeviceModel->removeRow(0);
}

myDevice = new Device();
QString qChType = ui->ChannelTypeComboBox->currentText();
UCHAR ucChannelType;

if(qChType == "Master"){
    ucChannelType = CHANNEL_TYPE_MASTER;
}else if(qChType == "Slave (channel 0 only)"){
    ucChannelType = CHANNEL_TYPE_SLAVE;
}else if(qChType == "Slave Multi Channel"){
    myDevice->isMultiChannel = true;
    ucChannelType = CHANNEL_TYPE_MULTI;
}else{
    ucChannelType = CHANNEL_TYPE_INVALID;
}
myDevice->storeConfig(ucChannelType,
                      ui->USBDeviceNumberEdit->text().toInt(),
                      ui->DeviceNumberEdit->text().toInt(),
                      ui->DeviceTypeEdit->text().toInt(),
                      ui->TransmissionTypeEdit->text().toInt(),
                      ui->RFEdit->text().toInt()
                      );
qDebug("GUI: store config finish");
connect(myDevice,SIGNAL(new_text(QString)),this,SLOT(displayText(QString)));
connect(this,SIGNAL(send_new_msg(QString)),myDevice,SLOT(sendAMessageAndStoreInBuffer(QString)));
connect(myDevice,SIGNAL(beaconInfo(int,QString,int)),this,SLOT(handleBeaconInfo(int,QString,int)));
connect(myDevice,SIGNAL(goToSearch(int)),this,SLOT(handleGoToSearch(int)));
connect(myDevice,SIGNAL(newBeacon(int)),this,SLOT(handleNewBeacon(int)));

if(myDevice->connectDevice()){
    myDevice->configure();
    emit new_text("Application initialization finish \n configure ANT node...\n");
    printDeviceInfo();
}else{
    emit new_text("No ANT device connect!");
    QMessageBox::warning(this,"Warning","No ANT device connected!","OK");
    delete myDevice;
    myDevice = NULL;
}

}

void ANTWidget::handleBeaconInfo(int ch, QString serialNum, int RSSI){
qDebug()<<"handleBeaco Info"<<ch<<serialNum<<RSSI;
beaconTableModel->setItem(ch,COLUMN_INDEX_CHANNEL,new QStandardItem(QString::number(ch)));
beaconTableModel->setItem(ch,COLUMN_INDEX_SERIAL_NUM,new QStandardItem(serialNum));
beaconTableModel->setItem(ch,COLUMN_INDEX_RSSI,new QStandardItem(QString::number(RSSI)));
if(out){
    if(myDevice->isMultiChannel){
        *out << ch <<"  "<< RSSI <<"\n";
    }else{
        *out << RSSI <<"\n";
    }

}
}

void ANTWidget::handleBeaconDisconnect(int iCh){
//delete beacon info from map;
QString serialNum = beaconTableModel->item(iCh,1)->data(Qt::DisplayRole).toString();
beaconMap.remove(serialNum);
qDebug("channel %d disconnect\n",iCh);
QString qsCh = QString::number(iCh);
beaconTableModel->setItem(iCh,0,new QStandardItem(qsCh));
beaconTableModel->setItem(iCh,1,new QStandardItem(serialNum));
beaconTableModel->setItem(iCh,2,new QStandardItem("disconnect"));
}

void ANTWidget::handleGoToSearch(int channel){
if(file){
    file->close();
    file = NULL;
}

if(measurementNum > MaxMeasurementNum ){
    this->on_QuitButton_clicked();
}
}


void ANTWidget::handleNewBeacon(int channel){


    emit new_text("Detect new beacon on channel "+QString::number(channel) +"\n");

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
        qDebug()<<"detect new beacon, open file "<<fileDirectoryAndName<<" succeed";
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateCOMPorts();
    for(int n=0;n!=128;n++){
        ui->registerComboBox->addItem(QString::number(n,16));
    }
    connect(ui->updateButton,SIGNAL(clicked(bool)),this,SLOT(updateCOMPorts()));
    connect(ui->portSettingButton,SIGNAL(clicked(bool)),this,SLOT(settingCOMPort()));
    connect(ui->registerComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeRegister(int)));
    connect(&usbPort,SIGNAL(errorSignal(QString)),this,SLOT(errorSlot(QString)));
    connect(&usbPort,SIGNAL(incomingRegisterSignal(unsigned char)),this,SLOT(incommingRegisterSlot(unsigned char)));
    connect(ui->connectButton,SIGNAL(clicked(bool)),SLOT(portConnectSlot()));

}
//////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete ui;
}
///////////////////////////////////////////////////////////////////////////////////////
void MainWindow::updateCOMPorts(){
    disconnect(ui->COMPortsBox,SIGNAL(currentIndexChanged(int)),this,SLOT(portChangeSlot()));
    ui->COMPortsBox->clear();
    QStringList list=usbPort.getSerialPorts();
    ui->COMPortsBox->addItems(list);
    connect(ui->COMPortsBox,SIGNAL(currentIndexChanged(int)),this,SLOT(portChangeSlot()));
}
////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::settingCOMPort(){
    serialPortsDialog dialog(this);
    QString name=ui->COMPortsBox->currentText();
    QSerialPort *port=usbPort.getCurrentPort();
    if(port==nullptr){
        QMessageBox box(this);
        box.setWindowTitle(tr("Ошибка"));
        box.setText(tr("Порта ")+name+tr(" не существует."));
        box.setDefaultButton(QMessageBox::Ok);
        box.setIcon(QMessageBox::Critical);
        box.exec();
        updateCOMPorts();
        return;
    }
    dialog.setPort(port);
    dialog.exec();
}
/////////////////////////////////////////////////////////////////////////////////
void MainWindow::changeRegister(int number){
    usbPort.getRegister(number);
    ui->byteCheckBox_0->setChecked(false);
    ui->byteCheckBox_0->setDisabled(true);
    ui->byteCheckBox_1->setChecked(false);
    ui->byteCheckBox_1->setDisabled(true);
    ui->byteCheckBox_2->setChecked(false);
    ui->byteCheckBox_2->setDisabled(true);
    ui->byteCheckBox_3->setChecked(false);
    ui->byteCheckBox_3->setDisabled(true);
    ui->byteCheckBox_4->setChecked(false);
    ui->byteCheckBox_4->setDisabled(true);
    ui->byteCheckBox_5->setChecked(false);
    ui->byteCheckBox_5->setDisabled(true);
    ui->byteCheckBox_6->setChecked(false);
    ui->byteCheckBox_6->setDisabled(true);
    ui->byteCheckBox_7->setChecked(false);
    ui->byteCheckBox_7->setDisabled(true);
    ui->HEXLabel->clear();
    ui->DECLabel->clear();
}
//////////////////////////////////////////////////////////////////////////
void MainWindow::portConnectSlot(){
    QString name=ui->COMPortsBox->currentText();
    if(ui->COMPortsBox->isEnabled()){
        if(!usbPort.connectPort(name)){
            QMessageBox box(this);
            box.setWindowTitle(tr("Ошибка"));
            box.setText(tr("Невозможно открыть порт ")+name);
            box.setDefaultButton(QMessageBox::Ok);
            box.setIcon(QMessageBox::Critical);
            box.exec();
            updateCOMPorts();
            ui->connectButton->setText(tr("Подключить"));
            return;
        }
        else{
            ui->connectButton->setText(tr("Отключить"));
            ui->COMPortsBox->setDisabled(true);
        }
    }
    else{
        ui->connectButton->setText(tr("Подключить"));
        ui->COMPortsBox->setEnabled(true);
    }
}
/////////////////////////////////////////////////////////////////////////
void MainWindow::incommingRegisterSlot(unsigned char reg){
    if(reg&(1<<(0))){
        ui->byteCheckBox_0->setChecked(true);
    }
    else{
        ui->byteCheckBox_0->setChecked(false);
    }
    if(reg&(1<<(1))){
        ui->byteCheckBox_1->setChecked(true);
    }
    else{
        ui->byteCheckBox_1->setChecked(false);
    }
    if(reg&(1<<(2))){
        ui->byteCheckBox_2->setChecked(true);
    }
    else{
        ui->byteCheckBox_2->setChecked(false);
    }
    if(reg&(1<<(3))){
        ui->byteCheckBox_3->setChecked(true);
    }
    else{
        ui->byteCheckBox_3->setChecked(false);
    }
    if(reg&(1<<(4))){
        ui->byteCheckBox_4->setChecked(true);
    }
    else{
        ui->byteCheckBox_4->setChecked(false);
    }
    if(reg&(1<<(5))){
        ui->byteCheckBox_5->setChecked(true);
    }
    else{
        ui->byteCheckBox_5->setChecked(false);
    }
    if(reg&(1<<(6))){
        ui->byteCheckBox_6->setChecked(true);
    }
    else{
        ui->byteCheckBox_6->setChecked(false);
    }
    if(reg&(1<<(7))){
        ui->byteCheckBox_7->setChecked(true);
    }
    else{
        ui->byteCheckBox_7->setChecked(false);
    }

    ui->HEXLabel->setText(QString::number(reg,16));
    ui->DECLabel->setText(QString::number(reg,10));
}
///////////////////////////////////////////////////////////////////////////
void MainWindow::errorSlot(QString error){
    QMessageBox box(this);
    box.setWindowTitle(tr("Ошибка порта "));
    box.setText(error);
    box.setDefaultButton(QMessageBox::Ok);
    box.setIcon(QMessageBox::Critical);
    box.exec();
    ui->connectButton->setText(tr("Подключить"));
}
//////////////////////////////////////////////////////////////////////////

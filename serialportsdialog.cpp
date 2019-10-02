#include "serialportsdialog.h"
#include "ui_serialportsdialog.h"

    serialPortsDialog::serialPortsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialPortsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Настройка последовательных портов"));

    port1=nullptr;

    fillComboBoxes();
    connect(ui->canselButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(saveSlot()));
}
///////////////////////////////////////////////////////////////////
serialPortsDialog::~serialPortsDialog()
{
    delete ui;
}
///////////////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::setPort(QSerialPort *port){
    int size=ui->baudRate->count();
    for(int n=0;n!=size;n++){
        if(ui->baudRate->itemData(n)==port->baudRate()){
            ui->baudRate->setCurrentIndex(n);
            break;
        }
    }
    size=ui->parity->count();
    for(int n=0;n!=size;n++){
        if(ui->parity->itemData(n)==port->parity()){
            ui->parity->setCurrentIndex(n);
            break;
        }
    }
    size=ui->dataBits->count();
    for(int n=0;n!=size;n++){
        if(ui->dataBits->itemData(n)==port->dataBits()){
            ui->dataBits->setCurrentIndex(n);
            break;
        }
    }
    size=ui->stopBits_1->count();
    for(int n=0;n!=size;n++){
        if(ui->stopBits_1->itemData(n)==port->dataBits()){
            ui->stopBits_1->setCurrentIndex(n);
            break;
        }
    }
    size=ui->flowControl->count();
    for(int n=0;n!=size;n++){
        if(ui->flowControl->itemData(n)==port->flowControl()){
            ui->flowControl->setCurrentIndex(n);
            break;
        }
    }
}
////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::fillComboBoxes(){
    ui->baudRate->addItem("1200",QSerialPort::Baud1200);
    ui->baudRate->addItem("2400",QSerialPort::Baud2400);
    ui->baudRate->addItem("4800",QSerialPort::Baud4800);
    ui->baudRate->addItem("9600",QSerialPort::Baud9600);
    ui->baudRate->addItem("19200",QSerialPort::Baud19200);
    ui->baudRate->addItem("38400",QSerialPort::Baud38400);
    ui->baudRate->addItem("57600",QSerialPort::Baud57600);
    ui->baudRate->addItem("115200",QSerialPort::Baud115200);

    ui->dataBits->addItem("5",QSerialPort::Data5);
    ui->dataBits->addItem("6",QSerialPort::Data6);
    ui->dataBits->addItem("7",QSerialPort::Data7);
    ui->dataBits->addItem("8",QSerialPort::Data8);

    ui->parity->addItem("No parity",QSerialPort::NoParity);
    ui->parity->addItem("Even parity",QSerialPort::EvenParity);
    ui->parity->addItem("Odd parity",QSerialPort::OddParity);
    ui->parity->addItem("Space parity",QSerialPort::SpaceParity);
    ui->parity->addItem("Mark parity",QSerialPort::MarkParity);

    ui->stopBits_1->addItem("1",QSerialPort::OneStop);
    ui->stopBits_1->addItem("1.5",QSerialPort::OneAndHalfStop);
    ui->stopBits_1->addItem("2",QSerialPort::TwoStop);

    ui->flowControl->addItem(tr("Нет"),QSerialPort::NoFlowControl);
    ui->flowControl->addItem(tr("Аппаратный"),QSerialPort::HardwareControl);
    ui->flowControl->addItem(tr("Программный"),QSerialPort::SoftwareControl);

}
////////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::saveSlot(){
    if(port1!=nullptr){
        port1->setBaudRate((QSerialPort::BaudRate)ui->baudRate->currentData().toInt());
        port1->setDataBits((QSerialPort::DataBits)ui->dataBits->currentData().toInt());
        port1->setFlowControl((QSerialPort::FlowControl)ui->flowControl->currentData().toInt());
        port1->setParity((QSerialPort::Parity)ui->parity->currentData().toInt());
        port1->setStopBits((QSerialPort::StopBits)ui->stopBits_1->currentData().toInt());
    }
    accept();
}

#include "usbcore.h"

usbCore::usbCore(QObject *parent):
    QObject (parent)
{
    currentPort=nullptr;
    inputBytesCounter=0;
    waitTimer = new QTimer(this);
    waitTimer->setSingleShot(true);
    connect(waitTimer,SIGNAL(timeout()),this,SLOT(waitTimeSignal()));
}
//////////////////////////////////////////////////////////
usbCore::~usbCore()
{
    if(currentPort!=nullptr){
        currentPort->close();
        delete currentPort;
    }
    delete waitTimer;
}
/////////////////////////////////////////////////////////
QStringList usbCore::getSerialPorts(){
    QList<QSerialPortInfo>list=QSerialPortInfo::availablePorts();
    QStringList stringList;
    int size=list.size();
    for(int n=0;n!=size;n++){
        stringList.append(list.at(n).portName());
    }
    return stringList;
}
///////////////////////////////////////////////////////////////////////
QSerialPort *usbCore::getCurrentPort(){
    return currentPort;
}
///////////////////////////////////////////////////////////////////////////
void usbCore::getRegister(int address){
    currentCommand=COMMAND_GET_REGISTER;
    rGetRregister(address);
}
//////////////////////////////////////////////////////////////////////////////////
bool usbCore::connectPort(QString portName){
    if(currentPort!=nullptr){
        disconnect(currentPort,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
        disconnect(currentPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(errorSlot(QSerialPort::SerialPortError)));
        currentPort->close();
        delete currentPort;
    }
    currentPort = new QSerialPort(portName);
    connect(currentPort,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    connect(currentPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(errorSlot(QSerialPort::SerialPortError)));
    if(!currentPort->open(QIODevice::ReadWrite)){
        return false;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usbCore::disconnectCurrentPort(){
    if(currentPort!=nullptr){
        currentPort->close();
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void usbCore::rGetRregister(unsigned char address){  
    if(currentPort!=nullptr){
        QByteArray array;
        QDataStream str(&array,QIODevice::WriteOnly);
        unsigned char size=4;
        str<<size;
        str<<static_cast<unsigned char>(COMMAND_GET_REGISTER);
        str<<static_cast<unsigned char>(address);
        str<<static_cast<unsigned char>(CRC16((unsigned char*)array.data(),array.size()));//контрольная сумма
        currentPort->write(array);
        waitTimer->start(WAIT_TIME);
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void usbCore::readyReadSlot(){
    inputBytesCounter+=currentPort->read((char*)&inputArray[inputBytesCounter],2000);//размер пакета ограничен 2000 байт
        if(inputBytesCounter<2000){//хотя функция QIODevice::read() и не даст забить в массив более 2000 байт, заполнение может происходить за несколько раз
            if(inputBytesCounter<=1){
                return;
            }
            if(inputArray[0]==(quint8)inputBytesCounter){//сравниваем размер пакета с его заголовком
                waitTimer->stop();//если пакет полностью получен, то сбрасываем таймер ожидания
                unsigned char crc=CRC16(inputArray,inputBytesCounter-1);//считаем CRC для всего пакета, кроме последнего байта
                if(crc==(unsigned char)inputArray[inputBytesCounter-1]){
                    inputBytesCounter=0;
                    readPacket();
                }
                else{//если СRC Не соответствуют
                    emit errorSignal(tr("Контрольная сумма принятого пакета не совпадает."));
                    return;
                }
            }
        }
        else{//если данных больше, чем 2000 байт, то это ошибка
            emit errorSignal(tr("Превышен размер принятого пакета."));
        }
}
///////////////////////////////////////////////////////////////////////////////////////
void usbCore::errorSlot(QSerialPort::SerialPortError error){
    if(error!=QSerialPort::NoError){
        QString errorString;
        switch(error){
            case(QSerialPort::DeviceNotFoundError):{
                errorString="An error occurred while attempting to open an non-existing device.";
                break;
            }
            case(QSerialPort::PermissionError):{
                errorString="An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open";
                break;
            }
            case(QSerialPort::OpenError):{
                errorString="An error occurred while attempting to open an already opened device in this object.";
                break;
            }
            case(QSerialPort::NotOpenError):{
                errorString="This error occurs when an operation is executed that can only be successfully performed if the device is open.";
                break;
            }
            case(QSerialPort::ParityError):{
                errorString="Parity error detected by the hardware while reading data. This value is obsolete. We strongly advise against using it in new code.";
                break;
            }
            case(QSerialPort::FramingError):{
                errorString="Framing error detected by the hardware while reading data. This value is obsolete. We strongly advise against using it in new code.";
                break;
            }
            case(QSerialPort::BreakConditionError):{
                errorString="Break condition detected by the hardware on the input line. This value is obsolete. We strongly advise against using it in new code.";
                break;
            }
            case(QSerialPort::WriteError):{
                errorString="An I/O error occurred while writing the data.";
                break;
            }
            case(QSerialPort::ReadError):{
                errorString="An I/O error occurred while reading the data.";
                break;
            }
            case(QSerialPort::ResourceError):{
                errorString="An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system.";
                break;
            }
            case(QSerialPort::UnsupportedOperationError):{
                errorString="The requested device operation is not supported or prohibited by the running operating system.";
                break;
            }
            case(QSerialPort::TimeoutError):{
                errorString="A timeout error occurred.";
                break;
            }
            case(QSerialPort::UnknownError):{
                errorString="An unidentified error occurred.";
                break;
            }
        }
        emit errorSignal(tr("Ошибка порта ")+currentPort->portName()+"\n"+errorString);
    }
}
//////////////////////////////////////////////////////////////////////////
void usbCore::waitTimeSlot(){
    emit errorSignal(tr("Превышено время ожидания."));
}
////////////////////////////////////////////////////////////////////////////
unsigned char usbCore::CRC16(unsigned char *pcBlock, unsigned short len){
    /*
      Name  : CRC-16 CCITT
      Poly  : 0x1021    x^16 + x^12 + x^5 + 1
      Init  : 0xFFFF
      Revert: false
      XorOut: 0x0000
      Check : 0x29B1 ("123456789")
      MaxLen: 4095 байт (32767 бит) - обнаружение
        одинарных, двойных, тройных и всех нечетных ошибок
    */
    unsigned short crc = 0xFFFF;
    unsigned char i;
    while(len--){
        crc^= *pcBlock++ << 8;
        for(i = 0;i<8;i++){
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return crc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void usbCore::readPacket(){
    emit incomingRegisterSignal(inputArray[1]);
}

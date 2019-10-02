#ifndef USBCORE_H
#define USBCORE_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDataStream>
#include <QObject>
#include <QTimer>

#define WAIT_TIME 10

//команды контроллеру
enum contrCommand : unsigned char{
    COMMAND_NO_COMMAND=0,
    COMMAND_GET_REGISTER,
    COMMAND_SET_GERISTER

};

class usbCore : public QObject
{
    Q_OBJECT
public:
    usbCore(QObject *parent=nullptr);
    ~usbCore();
    QStringList getSerialPorts();
    QSerialPort *getCurrentPort();
    void getRegister(int address);
    bool connectPort(QString portName);
    void disconnectCurrentPort();

protected:
    QSerialPort *currentPort;
    QTimer *waitTimer;
    contrCommand currentCommand;
    qint64 inputBytesCounter;
    unsigned char inputArray[1000];
    void rGetRregister(unsigned char address);
    unsigned char CRC16(unsigned char *pcBlock, unsigned short len);
    void readPacket();

protected slots:
    void readyReadSlot();
    void errorSlot(QSerialPort::SerialPortError error);
    void waitTimeSlot();



signals:
    void incomingRegisterSignal(unsigned char reg);
    void errorSignal(QString error);


};

#endif // USBCORE_H

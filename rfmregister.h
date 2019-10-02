#ifndef RFMREGISTER_H
#define RFMREGISTER_H

#include "rfmregisterbyte.h"


class rfmRegister
{
public:
    rfmRegister();
    unsigned char getAddress() const;
    void setAddress(unsigned char value);

protected:
    unsigned char address;
    QString description;
    rfmRegisterByte D0,D1,D2,D3,D4,D5,D6,D7;
    bool RW;//R - false RW - true


};

#endif // RFMREGISTER_H

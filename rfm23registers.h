#ifndef RFM23REGISTERS_H
#define RFM23REGISTERS_H

#include <QString>
#include "rfmregister.h"

class RFM23Registers
{
public:
    RFM23Registers();


protected:
    rfmRegister registersArray[128];

};

#endif // RFM23REGISTERS_H

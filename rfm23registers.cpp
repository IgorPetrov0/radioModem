#include "rfm23registers.h"

RFM23Registers::RFM23Registers()
{
    for(unsigned char n=0;n!=127;n++){///привсаиваем адреса
        registersArray[n].setAddress(n);
    }



}

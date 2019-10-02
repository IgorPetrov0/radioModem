#include "rfmregister.h"

rfmRegister::rfmRegister()
{
    address=0;
    RW=false;//R - false RW - true
}
///////////////////////////////////////////////////////////////////////////////
unsigned char rfmRegister::getAddress() const{
    return address;
}
/////////////////////////////////////////////////////////////////////////////////
void rfmRegister::setAddress(unsigned char value){
    address = value;
}

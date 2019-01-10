#ifndef RS232_H
#define RS232_H


#include "../Communications/serialCommunications/serialport.h"

class RS232: public SerialPort{

public:
    RS232();
    ~RS232();
};

#endif // RS232_H

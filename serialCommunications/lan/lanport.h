#ifndef LANPORT_H
#define LANPORT_H

#include "../Communications/serialCommunications/serialport.h"

class LANPort: public SerialPort{

public:
    LANPort();
    ~LANPort();
};

#endif // LANPORT_H

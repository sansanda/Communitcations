#ifndef PARALLELPORT_H
#define PARALLELPORT_H

#include "../Communications/communicationPort.h"

class ParallelPort: public CommunicationsPort
{
public:
    ParallelPort():CommunicationsPort(){}
    ~ParallelPort(){}
};

#endif // PARALLELPORT_H

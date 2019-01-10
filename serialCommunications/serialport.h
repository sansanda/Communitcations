#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "../Communications/communicationPort.h"

class SerialPort: public CommunicationsPort
{
public:
    SerialPort():CommunicationsPort()
    {
        setDataBits(this->DATABITS_8);
        setFlowControl(this->FLOWCONTROL_NONE);
        setParityType(this->PARITY_NONE);
        setStopBits(this->STOPBITS_1);
        setBaudRate(this->BAUDRATE_9600);
        setPortType(this->PORT_SERIAL);
    }
    SerialPort(QString pname,
               int pnumber,
               int ptype,
               QString obs,
               int db,
               int fct,
               int pt,
               int sb,
               int br):CommunicationsPort(pname,pnumber,ptype,obs)
    {
        setDataBits(db);
        setFlowControl(fct);
        setParityType(pt);
        setStopBits(sb);
        setBaudRate(br);
        setPortType(this->PORT_SERIAL);
    }
    ~SerialPort(){}

public:
    static const int DATABITS_5 = 5;
    static const int DATABITS_6 = 6;
    static const int DATABITS_7 = 7;
    static const int DATABITS_8 = 8;
    static const int FLOWCONTROL_NONE = 0;
    static const int FLOWCONTROL_RTSCTS_IN = 1;
    static const int FLOWCONTROL_RTSCTS_OUT = 2;
    static const int FLOWCONTROL_XONXOFF_IN = 4;
    static const int FLOWCONTROL_XONXOFF_OUT = 8;
    static const int PARITY_EVEN = 2;
    static const int PARITY_MARK = 3;
    static const int PARITY_NONE = 0;
    static const int PARITY_ODD = 1;
    static const int PARITY_SPACE = 4;
    static const int STOPBITS_1 = 1;
    static const int STOPBITS_1_5 = 3;
    static const int STOPBITS_2 = 2;

    static const int BAUDRATE_9600 = 9600;
    static const int BAUDRATE_14400 = 14400;



    int dataBits;
    int stopBits;
    int flowControl;
    int parityType;
    int baudRate;

    int getDataBits(){return dataBits;}
    int getStopBits(){return stopBits;}
    int getFlowControl(){return flowControl;}
    int getParityType(){return parityType;}
    int getBaudRate(){return baudRate;}

    void setDataBits(int db){dataBits = db;}
    void setStopBits(int sb){stopBits = sb;}
    void setFlowControl(int fc){flowControl = fc;}
    void setParityType(int pt){parityType = pt;}
    void setBaudRate(int br){baudRate = br;}

    virtual void print()
    {


        std::cout<<"*******************************************"<<std::endl;
        std::cout<<"**************SERIAL PORT DATA***************"<<std::endl;
        std::cout<<"*******************************************"<<std::endl;

        std::cout<<"  Port Name: "<<getPortName().toStdString()<<std::endl;
        std::cout<<"Port Number: "<<getPortNumber()<<std::endl;
        std::cout<<"        Obs: "<<getObservations().toStdString()<<std::endl;

    }
};

#endif // SERIALPORT_H

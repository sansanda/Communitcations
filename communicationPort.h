#ifndef COMMUNICATIONPORT_H
#define COMMUNICATIONPORT_H

#include <QObject>
#include <QCoreApplication>
#include <QtXml>
#include <QString>
#include <QVariant>
#include <QTextStream>
#include "iostream"

class CommunicationsPort: public QObject
{
    Q_OBJECT
public:
    CommunicationsPort(QString pname,int pnumber, int ptype, QString obs )
    {
        setPortName(pname);
        setObservations(obs);
        setPortNumber(pnumber);
        setPortType(ptype);
    }
    CommunicationsPort()
    {
        setPortName("Default Name");
        setObservations("Default Observations");
        setPortNumber(1);
        setPortType(this->PORT_SERIAL);
    }

    QString getPortName(){ return portName;}
    QString getObservations(){return observations;}
    int     getPortNumber(){return portNumber;}
    int     getPortType(){return portType;}

    void setPortName(QString name){ portName = name;}
    void setObservations(QString obs){observations = obs;}
    void setPortNumber(int pn){portNumber = pn;}
    void setPortType(int pt){portType = pt;}

    virtual void print()
    {


        std::cout<<"*******************************************"<<std::endl;
        std::cout<<"**************COMM PORT DATA***************"<<std::endl;
        std::cout<<"*******************************************"<<std::endl;

        std::cout<<"  Port Name: "<<getPortName().toStdString()<<std::endl;
        std::cout<<"Port Number: "<<getPortNumber()<<std::endl;
        std::cout<<"        Obs: "<<getObservations().toStdString()<<std::endl;
        std::cout<<"  Port Type: "<<getPortType()<<std::endl;
    }

public:
    static const int PORT_PARALLEL = 2;
    static const int PORT_SERIAL = 1;

private:
    QString portName;
    QString observations;
    int portNumber;
    int portType;


};

#endif // COMMUNICATIONPORT_H

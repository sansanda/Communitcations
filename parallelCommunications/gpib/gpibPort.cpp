#include "../Communications/parallelCommunications/gpib/gpibPort.h"

static bool noError = true;
static int device;

GPIBPort::GPIBPort(int addr):ParallelPort()
{
    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT:GPIBPort("+QString::number(addr)+"): Constructing an instance of GPIBPort class. ";
    #endif
    int add = addr;
    if (addr<=0 || addr>31) add=1;
    setAddress(add);
}
GPIBPort::GPIBPort():ParallelPort()
{
    int add = 24;
    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT:GPIBPort("+QString::number(add)+"): Constructing an instance by defalult of GPIBPort class. ";
    #endif
    setAddress(add);
}
GPIBPort::~GPIBPort(){}


void GPIBPort::setAddress(int addr)
{
    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"):void setAddress("+QString::number(addr)+"): Setting the address of the GPIB Port. ";
    #endif
    if (addr<=0 || addr>31) address = 24;
    address = addr;
}

int GPIBPort::getAddress(){return address;}
int GPIBPort::setTimeout (int dev, int timo)
{
    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT:: Setting the timeout of the GPIB Card. ";
    #endif
    if (dev!=0) return ibtmo(device,timo);
    else return ibtmo(BOARD_INDEX,timo);
}

int GPIBPort::executePolling(char *spr)
{
    return ibrsp(device,spr);
}

/**
  * @brief Test GPIB instructions
  *
  */
int GPIBPort::testInstrunction(QString instruction)
{
    QString output = instruction;
    if(isNoError()) return write(output.toLocal8Bit().data() );
    else return -1;
}

/**
  * @brief Use to read Status and Standard registers and masks (enable)
  */
int GPIBPort::allResgistersQueryTest()
{
    int result = 0;
    // Status byte register
    // Event
    //QTest::qSleep(1000);

    int statusR = stbQuery();
    statusR = statusR +1 -1;

    // Enable
    //QTest::qSleep(1000);

    QString statusE = sreQuery();
    statusE = statusE.append("!!");

    // Standard register
    // Event
    //QTest::qSleep(1000);
    QString standardR = esrQuery().toString();
    standardR = standardR.append("!!");

    // Enable
    QString standardE = eseQuery().toString();
    statusE = standardE.append("!!");

    return result;
}


int GPIBPort::sendReadQueryAsCharArrayAsynchronously(char* message, const int bytesToRead)
{

    QString output = QString( ":READ?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );
    //else qDebug() << "GPIB ERROR";

    return reada( message, bytesToRead );
}

int GPIBPort::sendReadQueryAndGetResultAsCharArray( char* message, const int bytesToRead )
{
    QString output = QString( ":READ?" );
    //qDebug() << output.toLocal8Bit().data();

    if( isNoError() ) write( output.toLocal8Bit().data() );
    //else qDebug() << "GPIB ERROR";
    return read( message, bytesToRead );
}

int GPIBPort:: sendReadQueryAndGetResultAsString(int size, QString &result)
{
    int readSize = size;
    if (readSize< (READBUFFER_SIZE_MIN) || readSize>(READBUFFER_SIZE_MAX)) readSize = READBUFFER_SIZE_DEFAULT;

    QString output = QString( ":READ?" );
    //qDebug() << output.toLocal8Bit().data();

    if( isNoError() ) {
        //qDebug() << "NO GPIB ERROR";
        write( output.toLocal8Bit().data() );
        QThread::msleep(10);
    }
    //else qDebug() << "GPIB ERROR";

    char measure[readSize + 50];
    if( isNoError() ) read( measure, readSize);


    result = QString(QVariant(measure).toString());

    return errors();
}


/**
  * @brief Read messages from GPIB bus.
  *
  * @return A char[100] that contains 42 bytes read from the device.
  */

int GPIBPort:: read(int size, QVariant &result)
{
    int readSize = size;
    if (readSize< (READBUFFER_SIZE_MIN) || readSize>(READBUFFER_SIZE_MAX)) readSize = READBUFFER_SIZE_DEFAULT;


#ifndef TEST
    char measure[readSize + 50];
    if( isNoError() ) read( measure, readSize);

    /*for (int i=0;i<(readSize + 50);i++) {
        qDebug() << measure[i];
    }*/

    result = QVariant(measure);
#else
    result = QVariant("+9.979305E-01,+1.003080E-04,+1.074219E-02\n");
    //return QVariant("UM, RANGE=12.000kV, VALUE=1.053kV");
    //return QVariant("+5.000000E+00,+8.249011E-05,+5.285156E+02,+6.111000E+00,+8.381449E-05,+5.285205E+02,+7.222000E+00,+8.429056E-05,+5.285254E+02,+8.333500E+00,+8.415909E-05,+5.285303E+02,+9.444500E+00,+8.352670E-05,+5.285352E+02,+1.055550E+01,+8.389394E-05,+5.285400E+02,+1.166650E+01,+8.381329E-05,+5.285449E+02,+1.277800E+01,+8.840705E-05,+5.285498E+02,+1.388900E+01,+8.393636E-05,+5.285547E+02,+1.500000E+01,+8.438555E-05,+5.285596E+02\n\320\231\210\001\210a"" @0x1528be8");
#endif
    return errors();
}

/**
  * @brief Read (Synchronously) the device GPIB message.
  *
  * @param message Is the storage buffer for the read data.
  * @param size An integer that specifies the maximum number of bytes to read.
  */

int GPIBPort::read(char* message, int size){
#ifndef TEST
    if( isNoError() ) ibrd (device, message, size);
#endif
    return errors();
}

/**
  * @brief Read (Asynchronously) the device GPIB message.
  *
  * @param message Is the storage buffer for the read data.
  * @param size An integer that specifies the maximum number of bytes to read.
  */

int GPIBPort::reada(char* message, int size){
#ifndef TEST
    if( isNoError() ) ibrda (device, message, size);
#endif
    return errors();
}

/**
  * @brief getNumberOfReadBytes().
  *
  * @params none.
  * @return The number of bytes readed on the last read operation.
  */

int GPIBPort::getNumberOfReadBytes(){
#ifndef TEST
    if( isNoError() ) return ibcnt;
#endif
}

/**
  * @brief Open a device GPIB connection.
  */

int GPIBPort::openConnection(int eos){
    int status = EXIT_SUCCESS;

    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"):int openConnection("+QString::number(eos)+"): Starting the opening the connection process.";
    #endif

#ifndef TEST
    short listen;

    if( isNoError() ){
        int v;
        std::cout<<"Looking for the GPIB Interface"<<std::endl;
        #if DEBUG_GPIBPORT==1
            qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"):int openConnection("+QString::number(eos)+"): ibask("+QString::number(BOARD_INDEX)+",1,&v): Query the configuration of the GPIB Board ";
        #endif

        ibask(BOARD_INDEX,1,&v);
        status = errors();
    }
    if( isNoError() ){
        #if DEBUG_GPIBPORT==1
            qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"):int openConnection("+QString::number(eos)+"): ibdev("+QString::number(BOARD_INDEX)+", "+QString::number(address)+", "+QString::number(SAD)+", "+QString::number(TIMEOUT_3s)+", "+QString::number(EOT)+", "+QString::number(eos)+"): Open a the GPIB Board ";
        #endif
        device = ibdev(BOARD_INDEX, address, SAD, TIMEOUT_3s, EOT, eos);
        status = errors();
    }
    if( isNoError() ){
        #if DEBUG_GPIBPORT==1
            qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"):int openConnection(int eos): ibln( "+QString::number(BOARD_INDEX)+", "+QString::number(address)+", "+QString::number(SAD)+ ", &listen): Check if listener (Instrument. Not the GPIB Board) is present. ";
        #endif
        ibln( BOARD_INDEX, address, SAD, &listen);
        status = errors();
    }
#endif
    setNoError(true);
    return status;
}

/**
  * @brief Write a desired GPIB instruction.
  *
  * @param instruction QString that contains the GPIB instruction desired.
  */
int GPIBPort::write(QString instruction){
    if( isNoError()) write(instruction.toLocal8Bit().data());
    return errors();
}

/**
  * @brief Write a desired GPIB instruction.
  *
  * @param instruction A char sequence that contains the GPIB instruction desired.
  */
int GPIBPort::write(char * instruction){

    #if SHOW_GPIB_COMMANDS == 1
            qDebug() << instruction;
    #endif
    if( isNoError()) ibwrt( device, instruction, strlen(instruction) );
    return errors();
}

/**
  * @brief Asserts/unasserts remote enableazaa
  */
int GPIBPort::remoteEnable()
{
    int result = 0;
#ifndef TEST
    if( noError ){
        //ibsre( device, 1 ); // depracated
        result = ibconfig( device, IbcSRE, 1);
        errors();
    } else result = -1;
#endif
    return result;
}

/**
  * @brief Clear a specific device
  */
void GPIBPort::clearDevice()
{
#ifndef TEST
    if( noError){
        ibclr( device );
        errors();
    }
#endif
}

/**
  * @brief go local
  */

void GPIBPort::closeConnection(){
#ifndef TEST
    // Force the device into local mode
    if( noError ){
        ibloc( device );
        errors(); // Just check for errors if there is no previous errors
        // avoids errors loops
    }
#endif
}

/**
  * @brief Close device
  */

void GPIBPort::disable(){
#ifndef TEST
    // Force the device into local mode
    if( noError ){
        ibonl(device, 0);
        errors();
    }
#endif
}

bool GPIBPort::isNoError() {return noError;}

void GPIBPort::setNoError(bool state) {noError = state;}

/**
  * @brief Detect if a GPIB error just happened.
  */

int GPIBPort::errors(){
    int status = EXIT_SUCCESS;

#if DEBUG_GPIBPORT==1
    qDebug()<<"GPIBPort::errors(): iberr :"+ QString::number(iberr) ;
#endif

#ifndef TEST

    if( ( ibsta & ERR ) == ERR ) {
        //We have and error!
        //Extracting type of error
        QString errorMessageText = "GPIB Error: ";
        switch( iberr ){
        case EDVR:
            errorMessageText.append("System error.");
            status = -1;break;
        case ECIC:
            errorMessageText.append("Function requires GPIB board to be CIC.");
            status = -1;break;
        case ENOL:
            errorMessageText.append("Write function detected no Listeners.");
            status = -2;break;
        case EADR:
            errorMessageText.append("Interface board not addressed correctly.");
            status = -1;break;
        case EARG:
            errorMessageText.append("Invalid argument to function call.");
            status = -1;break;
        case ESAC:
            errorMessageText.append("Function requires GPIB board to be SAC.");
            status = -1;break;
        case EABO:
            errorMessageText.append("I/O operation aborted.");
            status = -1;break;
        case ENEB:
            errorMessageText.append("Non-existent interface board.");
            status = -1;break;
        case EDMA:
            errorMessageText.append("DMA hardware error detected.");
            status = -1;break;
        case EOIP:
            errorMessageText.append("I/O operation started before previous operation completed.");
            status = -1;break;
        case ECAP:
            errorMessageText.append("No capability for intended operation.");
            status = -1;break;
        case EFSO:
            errorMessageText.append("File system operation error.");
            status = -1;break;
        case EBUS:
            errorMessageText.append("Command error during device call.");
            status = -1;break;
        case ESTB:
            errorMessageText.append("Serial poll status byte lost.");
            status = -1;break;
        case ESRQ:
            errorMessageText.append("SRQ remains asserted.");
            status = -1;break;
        case ETAB:
            errorMessageText.append("The return buffer is full.");
            status = -1;break;
        }
        errorMessage( errorMessageText );


    }
#endif
    return status;
}

/**
  * @brief Pop up errors
  *
  * @param erroType A QString that holds the error type.
  */

void GPIBPort::errorMessage(QString errorType)
{
    QMessageBox errorBox;
    errorBox.setIcon( QMessageBox::Critical );

    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"): void GPIBPort::errorMessage("+errorType+"): " + errorType.toLocal8Bit().constData();
    #endif

    errorBox.setText( errorType.toLocal8Bit().constData());
    errorBox.exec();
    setNoError(false);
}

/**
  * @brief Reads Status Byte Register.
  *
  * @return An integer containing the Status Byte info.
  */

QString GPIBPort::sreQuery()
{
    QString output = QString( "*SRE?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes.toString();
}

/**
  * @brief Reads Status Byte Register.
  *
  * @return An integer containing the Status Byte info.
  */

int GPIBPort::stbQuery()
{
    QString output = QString( "*STB?" );

#ifndef TEST

    if( isNoError() ) write( output.toLocal8Bit().data() );
    QVariant qVarRes;
    read(READBUFFER_SIZE_DEFAULT,qVarRes);
    QByteArray statusRegister = qVarRes.toByteArray();

    int i = 0;
    while( statusRegister.at( i ) != '\n' )
        i++;

    statusRegister.truncate(i);
    return statusRegister.toInt();

#else

    return 0;

#endif
}

/**
  * @brief ......
  *
  * @return A QVariant containing the .....
  */

QVariant GPIBPort::esrQuery()
{
    QString output = QString( "*ESR?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief ......
  *
  * @return A QVariant containing the .....
  */

QVariant GPIBPort::eseQuery()
{
    QString output = QString( "*ESE?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief Reads and resets Operation Event Register.
  *
  * @return A QVariant containing the Operation Event Register status previous reset.
  */

QVariant GPIBPort::oerQuery()
{
    QString output = QString( ":STAT:OPER:EVEN?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief Reads and resets Measurement Event Register.
  *
  * @return A QVariant containing the Measurement Event Register status previous reset.
  */

QVariant GPIBPort::merQuery()
{
    QString output = QString( ":STAT:MEAS:EVEN?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief Reads and resets Questionable Event Register.
  *
  * @return A QVariant containing the Questionable Event Register status previous reset.
  */

QVariant GPIBPort::qerQuery()
{
    QString output = QString( ":STAT:QUES:EVEN?" );

    if( isNoError() ) write( output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

int GPIBPort::waitForResponse(int status_mask){return ibwait(device,status_mask);}


/**
  * @brief Trigger and reads Status Byte Register.
  *
  * @return A Qstring cointaining the last error read from the error queue.
  */
/*
QString GPIBPort::queueErrorQuery()
{
    QString output = QString( ":STAT:QUE?" );
    qDebug() << output.toLocal8Bit().data();

    allResgistersQueryTest();

    if( isNoError() ) write( output.toLocal8Bit().data() );
    return read().toString();
}
*/
/**
  * @brief Trigger and adquires readings.
  */
/*
QVariant GPIBPort::readQuery( const int size )
{
    QString output = QString( ":READ?" );
    qDebug() << output.toLocal8Bit().data();
    write(output);
    return read( size );
}
*/
/**
  * @brief get data from instrument buffer.
  */
/*
QVariant GPIBPort::dataQuery( const int size )
{
    QString output = QString( ":TRAC:DATA?" );
    qDebug() << output.toLocal8Bit().data();
    write(output);
    return read( size );
}
*/
/**
  * @brief Fetch data from instrument buffer.
  */
/*
QVariant GPIBPort::fetchQuery( const int size )
{
    QString output = QString( ":FETC?" );
    qDebug() << output.toLocal8Bit().data();
    write(output);
    return read( size );
}
*/

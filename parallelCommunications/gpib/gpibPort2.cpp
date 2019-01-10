#include "../Communications/parallelCommunications/gpib/GPIBPort2.h"

int     GPIBPort2::address = 21;
bool    GPIBPort2::noError = true;
int     GPIBPort2::device = 0;
int     GPIBPort2::boardPrimaryAddress = BOARD_INDEX;
int     GPIBPort2::boardSecondaryAddress = SAD;
QList<int> *GPIBPort2::instrumentsList = new QList<int>();




int GPIBPort2::getPrimaryBoardAddress(){return boardPrimaryAddress;}
int GPIBPort2::getSecondaryBoardAddress(){return boardSecondaryAddress;}
int GPIBPort2::setPrimaryBoardAddress(int pAddr){boardPrimaryAddress=pAddr;}
int GPIBPort2::setSecondaryBoardAddress(int sAddr){boardSecondaryAddress=sAddr;}


int GPIBPort2::setBoardTimeout (int timo)
{
    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT:: Setting the timeout of the GPIB Card. ";
    #endif
    return ibtmo(boardPrimaryAddress,timo);
}

int GPIBPort2::setInstrumentTimeout (int instHandler, int timo)
{
    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT:: Setting the timeout of the Instrument. ";
    #endif
    return ibtmo(instHandler,timo);
}

int GPIBPort2::executePolling(int instHandler, char *spr)
{
    return ibrsp(instHandler,spr);
}

/**
  * @brief Test GPIB instructions
  *
  */
int GPIBPort2::testCommand(int instHandler,QString instruction)
{
    QString output = instruction;
    if(isNoError()) return write(instHandler,output.toLocal8Bit().data() );
    else return -1;
}

/**
  * @brief Use to read Status and Standard registers and masks (enable)
  */
int GPIBPort2::allResgistersQueryTest(int instHandler)
{
    int result = 0;
    // Status byte register
    // Event
    //QTest::qSleep(1000);

    int statusR = stbQuery(instHandler);
    statusR = statusR +1 -1;

    // Enable
    //QTest::qSleep(1000);

    QString statusE = sreQuery(instHandler);
    statusE = statusE.append("!!");

    // Standard register
    // Event
    //QTest::qSleep(1000);
    QString standardR = esrQuery(instHandler).toString();
    standardR = standardR.append("!!");

    // Enable
    QString standardE = eseQuery(instHandler).toString();
    statusE = standardE.append("!!");

    return result;
}


int GPIBPort2::sendReadQueryAsCharArrayAsynchronously(int instHandler,char* message, const int bytesToRead)
{

    QString output = QString( ":READ?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );
    //else qDebug() << "GPIB ERROR";

    return reada(instHandler, message, bytesToRead );
}

int GPIBPort2::sendReadQueryAndGetResultAsCharArray(int instHandler,char* message, const int bytesToRead )
{
    QString output = QString( ":READ?" );
    //qDebug() << output.toLocal8Bit().data();

    if( isNoError() ) write(instHandler, output.toLocal8Bit().data() );
    //else qDebug() << "GPIB ERROR";
    return read(instHandler, message, bytesToRead );
}

int GPIBPort2:: sendReadQueryAndGetResultAsString(int instHandler,int size, QString &message)
{
    int readSize = size;
    if (readSize< (READBUFFER_SIZE_MIN) || readSize>(READBUFFER_SIZE_MAX)) readSize = READBUFFER_SIZE_DEFAULT;

    QString output = QString( ":READ?" );
    //qDebug() << output.toLocal8Bit().data();

    if( isNoError() ) {
        //qDebug() << "NO GPIB ERROR";
        write(instHandler,output.toLocal8Bit().data() );
        QThread::msleep(10);
    }
    //else qDebug() << "GPIB ERROR";

    char measure[readSize + 50];
    if( isNoError() ) read(instHandler, measure, readSize);


    message = QString(QVariant(measure).toString());

    return errors();
}


/**
  * @brief Read messages from GPIB bus.
  *
  * @return A char[100] that contains 42 bytes read from the device.
  */

int GPIBPort2:: read(int instHandler,int size, QVariant &result)
{
    int readSize = size;
    if (readSize< (READBUFFER_SIZE_MIN) || readSize>(READBUFFER_SIZE_MAX)) readSize = READBUFFER_SIZE_DEFAULT;


#ifndef TEST
    char measure[readSize + 50];
    if( isNoError() ) read(instHandler,measure, readSize);

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

int GPIBPort2::read(int instHandler,char* message, int size){
#ifndef TEST
    if( isNoError() ) ibrd (instHandler, message, size);
#endif
    return errors();
}

/**
  * @brief Read (Asynchronously) the device GPIB message.
  *
  * @param message Is the storage buffer for the read data.
  * @param size An integer that specifies the maximum number of bytes to read.
  */

int GPIBPort2::reada(int instHandler,char* message, int size){
#ifndef TEST
    if( isNoError() ) ibrda (instHandler, message, size);
#endif
    return errors();
}

/**
  * @brief getNumberOfReadBytes().
  *
  * @params none.
  * @return The number of bytes readed on the last read operation.
  */

int GPIBPort2::getNumberOfReadBytes(){
    #ifndef TEST
        if( isNoError() ) return ibcnt;
    #endif
}

/**
  * @brief Open a device GPIB connection.
  */

int GPIBPort2::openConnection(int instAddress,int eos){
    int status = EXIT_SUCCESS;
    int instHandler;

    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT("+QString::number(instAddress)+"):int openConnection("+QString::number(eos)+"): Starting the opening the connection process.";
    #endif

#ifndef TEST
    short listen;

    if( isNoError() ){
        int v;
        #if DEBUG_GPIBPORT==1
                qDebug()<<"Looking for the Instrument GPIB Interface";
        #endif

        #if DEBUG_GPIBPORT==1
            qDebug()<<"GPIB-PORT("+QString::number(instAddress)+"):int openConnection("+QString::number(eos)+"): ibask("+QString::number(boardPrimaryAddress)+",1,&v): Query the configuration of the GPIB Board ";
        #endif

        ibask(boardPrimaryAddress,1,&v);
        status = errors();
    }
    if( isNoError() ){
        #if DEBUG_GPIBPORT==1
            qDebug()<<"GPIB-PORT("+QString::number(instAddress)+"):int openConnection("+QString::number(eos)+"): ibdev("+QString::number(boardPrimaryAddress)+", "+QString::number(instAddress)+", "+QString::number(boardSecondaryAddress)+", "+QString::number(TIMEOUT_3s)+", "+QString::number(EOT)+", "+QString::number(eos)+"): Open a the GPIB Board ";
        #endif
        instHandler = ibdev(boardPrimaryAddress, instAddress, boardSecondaryAddress, NEVERTIMEOUT, EOT, eos);
        status = errors();
    }
    if( isNoError() ){
        #if DEBUG_GPIBPORT==1
            qDebug()<<"GPIB-PORT("+QString::number(instAddress)+"):int openConnection(int eos): ibln( "+QString::number(boardPrimaryAddress)+", "+QString::number(instAddress)+", "+QString::number(boardSecondaryAddress)+ ", &listen): Check if listener (Instrument. Not the GPIB Board) is present. ";
        #endif
        ibln( boardPrimaryAddress, instAddress, boardSecondaryAddress, &listen);
        status = errors();
    }
#endif

    setNoError(true);
    if (status == EXIT_SUCCESS) return instHandler;
    else return status;
}

/**
  * @brief Write a desired GPIB instruction.
  *
  * @param instruction QString that contains the GPIB instruction desired.
  */
int GPIBPort2::write(int instHandler, QString instruction){
    if( isNoError()) write(instHandler,instruction.toLocal8Bit().data());
    return errors();
}

/**
  * @brief Write a desired GPIB instruction.
  *
  * @param instruction A char sequence that contains the GPIB instruction desired.
  */
int GPIBPort2::write(int instHandler, char * instruction){

    #if SHOW_GPIB_COMMANDS == 1
            qDebug() << instruction;
    #endif
    if( isNoError()) ibwrt( instHandler, instruction, strlen(instruction) );
    return errors();
}

/**
  * @brief Asserts/unasserts remote enableazaa
  */
int GPIBPort2::remoteEnable(int instHandler)
{
    int result = 0;
#ifndef TEST
    if( noError ){
        //ibsre( device, 1 ); // depracated
        result = ibconfig( instHandler, IbcSRE, 1);
        errors();
    } else result = -1;
#endif
    return result;
}

/**
  * @brief Clear a specific device
  */
void GPIBPort2::clearInstrument(int instHandler)
{
#ifndef TEST
    if( noError){
        ibclr(instHandler);
        errors();
    }
#endif
}

/**
  * @brief go local
  */

void GPIBPort2::closeConnection(int instHandler){
#ifndef TEST
    // Force the device into local mode
    if( noError ){
        ibloc( instHandler );
        errors(); // Just check for errors if there is no previous errors
        // avoids errors loops
    }
#endif
}

/**
  * @brief Close device
  */

void GPIBPort2::disable(int instHandler){
#ifndef TEST
    // Force the device into local mode
    if( noError ){
        ibonl(instHandler, 0);
        errors();
    }
#endif
}

bool GPIBPort2::isNoError() {return noError;}

void GPIBPort2::setNoError(bool state) {noError = state;}

/**
  * @brief Detect if a GPIB error just happened.
  */

int GPIBPort2::errors(){
    int status = EXIT_SUCCESS;

#if DEBUG_GPIBPORT==1
    qDebug()<<"GPIBPort2::errors(): iberr :"+ QString::number(iberr) ;
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

void GPIBPort2::errorMessage(QString errorType)
{
    QMessageBox errorBox;
    errorBox.setIcon( QMessageBox::Critical );

    #if DEBUG_GPIBPORT==1
        qDebug()<<"GPIB-PORT("+QString::number(this->getAddress())+"): void GPIBPort2::errorMessage("+errorType+"): " + errorType.toLocal8Bit().constData();
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

QString GPIBPort2::sreQuery(int instHandler)
{
    QString output = QString( "*SRE?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes.toString();
}

/**
  * @brief Reads Status Byte Register.
  *
  * @return An integer containing the Status Byte info.
  */

int GPIBPort2::stbQuery(int instHandler)
{
    QString output = QString( "*STB?" );

#ifndef TEST

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );
    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_DEFAULT,qVarRes);
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

QVariant GPIBPort2::esrQuery(int instHandler)
{
    QString output = QString( "*ESR?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief ......
  *
  * @return A QVariant containing the .....
  */

QVariant GPIBPort2::eseQuery(int instHandler)
{
    QString output = QString( "*ESE?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief Reads and resets Operation Event Register.
  *
  * @return A QVariant containing the Operation Event Register status previous reset.
  */

QVariant GPIBPort2::oerQuery(int instHandler)
{
    QString output = QString( ":STAT:OPER:EVEN?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief Reads and resets Measurement Event Register.
  *
  * @return A QVariant containing the Measurement Event Register status previous reset.
  */

QVariant GPIBPort2::merQuery(int instHandler)
{
    QString output = QString( ":STAT:MEAS:EVEN?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

/**
  * @brief Reads and resets Questionable Event Register.
  *
  * @return A QVariant containing the Questionable Event Register status previous reset.
  */

QVariant GPIBPort2::qerQuery(int instHandler)
{
    QString output = QString( ":STAT:QUES:EVEN?" );

    if( isNoError() ) write(instHandler,output.toLocal8Bit().data() );

    QVariant qVarRes;
    read(instHandler,READBUFFER_SIZE_MAX, qVarRes);
    return qVarRes;
}

int GPIBPort2::waitForResponse(int instHandler,int status_mask){return ibwait(instHandler,status_mask);}


/**
  * @brief Trigger and reads Status Byte Register.
  *
  * @return A Qstring cointaining the last error read from the error queue.
  */
/*
QString GPIBPort2::queueErrorQuery()
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
QVariant GPIBPort2::readQuery( const int size )
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
QVariant GPIBPort2::dataQuery( const int size )
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
QVariant GPIBPort2::fetchQuery( const int size )
{
    QString output = QString( ":FETC?" );
    qDebug() << output.toLocal8Bit().data();
    write(output);
    return read( size );
}
*/


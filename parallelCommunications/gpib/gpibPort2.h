#ifndef GPIBPORT2_H
#define GPIBPORT2_H

#include "../Communications/parallelCommunications/parallelport.h"
#include "./Instruments/keithley/sourceMeters/K24xxConfigurationParameters.h"
#include "../Tools/debug.h"
#include <QMessageBox>



#define BOARD_INDEX 0
#define SAD 0
#define READ_SIZE 100
#define DEFAULT_GPIBADDERSS_FOR_INSTRUMENTS 21

#define TIMEOUT_1s T1s
#define TIMEOUT_3s T3s
#define TIMEOUT_10s T10s
#define NEVERTIMEOUT TNONE

#define EOT 1

#define FORMAT_DATA_SIZE    14 * 3
/**
  * End Of String
  *  - 0 = none EOS
  *  - 0x140A = {0A} retorno de carro
  */
#define EOS 0

#ifndef TEST
    #ifdef  NI_PCI_GPIB
        #include <windows.h>
        #include "gpib/ni488.h"
    #elif   AD_GPIB
        #include "../GPIB/Adgpib.h"
        #include "../GPIB/gpib_user.h"
    #endif
#endif

class GPIBPort2: public ParallelPort{

    Q_OBJECT
public:

    static  void     closeConnection(int instHandler);
    static  void     disable(int instHandler);
    static  int      openConnection(int instAddress, int eos = 0 );
    static int      write(int instHandler,QString instruction);
    static int      write(int instHandler,char *instruction );
    static int      read (int instHandler,int size, QVariant &result);
    static int      read(int instHandler,char * message, int size );
    static int      reada(int instHandler,char * message, int size );
    static int      sendReadQueryAsCharArrayAsynchronously(int instHandler,char* message, const int bytesToRead);
    static int      sendReadQueryAndGetResultAsCharArray(int instHandler,char* message, int bytesToRead );
    static int      sendReadQueryAndGetResultAsString(int instHandler,int size, QString &result);

    static int      remoteEnable(int instHandler);
    static void     clearInstrument(int instHandler);

    static bool     isNoError();
    static void     setNoError(bool state);



    static int      getPrimaryBoardAddress();
    static int      getSecondaryBoardAddress();
    static int      setPrimaryBoardAddress(int pAddr);
    static int      setSecondaryBoardAddress(int sAddr);

    static int      setBoardTimeout (int timo);
    static int      setInstrumentTimeout (int instHandler, int timo);

    static int      testCommand(int instHandler,QString instruction);
    static int      allResgistersQueryTest(int instHandler);


    static int      waitForResponse(int instHandler,int status_mask);

    static int      executePolling(int instHandler,char *spr);

    static int      getNumberOfReadBytes();

    static int      stbQuery(int instHandler);
    static QString  sreQuery(int instHandler);
    static QVariant eseQuery(int instHandler);
    static QVariant esrQuery(int instHandler);
    static QVariant oerQuery(int instHandler);
    static QVariant merQuery(int instHandler);
    static QVariant qerQuery(int instHandler);

protected:
    static int      errors();
    static void     errorMessage( QString errorType );

signals:
    void            errorSignal();

private:
    static int      address;
    static bool     noError;
    static int      device;
    static int      boardPrimaryAddress;
    static int      boardSecondaryAddress;
    static QList<int>    *instrumentsList;
};

#endif // GPIBPORT2_H

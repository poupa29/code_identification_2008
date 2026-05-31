////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LivicRS232Timer_H
#define _Maps_LivicRS232Timer_H

// Includes maps sdk library header
//#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include "maps.hpp"
//#include "R:/maps/materiel/capteurs/portRS_232_422/MAPS_COMPort.h"
//#include <ntddk.h>

#ifndef EXCEPTION_X	
#define EXCEPTION_X
#ifdef _DEBUG
#define EXCEPTION_STRING_ALLOCATION static char __chaine__[500];
#define EXCEPTION(x)	{sprintf(__chaine__,"%s ; FILE : %s ; LINE : %d\n",(const char *)(x),__FILE__,__LINE__);throw(__chaine__);}
#define EXCEPTION_MAPS(x)	{sprintf(__chaine__,"%s ; FILE : %s ; LINE : %d\n",(const char *)(x),__FILE__,__LINE__);Error((const char *)__chaine__);}
#else
#define EXCEPTION(x)		throw((const char *)(x));
#define EXCEPTION_MAPS(x)	MAPS::ReportError((const char *)(x));
#endif
#endif

// Definitions
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


// Classes enumeration
class CommPortDefinition;
class CommPort;
class AddressDataField;
class CommPortMessage;
class BaudRateDefinition;

class AddressDataField
{
public :
	union AddressDataFieldUnion
	{
		class NamedField
		{
		public:
			char *name;
			unsigned int bufferLength;
		};
		class BitField 
		{
		public:
			unsigned char bits[8];
			unsigned char nbBits;
		};
		class ByteField
		{
		public:
			unsigned char *bytes;
			unsigned int nbBytes;
			unsigned int bufferLength;
		} ;
		public:
			NamedField namedField;
			unsigned int handle32;
			MAPSInt64 handle64;
			BitField bitField;
			ByteField byteField;
	} ;
	
//
	AddressDataFieldUnion value;
	int typeOfData;

// CONSTS
	static const int ConstNone;
	static const int ConstNamedField;
	static const int ConstHandle32Field;
	static const int ConstHandle64Field;
	static const int ConstBitField;
	static const int ConstByteField;

// Constructors
	// Default constructor
	AddressDataField() {typeOfData=ConstNone;memset(&value,0,sizeof(value));};
	// Constructor of a named field
	AddressDataField(const char *name,unsigned int bufferLength=0);
	// Constuctor of a 32 bit handle field
	AddressDataField(unsigned int val) {value.handle32=val;typeOfData=ConstHandle32Field;};
	// Constuctor of a 64 bit handle field
	AddressDataField(MAPSInt64 val) {value.handle64=val;typeOfData=ConstHandle64Field;};
	// Constructor of a 64 bits field
	AddressDataField(unsigned char nbBits,unsigned char bits[8]);
	// Constructor of a byte field
	AddressDataField(unsigned int bufferLength, unsigned int nbBytes, const unsigned char *bytes);
	// Copy constructor
	AddressDataField(const AddressDataField &adf);
	// Operator =
	AddressDataField & operator = (const AddressDataField &adf);

// Destructor
	~AddressDataField();

// Free all dynamically allocated memory
	void FreeAll();

// Allocation of memory methods
	// named field allocation
	bool Allocate(const char *name,unsigned int bufferLength=0);
	// Byte field allocation
	bool Allocate(unsigned int bufferLength, unsigned int nbBytes, const unsigned char *bytes);

// Set new content of field
	// Set the named field with a new content
	bool Set(const char *name);
	// Set 32 bit handle field 
	bool Set(unsigned int val);
	// Set 64 bit handle field
	bool Set(MAPSInt64 val);
	// Set bits field
	bool Set(unsigned char bits[8]);
	// Set the byte field with a new content
	bool Set(unsigned int nbBytes,const unsigned char *bytes);
};
/////////////////////////////

class CommPortMessage
{
public:
	AddressDataField	address;
	AddressDataField	data;
	MAPSTimestamp		timeStamp;

#ifdef WIN32
	HANDLE				event;
#endif

	CommPortMessage(AddressDataField &d,AddressDataField &a) {data=d;address=a;timeStamp=0;};
	AddressDataField	&Address() { return(address);};
	AddressDataField	&Data() { return(data);};
	MAPSTimestamp		&TimeStamp() {return(timeStamp);};

};

/////////////////////////////

class RealTimeDefinition
{
public :
	static const unsigned int ConstNoRTCapabilities;
	static const unsigned int ConstRTPriorityOnAddress;
	static const unsigned int ConstRTPriorityOnAddressIncreasing;
	static const unsigned int ConstRTPriorityOnAddressDecreasing;
	unsigned int		realTimeCaps;
	AddressDataField	highestPriorityAddress;
	AddressDataField	lowestPriorityAddress;
	float				maxLatencyFactor;			// Latency in microseconds if baudRate=1000000

	// Give the latency time for a baud rate value "baudRate"
	unsigned int		GetMaxLatency(unsigned int baudRate);

	// Constructor
	RealTimeDefinition(unsigned int realTimeCapsX,AddressDataField &hpa,
						AddressDataField &lpa, float maxLatencyFactorX=0);
};

/////////////////////////////

class BaudRateDefinition
{
public :
	static const unsigned int ConstDiscreteValues;
	static const unsigned int ConstContinuousValues;
	static const unsigned int ConstNotInitialized;

	union BaudRateValues
	{
		public:
		struct DiscreteBaudRateValues
		{
			unsigned int *values;
			unsigned int nbValues;
		};
		struct ContinuousBaudRateValues
		{
			unsigned int minValue;
			unsigned int maxValue;
		};
		DiscreteBaudRateValues		discrete;
		ContinuousBaudRateValues	continuous;
	};
	unsigned int typeOfValues;
	BaudRateValues values;
	unsigned int defaultValue;
	
	BaudRateDefinition();
	BaudRateDefinition(unsigned int min, unsigned int max, unsigned int defaultVal=0);
	BaudRateDefinition(unsigned int *vals, unsigned int nbvals, unsigned int defaultVal=0);
	BaudRateDefinition(const BaudRateDefinition *br);
	~BaudRateDefinition();
	BaudRateDefinition & operator = (const BaudRateDefinition &br);
	
	bool Set(unsigned int min, unsigned int max, unsigned int defaultVal=0);
	bool Set(unsigned int *vals, unsigned int nbvals, unsigned int defaultVal=0);

	bool Free();
};

/////////////////////////////

class CommPortDefinition
{
public :
	MAPSString			portName;
	BaudRateDefinition	baudRateDef;
	RealTimeDefinition	realTimeDef;

	// Constructor
	CommPortDefinition(const char *portNameX=NULL,const BaudRateDefinition *brDef=NULL,const RealTimeDefinition *rtDef=NULL);
	virtual ~CommPortDefinition() {};	// Must be virtual, because of inheritance
};

class CommPort// : public BaseModule
{
protected :

	//////////////////////////////
	// Port parameters definitions
	CommPortDefinition *definition;

	FILE *F_DEBUG;

	//////////////////////
	// Dynamic parameters

	// Timeouts
	int readTimeOut;	// read timeout (in milliseconds)
	int writeTimeOut;	// write timeout (in milliseconds)

public :
	// Constructor
	CommPort(const char *nameX,const CommPortDefinition *rcDef);

	// Destructor
	virtual ~CommPort();	// Must be virtual, because of inheritance

	////////////////////////////////////////////
	//////// Services
	////////////////////////////////////////////

	bool SwitchOnOffDebugMode(const char *fileName=NULL);
	void DebugOutput(const char *str);
	void DebugOutput(const char *str,int i);
	bool InDebugMode();

	////////////////////////////////////////////
	//////// To be implemented 
	////////////////////////////////////////////
	
	// Port initialisation, compulsory implementation
	virtual bool InitializePort()=0;

	// Close port, compulsory implementation
	virtual bool ClosePort()=0;

	// Synchronous write, compulsory implementation
	virtual bool Write(const CommPortMessage *message)=0;

	// Synchronous read, compulsory implementation
	virtual bool Read(CommPortMessage *message)=0;

	
	////////////////////////////////////////////
	/////// To be implemented according to caps
	////////////////////////////////////////////
	
	// Set the baud rate value (to the default value if value=0)
	virtual bool SetBaudRate(unsigned int value=0);

	// Set timeouts to default value
	virtual bool SetDefaultTimeouts() {readTimeOut=writeTimeOut=0;return(true);};

	// Set read timeout value
	virtual bool SetReadTimeout(int readTimeOutX) {readTimeOut=readTimeOutX;return(true);};

	// Get read timeout value
	virtual int GetReadTimeout() {return(readTimeOut);};

	// Set write timeout value
	virtual bool SetWriteTimeout(int writeTimeOutX) {writeTimeOut=writeTimeOutX;return(true);};
	
	// Get write timeout value
	virtual int GetWriteTimeout() {return(writeTimeOut);};

	// Set address value according to priority
	// 0 (lowest priority level) < priority < 255 (highest priority level)
	virtual bool GetAddressFromPriority(unsigned char priority,const AddressDataField *address);

	// Asynchronous write, signal event in message when done
	virtual bool AWrite(const CommPortMessage *message);

	// Asynchronous read, signal event in message when done
	virtual bool ARead(CommPortMessage *message);
};


class RS232PortDefinition : public CommPortDefinition
{
public :
	COMMTIMEOUTS	timeouts;
	COMMCONFIG		commConfig;
	DWORD			readQ,writeQ;
	static const DWORD ConstDefaultComReadQ;
	static const DWORD ConstDefaultComWriteQ;

	RS232PortDefinition(const char *portNameX, const BaudRateDefinition *brDef, 
										 DWORD DtrControl, 
										 DWORD RtsControl);

};

class RS232Port : public CommPort
{
private:

protected :
	CRITICAL_SECTION writeCs;
	CRITICAL_SECTION readCs;
	CRITICAL_SECTION interruptsCs;

	HANDLE handlePortCom;
	OVERLAPPED overlappedWrite,overlappedRead,overlappedInterrupts;
	bool overlappedIO;

	bool initialized;

	RS232PortDefinition *definition;	// Overloads *CommPort::definition

public:
	// Instantiation function of this module
	//static BaseModule *instFunct(const char *nameX,void *defStruct);
	//static void	unregistrationFunct(void *defStruct);
	//static void	PlugAndPlayRegistration();

	//static bool GetAvailableBaudRate(HANDLE handle, int *min, int *max, bool *continuous=NULL,unsigned int **values=NULL,int *nbValues=NULL);


	// Constructor 
	RS232Port(const char *nameX,const RS232PortDefinition *def);
	
	// Destructor
	virtual ~RS232Port();	// Must be virtual (it should already be, because of CommPort inheritance)

	// Port initialisation, compulsory implementation
	virtual bool InitializePort();

	// Close port, compulsory implementation
	virtual bool ClosePort();

	// Synchronous write, compulsory implementation
	virtual bool Write(const CommPortMessage *message);

	// Synchronous read, compulsory implementation
	virtual bool Read(CommPortMessage *message);

///////////////////////////////////////
	
	// Set the baud rate value (to the default value if value=0)
	virtual bool SetBaudRate(unsigned int value=0);

	// Set timeouts to default value
	virtual bool SetDefaultTimeouts();

	// Set read timeout value
	virtual bool SetReadTimeout(int readTimeOutX);

	// Get read timeout value
	virtual int GetReadTimeout();

	// Set write timeout value
	virtual bool SetWriteTimeout(int writeTimeOutX);
	
	// Get write timeout value
	virtual int GetWriteTimeout();

	// Set address value according to priority
	// 0 (lowest priority level) < priority < 255 (highest priority level)
	virtual bool GetAddressFromPriority(unsigned char priority,AddressDataField &address);

	// Asynchronous write, signal event in message when done
//	virtual bool AWrite(CommPortMessage &message);

	// Asynchronous read, signal event in message when done
//	virtual bool ARead(CommPortMessage &message);

///////////////////////////////////////
// Specific to RS232Port class
	
	void GetLastErrorString(MAPSString &str);
	bool SetReadTimeouts(int constant, int factor,int interval);
	bool SetWriteTimeouts(int constant, int factor);
	bool SetParity(int parity);
	bool SetInterrupts();
	bool SetQueuesLengths(int read=-1,int write=-1);
	bool GetQueuesState(DWORD *rQueue,DWORD *wQueue);
	bool CancelAllIoOperations();
	bool PurgeRx();
	bool PurgeTx();
	bool Reset();
	DWORD SendPacket(const unsigned char *bytes, DWORD nbBytesToWrite);
	DWORD GetPacket(unsigned char *bytes, DWORD nMaxLength );
	DWORD SendChar(unsigned char c);
	DWORD GetChar(unsigned char *c);
	DWORD WaitForInterrupts(int timeout);

};




//int *valeurThread;
//int *valeurTampon;

// Declares a new MAPSComponent child class
class MAPSLivicRS232Timer : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicRS232Timer)
    
    
private :
	// Place here your specific methods and attributes

    //MAPSCOMPort PortCom;
    RS232Port *comPort;
    MAPSTimestamp lastTime;
    int lastValeurThread;

    bool Initialize();


};


#endif

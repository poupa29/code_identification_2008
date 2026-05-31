////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicRS232Timer.h"	// Includes the header of this component

/*#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>*/

// Allocate the exception string table
#ifdef _DEBUG
EXCEPTION_STRING_ALLOCATION;
#endif

// Consts
const int AddressDataField::ConstNone=0;
const int AddressDataField::ConstNamedField=1;
const int AddressDataField::ConstHandle32Field=2;
const int AddressDataField::ConstHandle64Field=3;
const int AddressDataField::ConstBitField=4;
const int AddressDataField::ConstByteField=5;
const DWORD RS232PortDefinition::ConstDefaultComReadQ=8120;
const DWORD RS232PortDefinition::ConstDefaultComWriteQ=4096;

////////////////////////////////////////
// CLASS AddressDataField implementation
////////////////////////////////////////

AddressDataField::AddressDataField(const char *name,unsigned int bufferLength)
{
	typeOfData=ConstNone;	// We precise to Allocate method that no allocations 
							// have already been done
	if (!Allocate(name,bufferLength))
		throw("AddressDataField (NamedField) : cannot allocate buffers\n");
};

AddressDataField::AddressDataField(unsigned char nbBits,unsigned char bits[8])
{
	if (bits!=NULL)
	{
		memcpy(value.bitField.bits,bits,8);
	}
	else
		memset(&value.bitField,0,sizeof(value.bitField));
	value.bitField.nbBits=nbBits;
	typeOfData=ConstBitField;
}
AddressDataField::AddressDataField(unsigned int bufferLength, unsigned int nbBytes, const unsigned char *bytes)
{
	typeOfData=ConstNone;	// We precise to Allocate method that no allocations 
							// have already been done
	if (!Allocate(bufferLength,nbBytes,bytes))
		throw("AddressDataField (ByteField) : cannot allocate buffers\n");
}

AddressDataField::AddressDataField(const AddressDataField &adf)
{
	typeOfData=ConstNone;
	*this=adf;
}
AddressDataField & AddressDataField::operator = (const AddressDataField &adf)
{
	if (typeOfData!=ConstNone)
		FreeAll();
	typeOfData=adf.typeOfData;
	value=adf.value;
	switch(typeOfData)
	{
	case ConstNone:
		break;
	case ConstNamedField:
		// We have to instantiate another buffer and to copy adf one
		value.namedField.name=(char *)malloc(value.namedField.bufferLength);
		memcpy(value.namedField.name,adf.value.namedField.name,value.namedField.bufferLength);
		break;
	case ConstHandle32Field:
		break;
	case ConstHandle64Field:
		break;
	case ConstBitField:
		break;
	case ConstByteField:
		value.byteField.bytes=(unsigned char *)malloc(value.byteField.bufferLength);
		memcpy(value.byteField.bytes,adf.value.byteField.bytes,value.byteField.bufferLength);
		break;
	}
	return(*this);
}

AddressDataField::~AddressDataField()
{
	FreeAll();
}

bool AddressDataField::Allocate(const char *name,unsigned int bufferLength)
{
	if (typeOfData!=ConstNone)	// If some memory is already allocated
		FreeAll();				// free all
	typeOfData=ConstNamedField;

	if (bufferLength==0)
	{
		if (name==NULL)
		{
			OutputDebugString("AddressDataField error : name=NULL and bufferLength=0\n");
			return(false);
		}
		value.namedField.name=strdup(name);
		if (value.namedField.name==NULL)
		{
			OutputDebugString("Strdup error in AddressDataField : not enough memory\n");
			return(false);
		}
		value.namedField.bufferLength=strlen(name)+1;
	}
	else
	{
		value.namedField.name=(char *)malloc(bufferLength);
		if (value.namedField.name==NULL)
		{
			OutputDebugString("Malloc error in AddressDataField : not enough memory\n");
			return(false);
		}
		memset(value.namedField.name,0,bufferLength);
		memcpy(value.namedField.name,name,MIN(strlen(name),bufferLength));	// There is always a 0 terminating the string
		value.namedField.bufferLength=bufferLength;
	}
	return(true);
}

bool AddressDataField::Allocate(unsigned int bufferLength, unsigned int nbBytes, const unsigned char *bytes)
{
	if (typeOfData!=ConstNone)	// If some memory is already allocated
		FreeAll();				// free all
	typeOfData=ConstByteField;

	if (bufferLength<=0)
	{
		OutputDebugString("Error in AddressDataField : invalid length of buffer\n");
		return(false);
	}
	value.byteField.bytes=(unsigned char *)malloc (bufferLength);
	if (value.byteField.bytes==NULL)
	{
		OutputDebugString("Malloc error in AddressDataField (ByteField) : not enough memory\n");
		return(false);
	}
	value.byteField.bufferLength=bufferLength;
	if (bytes==NULL)
	{
		memset(value.byteField.bytes,0,bufferLength);
		value.byteField.nbBytes=0;
	}
	else
	{
		value.byteField.nbBytes=MIN(nbBytes,bufferLength);
		memcpy(value.byteField.bytes,bytes,value.byteField.nbBytes);
	}
	return(true);
}

void AddressDataField::FreeAll()
{
	switch(typeOfData)
	{
	case ConstNone:
		break;
	case ConstNamedField:
		free(value.namedField.name);
		value.namedField.name=NULL;
		break;
	case ConstHandle32Field:
		break;
	case ConstHandle64Field:
		break;
	case ConstBitField:
		break;
	case ConstByteField:
		free(value.byteField.bytes);
		value.byteField.bytes=NULL;
		break;
	}
	typeOfData=ConstNone;
}

// Set new content of field
// Set the named field with a new content
bool AddressDataField::Set(const char *name)
{
	unsigned int length=strlen(name)+1;
	if (typeOfData!=ConstNamedField)
		return(false);

	if (value.namedField.bufferLength<length)
		Allocate(name,length);
	else
		memcpy(value.namedField.name,name,length);
	return(true);
}
// Set 32 bit handle field 
bool AddressDataField::Set(unsigned int val)
{
#pragma message( "--> Set 32 bit handle field not implemented")
	return(false);
}
// Set 64 bit handle field
bool AddressDataField::Set(MAPSInt64 val)
{
#pragma message( "--> Set 64 bit handle field not implemented")
	return(false);
}
// Set bits field
bool AddressDataField::Set(unsigned char bits[8])
{
#pragma message( "--> Set bits field not implemented")
	return(false);
}
// Set the byte field with a new content
bool AddressDataField::Set(unsigned int nbBytes,const unsigned char *bytes)
{
#pragma message( "--> Set the byte field with a new content not implemented")
	return(false);
}

////////////////////////////////////////
// CLASS BaudRateDefinition implementation
////////////////////////////////////////
const unsigned int BaudRateDefinition::ConstDiscreteValues=0x1;
const unsigned int BaudRateDefinition::ConstContinuousValues=0x2;
const unsigned int BaudRateDefinition::ConstNotInitialized=0x4;

bool BaudRateDefinition::Set(unsigned int *vals, unsigned int nbvals, unsigned int defaultVal)
{
	if (typeOfValues!=ConstNotInitialized)
		Free();
	if (vals==NULL)
	{
		return(false);
		throw("Failed in initializing baud rate admissible values : NULL pointer\n");
	}
	if (nbvals==0)
	{
		return(false);
		throw("Failed in initializing baud rate admissible values : no values available\n");
	}
	if (defaultVal==0)
		defaultValue=vals[0];
	else
		defaultValue=defaultVal;
	values.discrete.nbValues=nbvals;
	values.discrete.values=(unsigned int *)malloc(nbvals*sizeof(unsigned int));
	if (values.discrete.values==NULL)
	{
		return(false);
		throw("Failed in initializing baud rate admissible values : out of memory\n");
	}
	memcpy(values.discrete.values,vals,nbvals*sizeof(unsigned int));
	typeOfValues=ConstDiscreteValues;
	return(true);
}
bool BaudRateDefinition::Set(unsigned int min, unsigned int max, unsigned int defaultVal)
{
	if (typeOfValues!=ConstNotInitialized)
		Free();
	if (min>max)
	{
		return(false);
		throw("Failed in initializing baud rate admissible values : min>max\n");
	}
	if (defaultVal==0)
		defaultValue=max;
	else
		defaultValue=defaultVal;
	values.continuous.minValue=min;
	values.continuous.maxValue=max;
	typeOfValues=ConstContinuousValues;
	return(true);
}

bool BaudRateDefinition::Free()
{
	switch(typeOfValues)
	{
	case ConstDiscreteValues:
		free(values.discrete.values);
		break;
	default:
		break;
	}
	typeOfValues=ConstNotInitialized;
	return(true);
}

BaudRateDefinition::BaudRateDefinition(unsigned int min, unsigned int max, unsigned int defaultVal)
{
	Set(min,max,defaultVal);
}

BaudRateDefinition::BaudRateDefinition(unsigned int *vals, unsigned int nbvals, unsigned int defaultVal)
{
	Set(vals,nbvals,defaultVal);
}

BaudRateDefinition::BaudRateDefinition()
{
	typeOfValues=ConstNotInitialized;
}

BaudRateDefinition::~BaudRateDefinition()
{
	Free();
}
BaudRateDefinition::BaudRateDefinition(const BaudRateDefinition *br)
{
	if (br!=NULL)
		*this=*br;
	else
		typeOfValues=ConstNotInitialized;
}

BaudRateDefinition & BaudRateDefinition::operator = (const BaudRateDefinition &br)
{
	defaultValue=br.defaultValue;
	typeOfValues=br.typeOfValues;
	values=br.values;
	switch(typeOfValues)
	{
	case ConstDiscreteValues:
		values.discrete.values=(unsigned int *)malloc(values.discrete.nbValues*sizeof(unsigned int));
		memcpy(values.discrete.values,br.values.discrete.values,values.discrete.nbValues*sizeof(unsigned int));
		break;
		
	case ConstContinuousValues:
		break;
	}
	return(*this);
}

////////////////////////////////////////
// CLASS RealTimeDefinition implementation
////////////////////////////////////////

const unsigned int RealTimeDefinition::ConstNoRTCapabilities=0;
const unsigned int RealTimeDefinition::ConstRTPriorityOnAddress=0x1;
const unsigned int RealTimeDefinition::ConstRTPriorityOnAddressIncreasing=0x2;
const unsigned int RealTimeDefinition::ConstRTPriorityOnAddressDecreasing=0x4;

// Give the latency time for a baud rate value "baudRate"
unsigned int RealTimeDefinition::GetMaxLatency(unsigned int baudRate)
{
	return((unsigned int)(1000000*maxLatencyFactor/baudRate));
}

// Constructors
RealTimeDefinition::RealTimeDefinition(unsigned int realTimeCapsX,AddressDataField &hpa,
									   AddressDataField &lpa, float maxLatencyFactorX) 
	: highestPriorityAddress(hpa),lowestPriorityAddress(lpa), maxLatencyFactor(maxLatencyFactorX),
		realTimeCaps(realTimeCapsX)
{
	if (hpa.typeOfData!=lpa.typeOfData)
		throw("RealTimeDefinition : Incompatible type of addresses\n");
}

////////////////////////////////////////
// CLASS CommPortDefinition implementation
////////////////////////////////////////

CommPortDefinition::CommPortDefinition(const char *portNameX,const BaudRateDefinition *brDef,const RealTimeDefinition *rtDef)
: realTimeDef(RealTimeDefinition::ConstNoRTCapabilities,AddressDataField(),AddressDataField())
{
	if (portNameX!=NULL)
		portName=portNameX;
	if (brDef!=NULL)
		baudRateDef=*brDef;
	if (rtDef!=NULL)
		realTimeDef=*rtDef;
}


////////////////////////////////////////
// CLASS CommPort implementation
////////////////////////////////////////

CommPort::CommPort(const char *nameX,const CommPortDefinition *rcDef)// : BaseModule(nameX)
{
	readTimeOut=100;
	writeTimeOut=100;
	if (rcDef!=NULL)	// If the definition structure is valid then copy it
		definition=new CommPortDefinition(*rcDef);	
	else
		definition=NULL;// else it is not defined

	F_DEBUG=NULL;		// For the debug file
}

CommPort::~CommPort()
{
	if (F_DEBUG!=NULL)
		SwitchOnOffDebugMode();

	if (definition)
	{
		delete definition;
		definition=NULL;
	}
}

//////////////////// SERVICES

bool CommPort::SwitchOnOffDebugMode(const char *fileName)
{
	MAPSString theFileName;
	if (fileName==NULL)
	{
		if (F_DEBUG!=NULL)
		{
			DebugOutput("End of debug session");
			fclose(F_DEBUG);
			F_DEBUG=NULL;
			return(false);
		}
		theFileName="Debug_";
		theFileName+="CommPort";
		theFileName+=".txt";
	}
	else
		theFileName=fileName;
	if (F_DEBUG==NULL)
	{
//		F_DEBUG=fopen(fileName,"at");
		F_DEBUG=fopen(theFileName,"wt");	
	}
	else
	{
		DebugOutput("End of debug session (because of a start of a new session)");
		fclose(F_DEBUG);
		F_DEBUG=fopen(theFileName,"wt");	
	}
	DebugOutput("////////////////////////////////////////////");
	DebugOutput("New debug session");
	DebugOutput("////////////////////////////////////////////");
	return(true);
}

void CommPort::DebugOutput(const char *str)
{
	if (F_DEBUG)
		fprintf(F_DEBUG,"* %d * %s\n",timeGetTime(),str);
}

void CommPort::DebugOutput(const char *str,int i)
{
	if (F_DEBUG)
		fprintf(F_DEBUG,"* %d * %s : %d\n",timeGetTime(),str,i);
}

bool CommPort::InDebugMode()
{
	if (F_DEBUG)
		return(true);
	else
		return(false);
}
//////////////////// Method that must be implemented according to caps

bool CommPort::SetBaudRate(unsigned int value)
{
	return(false);
}

// If the CommPort has RT capabilities,
// Get address value according to priority
// 0 (lowest priority level) < priority < 255 (highest priority level)
bool CommPort::GetAddressFromPriority(unsigned char priority,const AddressDataField *address)
{
	return(false);
}

// Asynchronous write, signal event in message when done
bool CommPort::AWrite(const CommPortMessage *message)
{
	return(false);
}

// Asynchronous read, signal event in message when done
bool CommPort::ARead(CommPortMessage *message)
{
	return(false);
}

//*********************************************************************************************
//RS232PortDefinition CLASS implementation
//*********************************************************************************************
RS232PortDefinition::RS232PortDefinition(const char *portNameX, const BaudRateDefinition *brDef, 
										 DWORD DtrControl = DTR_CONTROL_DISABLE, 
										 DWORD RtsControl = RTS_CONTROL_DISABLE)
{
	if (portNameX!=NULL)
		portName=portNameX;
	if (brDef!=NULL)
		baudRateDef=*brDef;

	DWORD size;
	DCB &dcb=commConfig.dcb;

	memset(&commConfig,0,sizeof(commConfig));
	commConfig.dwSize=sizeof(commConfig);
	size=commConfig.dwSize;
	if ((const char *)portName!=NULL)
		GetDefaultCommConfig(portName,&commConfig,&size);

	memset(&timeouts,0,sizeof(timeouts));
	
	timeouts.ReadIntervalTimeout=MAXDWORD;

	dcb.DCBlength=sizeof(dcb);
	dcb.BaudRate=baudRateDef.defaultValue;
	dcb.ByteSize=8;	  	// 8 bits
	dcb.StopBits=ONESTOPBIT;	// 1 stop bit length.
//	dcb.fAbortOnError=TRUE;
	dcb.fAbortOnError=FALSE;
	dcb.fBinary=TRUE;
	dcb.Parity=NOPARITY;
	if (dcb.Parity!=NOPARITY)
		dcb.fParity=TRUE;
	else
		dcb.fParity=FALSE;
	dcb.fDtrControl = DtrControl;
	dcb.fRtsControl = RtsControl;
	dcb.fOutxCtsFlow=FALSE;
	dcb.fOutxDsrFlow=FALSE;
	dcb.fInX=FALSE;
	dcb.fOutX=FALSE;

	readQ=ConstDefaultComReadQ;
	writeQ=ConstDefaultComWriteQ;
}

/////////////////////////////////////////////////
// Class RS232Port
/////////////////////////////////////////////////
/*
// Instantiation function of this module
BaseModule *RS232Port::instFunct(const char *nameX,void *defStruct)
{
	return(new RS232Port(nameX,(RS232PortDefinition *)defStruct));
}

void	RS232Port::unregistrationFunct(void *defStruct)
{
	delete ((RS232PortDefinition *)defStruct);
}

void	RS232Port::PlugAndPlayRegistration()
{
	int i=0;
	int successiveFailure=0;
	char tempStr[100];
	HANDLE handle;

	// Try to open COMi from i=0 : stop when 3 successive i failed, and i > 16
	while(1)
	{
		sprintf(tempStr,"COM%d",i);
		// Try to open COMi
		if ((handle=CreateFile(tempStr,GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL , NULL))==(HANDLE)-1)
		{
			// Failure
			successiveFailure++;
			if ((successiveFailure>=3)&&(i>32))
				break;	// Third successive failure : exit
		}
		else
		{
			// Success
			int min,max,nbValues;
			unsigned int *tab;
			bool continuous;

			// We retrieve all available baudrate
			if (!GetAvailableBaudRate(handle,&min,&max,&continuous,&tab,&nbValues))
				EXCEPTION("RS232Port : Error while retriving COM properties\n");

			if (continuous)
			{
				BaudRateDefinition br(min,max);
				RS232PortDefinition *defaultDefinition=new RS232PortDefinition(tempStr,&br);
				// We declare a new port
				NEW_MODULE_DEFINITION_X(tempStr,"RS232Port",RS232Port::instFunct,defaultDefinition,NULL,RS232Port::unregistrationFunct);
			}
			else
			{
				BaudRateDefinition br(tab,nbValues);
				RS232PortDefinition *defaultDefinition=new RS232PortDefinition(tempStr,&br);
				// We declare a new port
				NEW_MODULE_DEFINITION_X(tempStr,"RS232Port",RS232Port::instFunct,defaultDefinition,NULL,RS232Port::unregistrationFunct);
			}
			// Let's close the port and continue
			CloseHandle(handle);
			free(tab);
			// Reset counter of succesive failures
			successiveFailure=0;
		}
		i++;
	};
}
*/


RS232Port::RS232Port(const char *nameX,const RS232PortDefinition *def) : CommPort(nameX,NULL)
{
	overlappedIO=true;
	memset(&overlappedRead,0,sizeof(overlappedRead));
	memset(&overlappedWrite,0,sizeof(overlappedWrite));
	memset(&overlappedInterrupts,0,sizeof(overlappedInterrupts));

	//SwitchOnOffDebugMode();

	initialized=false;

	if (def!=NULL)
	{
		CommPort::definition=definition=new RS232PortDefinition(*def);

		// TODO : Remove this call to InitializePort because it is a virtual function and is called in the constructor
		if (!InitializePort())
			EXCEPTION("The RS232 communication port initialisation failed : please check your configuration features");
	}
	else
		definition=NULL;

	InitializeCriticalSection(&writeCs);	
	InitializeCriticalSection(&readCs);	
	InitializeCriticalSection(&interruptsCs);	
}

RS232Port::~RS232Port()
{
	
	DebugOutput("~RS232Port()");

	// TODO : Remove this call to InitializePort because it is a virtual function and is called in the constructor
	ClosePort();

	if (definition!=NULL)
		delete definition;

	CommPort::definition=definition=NULL;

	DeleteCriticalSection (&writeCs);
	DeleteCriticalSection (&readCs);
	DeleteCriticalSection (&interruptsCs);
}

/////
// Port initialisation, compulsory implementation
bool RS232Port::InitializePort()
{
	DWORD flags=FILE_ATTRIBUTE_NORMAL;

	DebugOutput("InitializePort()");

	if (initialized)
		ClosePort();
	if (overlappedIO)
	{
		overlappedWrite.Offset=0;
		overlappedWrite.OffsetHigh=0;
		overlappedWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
		
		overlappedRead.Offset=0;
		overlappedRead.OffsetHigh=0;
		overlappedRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

		overlappedInterrupts.Offset=0;
		overlappedInterrupts.OffsetHigh=0;
		overlappedInterrupts.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

		flags|=FILE_FLAG_OVERLAPPED;
	}
	
	if ((handlePortCom=CreateFile(definition->portName,GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,
		flags, NULL))==(HANDLE)-1)
	{
		MAPSString errStr;
		this->GetLastErrorString(errStr);
		MAPS::ReportError(errStr);
		char str[100];
		sprintf(str,"Cannot open COM port : %s",(const char *)definition->portName);
		DebugOutput(str);
		ClosePort();
		return(initialized=false);
	//	EXCEPTION("Je n'ai pas reussi a ouvrir le port serie en question");
	}
	
	initialized=true;	// OK, handle is valid and initialisation is in progress
	
	if (!SetCommState(handlePortCom,&definition->commConfig.dcb))
	{
		MAPSString str;
		GetLastErrorString(str);
		DebugOutput(str+"(InitializePort:SetCommState)");
	
		ClosePort();
		return(initialized=false);
	}
	
	if (!SetQueuesLengths())
	{
		ClosePort();
		return(initialized=false);
	}
	if (!(PurgeRx()&&PurgeTx()))
	{
		DebugOutput("Cannot purge COM ports");

		ClosePort();
		return(initialized=false);
	}
	
	if (!SetDefaultTimeouts())
	{
		ClosePort();
		return(initialized=false);
	}
	
	if (!SetInterrupts())
	{
		ClosePort();
		return(initialized=false);
	}

	return(initialized=true);
};

// Close port, compulsory implementation
bool RS232Port::ClosePort()
{
	DebugOutput("ClosePort()");

	if (handlePortCom)
	{
		DebugOutput("Close serial port handle");
		SetCommMask(handlePortCom,0);
		EscapeCommFunction(handlePortCom,CLRDTR);
		PurgeComm(handlePortCom,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(handlePortCom);
		handlePortCom=NULL;
	}
	if (overlappedWrite.hEvent)
	{
		DebugOutput("Close write overlapped event handle");

		CloseHandle(overlappedWrite.hEvent);
		overlappedWrite.hEvent=NULL;
	}
	if (overlappedRead.hEvent)
	{
		DebugOutput("Close read overlapped event handle");
		CloseHandle(overlappedRead.hEvent);
		overlappedRead.hEvent=NULL;
	}

	if (overlappedInterrupts.hEvent)
	{
		DebugOutput("Close interrupts overlapped event handle");
		CloseHandle(overlappedInterrupts.hEvent);
		overlappedInterrupts.hEvent=NULL;
	}
	initialized=false;
	return(true);
};

// Synchronous write, compulsory implementation
bool RS232Port::Write(const CommPortMessage *message)
{
	DebugOutput("Write(CommPortMessage &message)");

	if (message->address.typeOfData!=AddressDataField::ConstNone)
		return(false);
	if (message->data.typeOfData==AddressDataField::ConstByteField)
	{
		if (SendPacket(message->data.value.byteField.bytes,message->data.value.byteField.nbBytes)==message->data.value.byteField.nbBytes)
			return(true);
		else
			return(false);
	}
	if (message->data.typeOfData==AddressDataField::ConstNamedField)
	{
		unsigned char *nf=(unsigned char *)message->data.value.namedField.name;
		unsigned int lengthOfField=strlen(message->data.value.namedField.name);
		if (SendPacket(nf,lengthOfField)==lengthOfField)
			return(true);
		else
			return(false);
	}
	return(false);
};

// Synchronous read, compulsory implementation
bool RS232Port::Read(CommPortMessage *message)
{
	DebugOutput("Read(CommPortMessage &message)");

	if (message->address.typeOfData!=message->address.ConstNone)
		return(false);
	return(true);
};

// Set the baud rate value (to the default value if value=0)
bool RS232Port::SetBaudRate(unsigned int value)
{
	DebugOutput("SetBaudRate(unsigned int value)",value);

	definition->commConfig.dcb.BaudRate=value;
	if (initialized)
	{
		EnterCriticalSection(&readCs);
		EnterCriticalSection(&writeCs);
		EnterCriticalSection(&interruptsCs);
		bool returnValue=(SetCommState(handlePortCom,&definition->commConfig.dcb)!=0);
		LeaveCriticalSection(&interruptsCs);
		LeaveCriticalSection(&writeCs);
		LeaveCriticalSection(&readCs);

		if (returnValue)
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(SetBaudRate:SetCommState)");
			return(false);
		}
	}
	else
	{
		DebugOutput("Trying to set baudrate while port isn't initialized");
		return(false);
	}
};

// Set read timeout value
bool RS232Port::SetReadTimeout(int readTimeOutX)
{
	DebugOutput("SetReadTimeout(readTimeOutX)",readTimeOutX);
	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		if ((timeouts.ReadTotalTimeoutConstant!=(DWORD)readTimeOutX)||(timeouts.ReadIntervalTimeout!=0)||(timeouts.ReadTotalTimeoutMultiplier!=0))
		{
			CommPort::SetReadTimeout(readTimeOutX);
			timeouts.ReadTotalTimeoutConstant=readTimeOutX;
			timeouts.ReadIntervalTimeout=0;
			timeouts.ReadTotalTimeoutMultiplier=0;
			if (::SetCommTimeouts(handlePortCom,&timeouts))
				return(true);
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(SetReadTimeout:SetCommTimeouts)");
				return(false);
			}
		}
		else
			return(true);
	}
	return(false);
};

// Get read timeout value
int RS232Port::GetReadTimeout()
{
	DebugOutput("GetReadTimeout()");

	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		if (::GetCommTimeouts(handlePortCom,&timeouts))
			return(timeouts.ReadTotalTimeoutConstant);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(GetReadTimeout:GetCommTimeouts)");
			return(-1);
		}
	}
	return(-1);
};

// Set write timeout value
bool RS232Port::SetWriteTimeout(int writeTimeOutX)
{
	DebugOutput("SetWriteTimeout(int writeTimeOutX)",writeTimeOutX);
	
	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		if ((timeouts.WriteTotalTimeoutConstant!=(DWORD)writeTimeOutX)||(timeouts.WriteTotalTimeoutMultiplier!=0))
		{
			CommPort::SetWriteTimeout(writeTimeOutX);
			timeouts.WriteTotalTimeoutConstant=writeTimeOutX;
			timeouts.WriteTotalTimeoutMultiplier=0;
			if (::SetCommTimeouts(handlePortCom,&timeouts))
				return(true);
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(SetWriteTimeout:SetCommTimeouts)");
				return(false);
			}
		}
		else
			return(true);
	}
	else
		return(false);
};

// Get write timeout value
int RS232Port::GetWriteTimeout()
{
	DebugOutput("GetQueuesState()");

	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		if (::GetCommTimeouts(handlePortCom,&timeouts))
			return(timeouts.WriteTotalTimeoutConstant);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(GetWriteTimeout:GetCommTimeouts)");
			return(-1);
		}
	}
	return(-1);
};

bool RS232Port::SetDefaultTimeouts()
{
	DebugOutput("SetDefaultTimeout()");
	
	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		memset(&timeouts,0,sizeof(timeouts));
		timeouts.ReadIntervalTimeout=MAXDWORD;
		if (::SetCommTimeouts(handlePortCom,&timeouts))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(SetDefaultTimeout:SetCommTimeouts)");
			return(false);
		}
	}
	else
		return(false);
};

// Set address value according to priority
// 0 (lowest priority level) < priority < 255 (highest priority level)
bool RS232Port::GetAddressFromPriority(unsigned char priority,AddressDataField &address)
{
	DebugOutput("GetAddressFromPriority()");
	return(false);
};

///////////////////////////////////////
// Specific to RS232Port class
///////////////////////////////////////

void RS232Port::GetLastErrorString(MAPSString &str)
{
	DWORD errorIO=GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorIO,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	str=(char *)lpMsgBuf;
	LocalFree( lpMsgBuf );
}

bool RS232Port::GetQueuesState(DWORD *rQueue,DWORD *wQueue)
{
	DebugOutput("GetQueuesState()");

	COMSTAT comStat;
	DWORD dwErrors;
	if (!ClearCommError(handlePortCom,&dwErrors,&comStat))
	{
		MAPSString str;
		GetLastErrorString(str);
		DebugOutput(str+"(GetQueuesState:ClearCommError)");
		return(false);
	}
	if (rQueue)
		*rQueue=comStat.cbInQue;
	if (wQueue)
		*wQueue=comStat.cbOutQue;
	{
		if (dwErrors&CE_OVERRUN)
			DebugOutput("Over-run error");
		if (dwErrors&CE_RXOVER)
			DebugOutput("Receive buffer overflow");
		if (dwErrors&CE_TXFULL)
			DebugOutput("Transmit buffer is full");

		char str[100];
		sprintf(str,"rQueue=%d,wQueue=%d",comStat.cbInQue,comStat.cbOutQue);
		DebugOutput(str);
	}
	return(true);
}


bool RS232Port::SetReadTimeouts( int constant, int factor,int interval)
{
		char str[100];
		sprintf(str,"SetReadTimeouts(constant=%d,factor=%d,interval=%d)",constant,factor,interval);
		DebugOutput(str);

	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		if ((timeouts.ReadTotalTimeoutConstant!=(DWORD)constant)||(timeouts.ReadIntervalTimeout!=(DWORD)interval)||(timeouts.ReadTotalTimeoutMultiplier!=(DWORD)factor))
		{
			CommPort::SetReadTimeout(interval);
			timeouts.ReadTotalTimeoutConstant=constant;
			timeouts.ReadIntervalTimeout=interval;
			timeouts.ReadTotalTimeoutMultiplier=factor;
			if(::SetCommTimeouts(handlePortCom,&timeouts))
				return(true);
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(SetReadTimeouts:SetCommTimeouts)");
			}
		}
		else
			return(true);
	}
	return(false);
}

bool RS232Port::SetWriteTimeouts(int constant, int factor)

{
	char str[100];
	sprintf(str,"SetWriteTimeouts(constant=%d,factor=%d)",constant,factor);
	DebugOutput(str);
	
	COMMTIMEOUTS &timeouts=definition->timeouts;
	if (initialized)
	{
		if ((timeouts.WriteTotalTimeoutConstant!=(DWORD)constant)||(timeouts.WriteTotalTimeoutMultiplier!=(DWORD)factor))
		{
			CommPort::SetWriteTimeout(constant);
			timeouts.WriteTotalTimeoutConstant=constant;
			timeouts.WriteTotalTimeoutMultiplier=factor;
			if(::SetCommTimeouts(handlePortCom,&timeouts))
				return(true);
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(SetWriteTimeouts:SetCommTimeouts)");
			}
		}
		else
			return(true);
	}
	return(false);
}

DWORD RS232Port::SendPacket(const unsigned char *bytes, DWORD nbBytesToWrite)
{
	DWORD dwBytesWritten;

	if (InDebugMode())
	{
		char debugBuffer[1000];
		DWORD pos=0,i=0;
		sprintf(debugBuffer,"* %d * SendPacket(bytes,nbBytesToWrite=%d)\n",timeGetTime(),nbBytesToWrite);
		pos=strlen(debugBuffer);
		while((pos<sizeof(debugBuffer)-5)&&(i<nbBytesToWrite))
		{
			sprintf(debugBuffer+pos," %2.2x",bytes[i]);
			i++;
			pos+=3;
		}
		sprintf(debugBuffer+pos,"\n");
		DebugOutput(debugBuffer);
	}


	EnterCriticalSection(&writeCs);

	while(1)
	{
		if (!overlappedIO)
		{
			if(WriteFile(handlePortCom,bytes,nbBytesToWrite,&dwBytesWritten,NULL))
				break;
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(SendPacket : WriteFile failed, error while trying to write to COM port)");
//				throw((const char *)str);
			}
		}
		else
		{
		
			WriteFile(handlePortCom,bytes,nbBytesToWrite,&dwBytesWritten,&overlappedWrite);
			if (GetOverlappedResult(handlePortCom,&overlappedWrite,&dwBytesWritten, TRUE))
			{
				ResetEvent(overlappedWrite.hEvent);
				break;
			}
		}
		DWORD errorIO=GetLastError();
		if (errorIO==ERROR_OPERATION_ABORTED)
		{
			DebugOutput("Error while trying to write to COM port : ERROR_IO_ABORTED");
			DWORD wQ,rQ;
			GetQueuesState(&rQ,&wQ); // Clear the error
		}
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(SendPacket : Error while trying to write to COM port)");
			throw((const char *)str);
		}
	}

	LeaveCriticalSection(&writeCs);

	DebugOutput("nbBytesWritten=",dwBytesWritten);
	return dwBytesWritten;

}

DWORD RS232Port::GetPacket(unsigned char *bytes, DWORD nMaxLength )
{
	DWORD      dwLength;

	DebugOutput("GetPacket(bytes,nMaxLength) nMaxLength=",nMaxLength);

	EnterCriticalSection(&readCs);

	while(1)
	{
		if (!overlappedIO)
		{
			if(ReadFile( handlePortCom, bytes,	nMaxLength, &dwLength, NULL))
				break;
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(GetPacket:ReadFile (Timeout ?))");
			}
		}
		else
		{
		
			ReadFile( handlePortCom, bytes,	nMaxLength, &dwLength, &overlappedRead );
			if (GetOverlappedResult(handlePortCom,&overlappedRead,&dwLength,true))
			{
				ResetEvent(overlappedRead.hEvent);
				break;
			}
			else
			{
				MAPSString str;
				GetLastErrorString(str);
				DebugOutput(str+"(GetPacket:GetOverlappedResult)");
			}
		}
		DWORD errorIO=GetLastError();
		if (errorIO==ERROR_OPERATION_ABORTED)
		{
			DebugOutput("Error while trying to read from COM port : ERROR_IO_ABORTED");
			DWORD wQ,rQ;
			GetQueuesState(&rQ,&wQ); // Clear the error
		}
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(GetPacket : Error while trying to read from COM port)");
			throw((const char *)str);
		}
	}

	LeaveCriticalSection(&readCs);

	if (InDebugMode())
	{
		char debugBuffer[1000];
		DWORD pos=0,i=0;
		sprintf(debugBuffer,"- %d - nbBytesRead=%d\n",timeGetTime(),dwLength);
		pos=strlen(debugBuffer);
		while((pos<sizeof(debugBuffer)-5)&&(i<dwLength))
		{
			sprintf(debugBuffer+pos," %2.2x",bytes[i]);
			i++;
			pos+=3;
		}
		sprintf(debugBuffer+pos,"\n");
		DebugOutput(debugBuffer);
		GetQueuesState(NULL,NULL);
	}

	return(dwLength);

}

DWORD RS232Port::WaitForInterrupts(int timeout)
{
	DWORD evtMask;
	DebugOutput("WaitForInterrupts");
	EnterCriticalSection(&readCs);
	if (!overlappedIO)
	{
		if (!WaitCommEvent(handlePortCom,&evtMask,NULL))
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(WaitForInterrupts:WaitCommEvent)");
		}
		LeaveCriticalSection(&readCs);
		return(evtMask);
	}
	else
	{
		WaitCommEvent(handlePortCom,&evtMask,&overlappedInterrupts);
		DWORD happy=WaitForSingleObject(overlappedInterrupts.hEvent,timeout);
		switch(happy)
		{
		case WAIT_OBJECT_0 :
			break;
		case WAIT_TIMEOUT :
			SetInterrupts();
			break;
		}
		DWORD dummy;
		if (GetOverlappedResult(handlePortCom,&overlappedInterrupts,&dummy,true))
			ResetEvent(overlappedInterrupts.hEvent);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(WaitForInterrupts:GetOverlappedResult)");
		}

		LeaveCriticalSection(&readCs);
		return(evtMask);
	}
}

DWORD RS232Port::SendChar(unsigned char c)
{
	return SendPacket(&c,1);
}

DWORD RS232Port::GetChar(unsigned char *c)
{
	return(GetPacket(c,1));
}

bool RS232Port::CancelAllIoOperations()
{
	DebugOutput("()");

	if (initialized)
	{
		GetQueuesState(NULL,NULL);
		if(PurgeComm(handlePortCom,PURGE_RXABORT|PURGE_TXABORT))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(CancelAllIoOperations:PurgeComm)");
		}
	}
	return false;
}

bool RS232Port::PurgeRx()
{
	DebugOutput("PurgeRx()");

	if (initialized)
	{
		GetQueuesState(NULL,NULL);
		if(PurgeComm(handlePortCom,PURGE_RXCLEAR))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(PurgeRx:PurgeComm)");
		}
	}			
	return false;
}

bool RS232Port::PurgeTx()
{
	DebugOutput("PurgeTx()");

	if (initialized)
	{
		GetQueuesState(NULL,NULL);
		if(PurgeComm(handlePortCom,PURGE_TXCLEAR))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(PurgeTx:PurgeComm)");
		}
	}
	return false;
}

bool RS232Port::SetQueuesLengths(int read,int write)
{
	char str[100];
	sprintf(str,"SetQueuesLengths(read=%d,write=%d)",read,write);
	DebugOutput(str);

	if (initialized)
	{
		GetQueuesState(NULL,NULL);
		if (read>=0)
			definition->readQ=(DWORD)read;
		if (write>=0)
			definition->writeQ=(DWORD)write;
		if(SetupComm(handlePortCom,definition->readQ,definition->writeQ))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(SetQueuesLengths:SetupComm)");
		}
	}
	return(false);
}

bool RS232Port::SetParity(int parity)
{
	DebugOutput("SetParity(parity)",parity);

	if (initialized)
	{
		DCB &dcb=definition->commConfig.dcb;
		dcb.Parity=parity;
		if (dcb.Parity!=NOPARITY)
			dcb.fParity=TRUE;
		else
			dcb.fParity=FALSE;
		if (SetCommState(handlePortCom,&definition->commConfig.dcb))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(SetParity:SetCommState)");
		}
	}
	return(false);
}

bool RS232Port::SetInterrupts()
{
	DebugOutput("SetInterrupts()");

	if (initialized)
	{
		if (SetCommMask(handlePortCom,EV_RING))
			return(true);
		else
		{
			MAPSString str;
			GetLastErrorString(str);
			DebugOutput(str+"(SetInterrupts:SetCommMask)");
		}
	}
	return(false);
}

//#define PORT1 0x3F8  /* Port Address Goes Here */
//#define INTVECT 0x0C /* Com Port's IRQ here (Must also change PIC setting) */
  /* Defines Serial Ports Base Address */
  /* COM1 0x3F8                        */
  /* COM2 0x2F8			       */
  /* COM3 0x3E8			       */
  /* COM4 0x2E8			       */




// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicRS232Timer)
    //MAPS_INPUT("iName",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicRS232Timer)
    //MAPS_OUTPUT("oName",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicRS232Timer)
    MAPS_PROPERTY("pNombreCycle",false,false,false)
    MAPS_PROPERTY("port",(char*)NULL,true,false)
	MAPS_PROPERTY("baudrate",9600,false,true)
	MAPS_PROPERTY("pControl",false,false,false)
	MAPS_PROPERTY("parity",0,false,false)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicRS232Timer)
    //MAPS_ACTION("aName",MAPSLivicRS232Timer::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicRS232Timer) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicRS232Timer,"LivicRS232Timer","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  0, // Nb of inputs
			  0, // Nb of outputs
			  5, // Nb of properties
			  0) // Nb of actions


int *valeurThread;
int *valeurTampon;

/*void TamponValeurThread()
{
    *valeurTampon = *valeurThread;
}*/

void MAPSLivicRS232Timer::Birth()
{
    // Reports this information to the RTMaps console
    ReportInfo("LivicRS232Timer: Passing through Birth() method");

    valeurThread = new int();
    *valeurThread = 0;
    valeurTampon = new int();
    *valeurTampon = 0;

    comPort=NULL;
	//useStreamOutput=GetBoolProperty("useStreamOutput");
	//useTerminalOutput=GetBoolProperty("useTerminalOutput");
	//maxsizeOfReadPacket=(int)GetIntegerProperty("maxsizeOfReadPacket");
	/*if (maxsizeOfReadPacket>ConstBufferSize)
	{
		MAPSStreamedString str;
		str<<"maxsizeOfReadPacket property value ("<<maxsizeOfReadPacket<<") is too large, set it to its max. value : "<<ConstBufferSize;
		ReportWarning(str,0);
		maxsizeOfReadPacket=ConstBufferSize;
	}*/
    if (!Initialize())
		Error("Cannot initialize COM port !");
	
	//comPort->SetReadTimeouts(500,0,1);
    
	
	//useRingOutput=GetBoolProperty("useRingOutput");
//	if (!useRingOutput)
//		CreateThread((MAPSComponentThreadFunction)ReadProc);

//		CreateThread((MAPSComponentThreadFunction)RingProc);

//	inputs[0]=&Input("streamInput");
//	inputs[1]=&Input("terminalInput");

    if (comPort->WaitForInterrupts(1000))
        ReportWarning("Signal detecté sur port COM");
    else
        ReportError("Signal non detecté sur port COM");
    lastTime = MAPS::CurrentTime();
    lastValeurThread = 0;
}

void MAPSLivicRS232Timer::Core() 
{

    if (comPort->WaitForInterrupts(1000))
        ++(*valeurThread);

    /*MAPSTimestamp time;
    time = MAPS::CurrentTime();
    if(time >= lastTime+1000000)
    {
        lastTime=time;
        char msg[24];
        sprintf(msg,"%d",*valeurThread-lastValeurThread);
        ReportInfo((const char*)msg);
        lastValeurThread=*valeurThread;
    }*/
 
}

void MAPSLivicRS232Timer::Death()
{
    // Reports this information to the RTMaps console
    ReportInfo("LivicRS232Timer: Passing through Death() method");

    delete comPort;
}

bool MAPSLivicRS232Timer::Initialize()
{
	if (comPort==NULL)
	{
		MAPSString portName=GetStringProperty("port");
		portName="\\\\.\\" + portName;
		int baudRate=(int)GetIntegerProperty("baudrate");
		int parity=(int)GetIntegerProperty("parity");
		
		BaudRateDefinition brdef(baudRate,baudRate,baudRate);
		if(GetBoolProperty("pControl"))
		{
			RS232PortDefinition rsdef(portName,&brdef, DTR_CONTROL_ENABLE, RTS_CONTROL_ENABLE);	
			comPort = new RS232Port(portName,&rsdef);
			if (comPort==NULL)
				return(false);
			return(comPort->SetBaudRate((int)baudRate)&&comPort->SetParity((int)parity));
		}
    	else
		{
			RS232PortDefinition rsdef(portName,&brdef);
			comPort = new RS232Port(portName,&rsdef);
			if (comPort==NULL)
				return(false);
			return(comPort->SetBaudRate((int)baudRate)&&comPort->SetParity((int)parity));
		}
	}
	return false;
}

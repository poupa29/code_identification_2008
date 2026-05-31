#include "MemoireP.h"


MemoireP::MemoireP()
{
	BufferErreur=NULL;
	MemPartagee=NULL;
	SemMemPart=NULL;
	AdresseMP=NULL;
	memset(ClefMem,'/0',40);
	memset(ClefSem,'/0',40);
	InitOK=false;
	return;
}

int MemoireP::Initialise(char *Clef, int size, bool Serv, int nbMax, long dureeWrite, long dureeRead)
{
	Serveur=Serv;
	Size=size;
	TimeToWaitWrite=dureeWrite;
	TimeToWaitRead=dureeRead;
	NbMAX=nbMax;
	
	BufferErreur=new char[40];
	if(BufferErreur==NULL)
		return -1;
	if(Clef==NULL)
	{
		sprintf(BufferErreur,"La clef est invalide");
		return -1;
	}
	sprintf(ClefMem,"%sMem",Clef);
	sprintf(ClefSem,"%sSem",Clef);

	if(Serveur)
	{
		SemMemPart=CreateSemaphore(NULL,NbMAX,NbMAX,(LPCTSTR)ClefSem);
		if(SemMemPart==NULL)
		{
			sprintf(BufferErreur,"Le Semaphore n'a pas ete cree par le Serveur");
			return -1;
		}
		MemPartagee=CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,Size,(LPCTSTR)ClefMem);
		if(MemPartagee==NULL)
		{
			sprintf(BufferErreur,"La Memoire partagee n'a pas ete creee par le Serveur");
			return -1;
		}
		AdresseMP=(void *)MapViewOfFile((HANDLE)MemPartagee,FILE_MAP_ALL_ACCESS,0,0,0);
		if(AdresseMP==NULL)
		{
			sprintf(BufferErreur,"La Memoire partagee n'a pas ete mappee le Serveur");
			return -1;
		}
	}
	else
	{
		SemMemPart=OpenSemaphore(SEMAPHORE_ALL_ACCESS,true,(LPCTSTR)ClefSem);
		if(SemMemPart==NULL)
		{
			sprintf(BufferErreur,"Le Semaphore n'a pas ete trouve par le client");
			return -1;
		}
		MemPartagee=OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,true,(LPCTSTR)ClefMem);
		if(MemPartagee==NULL)
		{
			sprintf(BufferErreur,"La Memoire partagee n'a pas ete trouvee par le client");
			return -1;
		}
		AdresseMP=(void *)MapViewOfFile((HANDLE)MemPartagee,FILE_MAP_ALL_ACCESS,0,0,0);
		if(AdresseMP==NULL)
		{
			sprintf(BufferErreur,"La Memoire partagee n'a pas ete mapper le client");
			return -1;
		}
	}
	InitOK=true;
	return 0;
}

char *MemoireP::GetLastErreur()
{
	if(BufferErreur!=NULL)
		return BufferErreur;
	else
		return "BufferErreur non Alloue";
}
int MemoireP::Read(void *data)
{
	if(!InitOK)
	{
		sprintf(BufferErreur,"Initialise non appelee ou echouee");
		return -1;
	}
	if(data==NULL)
	{
		sprintf(BufferErreur,"Read:data NULL");
		return -1;
	}
	printf("avant waitresult\n");
	WaitResult=WaitForSingleObject(SemMemPart,TimeToWaitRead);
	printf("apres waitresult : %d\n", WaitResult);
	switch(WaitResult)
	{
	case WAIT_OBJECT_0:
		printf("waitobject0\n");
		memcpy(data,AdresseMP,Size);
		break;
	case WAIT_TIMEOUT:
		printf("waittimeout\n");
		sprintf(BufferErreur,"Le Semaphore n'a pas ete relache");
		return 1;
		break;
	default:
		sprintf(BufferErreur,"Impossible d'acceder au Semaphore");
		return -1;
		break;
	}
	printf("apres switch\n");
	if(!ReleaseSemaphore(SemMemPart,1,NULL))
	{
		sprintf(BufferErreur,"ReleaseSemaphore a echoue");
		return -1;
	}


	return 0;
}
int MemoireP::Write(void *data)
{
	if(!InitOK)
	{
		sprintf(BufferErreur,"Initialise non appelee au echouee");
		return -1;
	}
	if(data==NULL)
	{
		sprintf(BufferErreur,"Write:data NULL");
		return -1;
	}
	WaitResult=WaitForSingleObject(SemMemPart,TimeToWaitWrite);
	switch(WaitResult)
	{
	case WAIT_OBJECT_0:
		memcpy(AdresseMP,data,Size);
		break;
	case WAIT_TIMEOUT:
		sprintf(BufferErreur,"Le Semaphore n'a pas ete relache");
		return 1;
		break;
	default:
		sprintf(BufferErreur,"Impossible d'acceder au Semaphore");
		return -1;
	}
	if(!ReleaseSemaphore(SemMemPart,1,NULL))
	{
		sprintf(BufferErreur,"ReleaseSemaphore aechoue");
		return -1;
	}
	return 0;
}
MemoireP::~MemoireP()
{
	if((SemMemPart!=NULL))
	{
		if(!CloseHandle(SemMemPart))
			printf("Erreur CloseHandle Sem\n");
        SemMemPart=NULL;
	}
	if(!UnmapViewOfFile((void *)AdresseMP))
		printf("Erreur UnmapViewofFile\n");

	if((MemPartagee!=NULL))
		if(!CloseHandle(MemPartagee))
			printf("Erreur CloseHandle MemPartagee\n");

	if(BufferErreur!=NULL)
	{
		delete [] BufferErreur;
		BufferErreur=NULL;
	}
	return;
}

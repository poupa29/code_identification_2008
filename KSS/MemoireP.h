#ifndef _MEMOIREP_H
#define _MEMOIREP_H

#include <windows.h>
#include <stdio.h>

class MemoireP
{
public:
	MemoireP();
	~MemoireP();
	int Initialise(char *Clef, int size, bool Serv, int nbMax=1, long dureeWrite=5L, long dureeRead=5L);
	char *GetLastErreur();
	int Read(void *data);
	int Write(void *data);
private:
	HANDLE MemPartagee;
	void *AdresseMP;
	HANDLE SemMemPart;
	char ClefMem[40];
	char ClefSem[40];
	int Size;
	bool Serveur;
	char *BufferErreur;
	long TimeToWaitWrite;
	long TimeToWaitRead;
	DWORD WaitResult;
	bool InitOK;
	int NbMAX;
};

#endif //_MEMOIREP_H
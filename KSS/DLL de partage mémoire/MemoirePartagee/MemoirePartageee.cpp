// Archivo DLL principal.

#include "MemoirePartageee.h"
#define LPCWSTR LPCSTR

int ReadMP(char *signetRead)
{
	int *a;
	Sleep(1000);
	hMap= OpenFileMapping(	FILE_MAP_READ | FILE_MAP_WRITE,
		TRUE,
		signetRead);

	if (hMap == NULL) 
	{
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());	
	}

	LPVOID lpMapAddress;
	lpMapAddress = MapViewOfFile(hMap,		// Handle to mapping object. 
		FILE_MAP_ALL_ACCESS,               // Read/write permission. 
		0,                                 // Max. object size. 
		0,                                 // Size of hFile. 
		0);                                // Map entire file. 

	if (lpMapAddress == NULL) 
	{ 
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());

	}

	a = (int *) lpMapAddress;

	//int val;
	//val =(int)*b;

	return (int)*a;
}

int WriteMP(int value, char *signetWrite)
{
	int *b;
	// Initialisation de la memoire partage 
	hMap = CreateFileMapping(	(HANDLE)0xFFFFFFFF,
		NULL ,
		PAGE_READWRITE 
		,0
		,4096
		,signetWrite);
	if (hMap == NULL) 
	{
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());
	}

	LPVOID lpMapAddress;
	lpMapAddress = MapViewOfFile(hMap, // Handle to mapping object. 
		FILE_MAP_ALL_ACCESS,               // Read/write permission 
		0,                                 // Max. object size. 
		0,                                 // Size of hFile. 
		0);                                // Map entire file. 

	if (lpMapAddress == NULL) 
	{ 
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());		
	} 

	b =(int *) lpMapAddress;

	*b =value;

	return 0;
}
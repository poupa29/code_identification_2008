// SensorDLL.h
#include <stdio.h>
#include <windows.h>
#undef UNICODE

int __declspec(dllexport) ReadMP(char *signetRead);
int __declspec(dllexport) WriteMP(int value, char *signetWrite);

bool Start;




	HANDLE hMap;
	
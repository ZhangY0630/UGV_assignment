#pragma once


#using <System.dll>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

#include "SMStructs.h"
#include "SMObject.h"

#define NUM_UNITS 2

//defining start up sequence
TCHAR Units[10][20] = //
{
	TEXT("GPS.exe"),
	TEXT("Laser.exe"),
	TEXT("Camera.exe"),
	TEXT("Display.exe"),
	TEXT("VehicleControl.exe")

};

STARTUPINFO s[10];
PROCESS_INFORMATION p[10];

void killProcessByName(const char* filename);
//Is process running function
bool IsProcessRunning(const char* processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_stricmp((const char*)entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}
void ClearProcesses() {
	int retval;
	retval = ::_tsystem(_T("taskkill /F /T /IM GPS.exe"));
	for (int i = 0; i < NUM_UNITS; i++)
	{	
		while (IsProcessRunning((const char*)Units[i])) {
			Console::WriteLine("Killing index {0}",i);
			killProcessByName(Units[i]);
		}
	}
};


void StartProcesses()
{
	//STARTUPINFO s[10];
	//PROCESS_INFORMATION p[10];

	for (int i = 0; i < NUM_UNITS; i++)
	{
		if (IsProcessRunning((const char*)Units[i]))
		{
			Console::WriteLine("Process still running");
			killProcessByName(Units[i]);
		}
		if (!IsProcessRunning((const char*)Units[i])){
			ZeroMemory(&s[i], sizeof(s[i]));
			s[i].cb = sizeof(s[i]);
			ZeroMemory(&p[i], sizeof(p[i]));

			if (!CreateProcess(NULL, Units[i], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &s[i], &p[i]))
			{
				printf("%s failed (%d).\n", Units[i], GetLastError());
				_getch();
			}
			std::cout << "Started: " << Units[i] << std::endl;
			Sleep(100);
		}
		else {
			Console::WriteLine("Process start fail");
		}



	}
}
//from stack overflow
void killProcessByName(const char* filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

void Restart(int i) {
	Console::WriteLine("Restarting..");
	Console::WriteLine("Index is {0}", i);

	if (!IsProcessRunning((const char*)Units[i]))
	{
		ZeroMemory(&s[i], sizeof(s[i]));
		s[i].cb = sizeof(s[i]);
		ZeroMemory(&p[i], sizeof(p[i]));
		if (!CreateProcess(NULL, Units[i], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &s[i], &p[i]))
		{
			printf("%s failed (%d).\n", Units[i], GetLastError());
			_getch();
		}
		std::cout << "Started: " << Units[i] << std::endl;
		Sleep(100);
	}
	else {
		Console::WriteLine("Fail to close the process");
	}
}
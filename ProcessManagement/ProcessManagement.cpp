
#using <System.dll>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

#include "SMStructs.h"
#include "SMObject.h"
#include "utility.h"

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;



bool IsProcessRunning(const char* processName);
void StartProcesses();



int main()
{
	killProcessByName(Units[0]);
	
	SMObject PMObj(_TEXT("PM_SM"), sizeof(ProcessManagement));
	PMObj.SMCreate();
	 SMObject LaserObj(_TEXT("Laser_SM"), sizeof(SM_Laser));
	 LaserObj.SMCreate();
	 LaserObj.SMAccess();
	SMObject GPSObj(_TEXT("GPS_SM"), sizeof(SM_GPS));
	GPSObj.SMCreate();
	SMObject ControlObj(_TEXT("Control_SM"), sizeof(SM_VehicleControl));
	ControlObj.SMCreate();
	//SMAccess()
	PMObj.SMAccess();
	GPSObj.SMAccess();
	LaserObj.SMAccess();
	ControlObj.SMAccess();

	//building a pointer
	ProcessManagement* PMData = (ProcessManagement*)PMObj.pData;
	 SM_Laser* LaserData = (SM_Laser*)LaserObj.pData;
	SM_GPS* GPSData = (SM_GPS*)GPSObj.pData;
	SM_VehicleControl* ControlData = (SM_VehicleControl*)ControlObj.pData;

	//initialise status
	PMData->Shutdown.Status = 0x00;
	PMData->Heartbeat.Status = 0x00;

	GPSData->easting = 0;
	GPSData->northing = 0;
	GPSData->height = 0;

	for (int i = 0; i < 8; i++) {
		PMData->waitCount[i] = 0;
	}

	//Console::WriteLine(GPSData->easting);

	//ClearProcesses();
	StartProcesses();
	//Restart(0);
	//Console::ReadKey();
	//while(1){
	Console::WriteLine("start watching...");
	while (!PMData->Shutdown.Flags.ProcessManagement) {
		Sleep(25);
		if (PMData->Heartbeat.Flags.GPS == 1) {
			Console::WriteLine("Detect gps heartheats");
			PMData->Heartbeat.Flags.GPS = 0;
			PMData->waitCount[gps_count] = 0;
		}
		else {
			PMData->waitCount[gps_count]++;
		}
		if (PMData->Heartbeat.Flags.Laser == 1) {
			Console::WriteLine("Detect Laser heartheats");
			PMData->Heartbeat.Flags.Laser = 0;
			PMData->waitCount[laser_count] = 0;
		}
		else {
			PMData->waitCount[laser_count]++;
		}
		if (PMData->Heartbeat.Flags.Camera == 1) {
			Console::WriteLine("Detect Camera heartheats");
			PMData->Heartbeat.Flags.Camera = 0;
			PMData->waitCount[camera_count] = 0;
		}
		else {
			PMData->waitCount[camera_count]++;
		}
		if (PMData->Heartbeat.Flags.OpenGL == 1) {
			Console::WriteLine("Detect OpenGL heartheats");
			PMData->Heartbeat.Flags.OpenGL = 0;
			PMData->waitCount[display_count] = 0;
		}
		else {
			PMData->waitCount[display_count]++;
		}
		if (PMData->Heartbeat.Flags.VehicleControl == 1) {
			Console::WriteLine("Detect VehicleControl heartheats");
			PMData->Heartbeat.Flags.VehicleControl = 0;
			PMData->waitCount[control_count] = 0;
		}
		else {
			PMData->waitCount[control_count]++;
		}


		for (int i = 0; i < 6; i++) {
			if (NONCRITICALMASK & (1 << i)) { //if this one is non-critical
				if (PMData->waitCount[i] > 100) { //and no response for a long time
					Console::WriteLine("Over limit");
					if (IsProcessRunning(Units[i])) {
						Console::WriteLine("Process Still running");
						killProcessByName(Units[i]);
						Restart(i);
						PMData->waitCount[i] = 0;
					}
					else {
						Restart(i);
						PMData->waitCount[i] = 0;
					}
				}
			}
			else {//if this one is critical
				if (PMData->waitCount[i] > 100) {
					PMData->Shutdown.Status = 0xFF;
				}
			}
		}

		if (_kbhit()) break;
	}

	//while (1) {
	//	if (_kbhit()) break;
	//}

	//start all 5 modules
	//StartProcesses();
	//return 0;
}




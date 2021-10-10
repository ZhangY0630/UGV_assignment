#pragma once

#ifndef SMSTRUCTS_H
#define SMSTRUCTS_H

#using <System.dll>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;


#define STANDARD_LASER_LENGTH 361

#define gps_count 0


struct SM_Laser
{
	double x[STANDARD_LASER_LENGTH];
	double y[STANDARD_LASER_LENGTH];
};

struct SM_VehicleControl
{
	double Speed;
	double Steering;
};

struct SM_GPS
{
	double northing;
	double easting;
	double height;
};

struct UnitFlags
{
	unsigned char GPS : 1,				//NONCRITICAL	
					Laser : 1,				//CRITICAL
					VehicleControl : 1,		//CRITICAL
					OpenGL : 1,				//NONCRITICAL
				Camera : 1,				//CRITICAL
				ProcessManagement : 1,	//CRITICAL
					Garbage : 2;
};

union ExecFlags
{
	UnitFlags Flags;
	unsigned short Status;
};
#pragma pack(push,4)
struct ProcessManagement
{
	ExecFlags Heartbeat;
	ExecFlags Shutdown;
	double waitCount[8];
	long int LifeCounter;
};
#pragma pack(pop)
#define NONCRITICALMASK 0x09	//00001001
#define CRITICALMASK 0xF6		//11110110
#endif

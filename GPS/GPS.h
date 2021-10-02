#pragma once

#include <UGV_module.h>
#include <smstructs.h>

#define CRC32_POLYNOMIAL 0xEDB88320L

unsigned long CRC32Value(int i);
unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer);

using namespace System;
using namespace System::Net::Sockets;

struct GPSData {
	unsigned char discard[44];
	double northing;
	double easting;
	double height;
	unsigned char discard1[40];
	unsigned int checksum;
};

ref class GPS : public UGV_module
{

public:
	ProcessManagement* PMdata;
	SM_GPS* GPSdata;
	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	~GPS();

private:
	//GSPData gpsdata
	TcpClient^ client;
	String^ ipAdress;
	NetworkStream^ stream;
	int port;
	double north;
	double east;
	double height;
	unsigned int crc;
	array<unsigned char>^ receiveData;
	unsigned char* structPtr;

//protected:
	// YOUR CODE HERE (ADDITIONAL MEMBER VARIABLES THAT YOU MAY WANT TO ADD)

};


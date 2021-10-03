#pragma once

#include <UGV_module.h>
#include <smstructs.h>

#define CRC32_POLYNOMIAL 0xEDB88320L

unsigned long CRC32Value(int i);
unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer);

using namespace System;
using namespace System::Net::Sockets;

#pragma pack(push,4)
struct GPSData {
	unsigned int Header;
	unsigned char Discards[40];
	double northing;
	double easting;
	double height;
	unsigned char Discards1[40];
	unsigned int checksum;
};
#pragma pack(pop)
ref class GPS : public UGV_module
{

public:
	ProcessManagement* PMdata;
	SM_GPS* GPSinfo;
	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	~GPS();
	int processData() override;
	void printData() override;

private:
	//GSPData gpsdata
	String^ ipAdress;
	int port;
	double north;
	double east;
	double height;
	unsigned int crc;
	unsigned char* structPtr;

//protected:
	// YOUR CODE HERE (ADDITIONAL MEMBER VARIABLES THAT YOU MAY WANT TO ADD)

};


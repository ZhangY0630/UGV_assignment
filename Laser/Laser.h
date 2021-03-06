#pragma once

#include <UGV_module.h>
#include <smstructs.h>
#include <SMObject.h>


ref class Laser : public UGV_module
{
public:

	ProcessManagement* PMdata;
	SM_Laser* Laser_info;

	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	~Laser();
	int processData() override;
	void printData() override;
	int Auth(String^ zid) override;
private:
	//GSPData gpsdata
	array<double>^ Range = nullptr;
	array<double>^ RangeX = nullptr;
	array<double>^ RangeY = nullptr;
	double StartAngle;
	double Resolution;
	int NumRanges;
	array<String^>^ Fragments = nullptr;
	//unsigned char* structPtr;

	//protected:
		// YOUR CODE HERE (ADDITIONAL MEMBER VARIABLES THAT YOU MAY WANT TO ADD)

};
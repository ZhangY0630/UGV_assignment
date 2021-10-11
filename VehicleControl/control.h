#pragma once
#pragma once

#include <UGV_module.h>
#include <smstructs.h>
#include <SMObject.h>



ref class Control : public UGV_module
{
public:

	ProcessManagement* PMdata;
	SM_VehicleControl* Control_info;

	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	~Control();
	int processData() override;
	void printData() override;

	//unsigned char* structPtr;

	//protected:
		// YOUR CODE HERE (ADDITIONAL MEMBER VARIABLES THAT YOU MAY WANT TO ADD)

};
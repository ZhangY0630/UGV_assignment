#include "control.h"

int Control::connect(String^ hostName, int portNumber) {
	return 1;
};
int Control::setupSharedMemory() {
	Console::WriteLine("Setting up shared memory");
	//	Console::WriteLine("Setting up shared memory");
	ProcessManagementData = new SMObject(_TEXT("PM_SM"), sizeof(ProcessManagement));
	SensorData = new SMObject(_TEXT("Control_SM"), sizeof(SM_VehicleControl));
	ProcessManagementData->SMAccess();
	SensorData->SMAccess();
	//
	if (SensorData->SMAccessError)
	{
		Console::WriteLine("Share memory access failed");
		return -2;
	}
	//
	PMdata = (ProcessManagement*)ProcessManagementData->pData;
	Control_info = (SM_VehicleControl*)SensorData->pData;
	Console::WriteLine("Setting up shared memory finished");
};
int Control::getData() {
	return 1;
};
int Control::checkData() {
	return 1;
};
int Control::sendDataToSharedMemory() {
	return 1;
};
bool Control::getShutdownFlag() {
	bool shutdown = PMdata->Shutdown.Flags.VehicleControl;
	return shutdown;
};
int Control::setHeartbeat(bool heartbeat) {
	PMdata->Heartbeat.Flags.VehicleControl = heartbeat;
	// YOUR CODE HERE
	return 1;
};
Control::~Control() {
	delete  SensorData;
	delete ProcessManagementData;
};
int Control::processData() {
	return 1;
};
void Control::printData() {
};
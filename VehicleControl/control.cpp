#include "control.h"

int Control::connect(String^ hostName, int portNumber) {
	this->Client = gcnew TcpClient(hostName, portNumber);
	Client->NoDelay = TRUE;
	Client->ReceiveTimeout = 2000;
	Client->SendTimeout = 1000;
	Client->ReceiveBufferSize = 2048;
	Client->SendBufferSize = 32;

	Stream = Client->GetStream();
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
	this->speed = Control_info->Speed;
	this->steering = Control_info->Steering;

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
int Control::Auth(String^ zid) {  //gcnew string
	array<unsigned char>^ SendZID;
	SendZID = System::Text::Encoding::ASCII->GetBytes(zid);
	Stream->Write(SendZID, 0, SendZID->Length);
	//array<unsigned char>^ IDAuthentication;
	//IDAuthentication = gcnew array<unsigned char>(512);
	//Stream->Read(IDAuthentication, 0, IDAuthentication->Length);
	//System::String^ ResponseData;
	//ResponseData = System::Text::Encoding::ASCII->GetString(IDAuthentication);
	//if (ResponseData->Contains("OK")) {
	//	return 1;
	//}
	//else {
	//	Console::WriteLine(ResponseData);
	//	return 0;
	return 1; // £¿£¿£¿
};

void Control::controlVehicle() {
	String^ SendControl = gcnew String("# " + steering.ToString("f2") + " "
		+ speed.ToString("f2") + " " + flag + " #");
}

void Control::flipFlag() {
	this->flag = !this->flag;
}
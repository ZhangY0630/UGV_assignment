#include "GPS.h"
//using namespace GPS;

int main() {
	GPS^ gpsmodule = gcnew GPS();
	gpsmodule->setupSharedMemory();
	gpsmodule->connect("192.168.1.200", 24000);
	gpsmodule->getData();
	if (gpsmodule->checkData() == 0) {
		Console::WriteLine("Error GPS read");
		//gpsmodule->sendDataToSharedMemory();
	}
	gpsmodule->processData();
	gpsmodule->~GPS();
	Console::WriteLine("GPS module terminate successfully");
	
	//int waitNum = 0;
	//while (!gpsmodule->getShutdownFlag()) {
	//	if (gpsmodule->PMdata->Heartbeat.Flags.GPS == 0) { //means the pm has already detect the heart beats
	//		gpsmodule->PMdata->Heartbeat.Flags.GPS == 1;

	//	}

	//}

	return 1;
}

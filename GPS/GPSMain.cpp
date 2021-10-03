#include "GPS.h"
//using namespace GPS;

int main() {
	GPS^ gpsmodule = gcnew GPS();
	gpsmodule->setupSharedMemory();
	gpsmodule->connect("192.168.1.200", 24000); // TODO: add connect file.

	double timeGap;
	_int64 frequency,counter, oldcounter;
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&oldcounter);
	
	int waitTime = 0;
	while (!gpsmodule->getShutdownFlag()) {
		Sleep(25);
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		timeGap = (double)(counter - oldcounter) / (double)frequency * 1000;
		if (gpsmodule->PMdata->Heartbeat.Flags.GPS == 1) { //means the pm not response to the heartbeats
			waitTime = waitTime + timeGap;
			if (waitTime > 1000) {
				Console::WriteLine("fail to receive the response of PM");
				break;
			}
		}
		else {
			waitTime = 0;
			gpsmodule->PMdata->Heartbeat.Flags.GPS = 1;
		}
	
		gpsmodule->getData();
		if (gpsmodule->checkData() == 0) {
			Console::WriteLine("Error GPS read");
			continue;
		}
		if (gpsmodule->processData() == 1) {
			gpsmodule->printData();
			gpsmodule->PMdata->Heartbeat.Flags.GPS = 1;
			gpsmodule->sendDataToSharedMemory();
		}
		else {
			Console::WriteLine("Process Fail... Discard and wait for other data");
		}

		gpsmodule->~GPS();
		


	}

	return 1;
}

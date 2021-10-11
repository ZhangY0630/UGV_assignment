#include "GPS.h"
//using namespace GPS;

int main() {
	GPS^ gpsmodule = gcnew GPS();
	gpsmodule->setupSharedMemory();
	gpsmodule->connect("192.168.1.200", 24000); // TODO: add connect file.
	//Console::ReadKey();
	double timeGap;
	_int64 frequency,counter, oldcounter;
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&oldcounter);

	//
	int waitTime = 0;
	while (!gpsmodule->getShutdownFlag()) {
		Sleep(25);
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		timeGap = (double)(counter - oldcounter) / (double)frequency * 1000;
		oldcounter = counter;
		if (gpsmodule->PMdata->Heartbeat.Flags.GPS == 1) { //means the pm not response to the heartbeats
			waitTime = waitTime + timeGap;
			Console::WriteLine(waitTime);
			if (waitTime > 1000) {
				Console::WriteLine("fail to receive the response of PM");
				break;
			}
		}
		else {
			Console::WriteLine("Detect PM Response");
			waitTime = 0;
			gpsmodule->PMdata->Heartbeat.Flags.GPS = 1;
		}
	//
		gpsmodule->getData();
		if (gpsmodule->checkData() == 0) {
			Console::WriteLine("Error GPS read");
			continue;
		}
		if (gpsmodule->processData() == 1) {
			gpsmodule->printData();
			gpsmodule->PMdata->Heartbeat.Flags.GPS = 1;



			Console::WriteLine("Send to shared mempory");
			gpsmodule->sendDataToSharedMemory();
		}
		else {
			Console::WriteLine("Process Fail... Discard and wait for other data");
		}

	}
	Console::WriteLine("Terminate Successfully");
	//gpsmodule->~GPS();

	return 1;
}

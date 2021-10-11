#include "Laser.h"
int main() {
	Laser^ lasermodule = gcnew Laser();
	lasermodule->setupSharedMemory();
	//lasermodule->connect("192.168.1.200", 23000);
	String^ zid = nullptr;
	//zid = gcnew String("5122988\n");
	//if (lasermodule->Auth(zid)) {
	//	Console::WriteLine("Authentication Fail");
	//}
	double timeGap;
	_int64 frequency, counter, oldcounter;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&oldcounter);
	int waitTime = 0;
	Console::WriteLine("Start Processing");
	while (!lasermodule->getShutdownFlag()) {
		//Console::WriteLine("testing..");
		Sleep(250);
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		timeGap = (double)(counter - oldcounter) / (double)frequency * 1000;
		oldcounter = counter;
		if (lasermodule->PMdata->Heartbeat.Flags.Laser == 1) { //means the pm not response to the heartbeats
			waitTime = waitTime + timeGap;
			Console::WriteLine(waitTime);
			if (waitTime > 1000) {
				Console::WriteLine("fail to receive the response of PM");
				lasermodule->PMdata->Shutdown.Status = 0xFF;
				break;
			}
		}
		else {
			Console::WriteLine("Detect PM Response");
			waitTime = 0;
			lasermodule->PMdata->Heartbeat.Flags.Laser = 1;
		}
		lasermodule->getData();
		if (lasermodule->processData() == 1) {
			lasermodule->printData();
			lasermodule->PMdata->Heartbeat.Flags.Laser = 1;

		}
		else {
			Console::WriteLine("Process Fail... Discard and wait for other data");
		}
		if (lasermodule->checkData() == 0) {
			Console::WriteLine("Error Lidar read");
			continue;
		}
		Console::WriteLine("Send to shared mempory");
		lasermodule->sendDataToSharedMemory();

	}

	Console::WriteLine("Terminate Successfully");
	//gpsmodule->~GPS();

	return 1;


}
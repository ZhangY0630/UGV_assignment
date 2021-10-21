#include "control.h"
int main() {
	Control^ controlmodule = gcnew Control();
	controlmodule->setupSharedMemory();

	double timeGap;
	_int64 frequency, counter, oldcounter;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&oldcounter);
	int waitTime = 0;
	Console::WriteLine("Start Processing");

	while (!controlmodule->getShutdownFlag()) {
		//Console::WriteLine("testing..");
		Sleep(250);
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		timeGap = (double)(counter - oldcounter) / (double)frequency * 1000;
		oldcounter = counter;
		if (controlmodule->PMdata->Heartbeat.Flags.VehicleControl == 1) { //means the pm not response to the heartbeats
			waitTime = waitTime + timeGap;
			Console::WriteLine(waitTime);
			if (waitTime > 2000) {
				Console::WriteLine("fail to receive the response of PM");
				controlmodule->PMdata->Shutdown.Status = 0xFF;
				break;
			}
		}
		else {
			Console::WriteLine("Detect PM Response");
			waitTime = 0;
			controlmodule->PMdata->Heartbeat.Flags.VehicleControl = 1;
		}

	}
}
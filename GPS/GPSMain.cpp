#include "GPS.h"
//using namespace GPS;

int main() {
	GPS^ gpsmodule = gcnew GPS();
	gpsmodule->connect("192.168.1.200", 24000);
	gpsmodule->getData();
	return 1;
}

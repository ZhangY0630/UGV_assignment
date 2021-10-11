#include "Laser.h"
int main() {
	Laser^ lasermodule = gcnew Laser();
	lasermodule->connect("192.168.1.200", 23000);

	String^ zid = nullptr;
	zid = gcnew String("5122988\n");
	lasermodule->Auth(zid);


	lasermodule->getData();
	lasermodule->processData();
	lasermodule->checkData();
	//lasermodule->processData();
	//lasermodule->printData();

	Console::ReadKey();
}
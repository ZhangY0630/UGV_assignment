#include "Laser.h"
int main() {
	Laser^ lasermodule = gcnew Laser();
	lasermodule->connect("192.168.1.200", 25000);
}
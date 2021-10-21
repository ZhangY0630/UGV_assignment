#ifndef __MYVEHICLE_HPP__
#define __MYVEHICLE_HPP__


#include "Vehicle.hpp"

class MyVehicle : public Vehicle
{
public:
	MyVehicle();
	int* num;
	double* xData;
	double* yData;
	void setData(double xData[], double yData[]);
	virtual void draw();
	void drawLaser();
};

#endif
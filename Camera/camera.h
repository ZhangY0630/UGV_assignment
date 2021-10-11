#pragma once
#include <zmq.hpp>
#include <Windows.h>

#include "SMStructs.h"
#include "SMFcn.h"
#include "SMObject.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <turbojpeg.h>

#include <UGV_module.h>
#include <smstructs.h>
#include <SMObject.h>



ref class Camera : public UGV_module
{
public:

	ProcessManagement* PMdata;
	//SM_Laser* _info;

	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	~Camera();
	int processData() override;
	void printData() override;


};

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


GLuint tex;
zmq::context_t context(1);
zmq::socket_t subscriber(context, ZMQ_SUB);
void display();
void idle();
SMObject* ProcessManagementData;
ProcessManagement* PMdata;

double timeGap;
_int64 frequency, counter, oldcounter;
int waitTime = 0;




int main(int argc, char** argv) {
	Console::WriteLine("Setting up shared memory");

	ProcessManagementData = new SMObject(_TEXT("PM_SM"), sizeof(ProcessManagement));
	ProcessManagementData->SMAccess();

	if (ProcessManagementData->SMAccessError)
	{
		Console::WriteLine("Share memory access failed");
		return -2;
	}
	//
	PMdata = (ProcessManagement*)ProcessManagementData->pData;
	Console::WriteLine("Setting up shared memory finished");



	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	//GL Window setup
	glutInit(&argc, (char**)(argv));
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("MTRN3500 - Camera");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glGenTextures(1, &tex);

	//Socket to talk to server
	subscriber.connect("tcp://192.168.1.200:26000");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&oldcounter);
	glutMainLoop();

	



	return 1;

}




void display()
{
	//Set camera as gl texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	//Map Camera to window
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(-1, -1);
	glTexCoord2f(1, 1); glVertex2f(1, -1);
	glTexCoord2f(1, 0); glVertex2f(1, 1);
	glTexCoord2f(0, 0); glVertex2f(-1, 1);
	glEnd();
	glutSwapBuffers();
}
void idle()
{
	
	//receive from zmq
	zmq::message_t update;
	if (PMdata->Shutdown.Flags.Camera) {
		exit(-1);
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&counter);
	timeGap = (double)(counter - oldcounter) / (double)frequency * 1000;
	oldcounter = counter;
	
	if (PMdata->Heartbeat.Flags.Camera == 1) { //means the pm not response to the heartbeats
		waitTime = waitTime + timeGap;
		Console::WriteLine(waitTime);
		if (waitTime > 2000) {
			Console::WriteLine("fail to receive the response of PM");
			PMdata->Shutdown.Status = 0xFF;
			exit(-1);
		}
	}
	else {
		Console::WriteLine("Detect PM Response");
		waitTime = 0;
		PMdata->Heartbeat.Flags.Camera = 1;
	}


	if (subscriber.recv(&update, ZMQ_NOBLOCK))
	{
		//Receive camera data
		long unsigned int _jpegSize = update.size();
		std::cout << "received " << _jpegSize << " bytes of data\n";
		unsigned char* _compressedImage = static_cast<unsigned char*>(update.data());
		int jpegSubsamp = 0, width = 0, height = 0;

		//JPEG Decompression
		tjhandle _jpegDecompressor = tjInitDecompress();
		tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);
		unsigned char* buffer = new unsigned char[width * height * 3]; //!< will contain the decompressed image
		printf("Dimensions:  %d   %d\n", height, width);
		tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, buffer, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);
		tjDestroy(_jpegDecompressor);

		//load texture
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
		delete[] buffer;
	}

	display();
	Sleep(100);
}
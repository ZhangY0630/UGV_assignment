

#include "camera.h"



//void display()
//{
//	//Set camera as gl texture
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, tex);
//
//	//Map Camera to window
//	glBegin(GL_QUADS);
//	glTexCoord2f(0, 1); glVertex2f(-1, -1);
//	glTexCoord2f(1, 1); glVertex2f(1, -1);
//	glTexCoord2f(1, 0); glVertex2f(1, 1);
//	glTexCoord2f(0, 0); glVertex2f(-1, 1);
//	glEnd();
//	glutSwapBuffers();
//}
//void idle()
//{
//
//	//receive from zmq
//	zmq::message_t update;
//	if (subscriber.recv(&update, ZMQ_NOBLOCK))
//	{
//		//Receive camera data
//		long unsigned int _jpegSize = update.size();
//		std::cout << "received " << _jpegSize << " bytes of data\n";
//		unsigned char* _compressedImage = static_cast<unsigned char*>(update.data());
//		int jpegSubsamp = 0, width = 0, height = 0;
//
//		//JPEG Decompression
//		tjhandle _jpegDecompressor = tjInitDecompress();
//		tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);
//		unsigned char* buffer = new unsigned char[width * height * 3]; //!< will contain the decompressed image
//		printf("Dimensions:  %d   %d\n", height, width);
//		tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, buffer, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);
//		tjDestroy(_jpegDecompressor);
//
//		//load texture
//		glBindTexture(GL_TEXTURE_2D, tex);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
//		delete[] buffer;
//	}
//
//	display();
//}

int Camera::connect(String^ hostName, int portNumber) {
	return 1;
}
int Camera::setupSharedMemory() {
	Console::WriteLine("Setting up shared memory");
	//	Console::WriteLine("Setting up shared memory");
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
	return 1;
}
int Camera::getData() {
	return 1;
}
int Camera::checkData() {
	return 1;
}
int Camera::sendDataToSharedMemory() {
	return 1;
}
bool Camera::getShutdownFlag() {
	bool shutdown = PMdata->Shutdown.Flags.GPS;
	return shutdown;
}
int Camera::setHeartbeat(bool heartbeat) {
	PMdata->Heartbeat.Flags.GPS = heartbeat;
	// YOUR CODE HERE
	return 1;
}
Camera::~Camera() {
	Stream->Close();
	Client->Close();
	delete ProcessManagementData;
}
int Camera::processData() {
	return 1;
}
void Camera::printData() {

}
#include "GPS.h"
//using namespace GPS;

unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer);

int GPS::connect(String^ hostName, int portNumber) 
{
	// YOUR CODE 
	this->ipAdress = hostName;
	this->port = portNumber;
	this->Client = gcnew TcpClient(hostName, portNumber);
	Client->NoDelay = TRUE;
	Client->ReceiveTimeout = 2000;
	Client->SendTimeout = 1000;
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	Stream = Client->GetStream();
	
	return 1;
}
int GPS::setupSharedMemory()
{
	SMObject PMObj(_TEXT("PM_SM"), sizeof(ProcessManagement));
	SMObject GPSObj(_TEXT("GPS_SM"), sizeof(SM_GPS));
	PMObj.SMAccess();
	GPSObj.SMAccess();
	PMdata = (ProcessManagement*)PMObj.pData;
	GPSdata = (SM_GPS*)GPSObj.pData;

	// YOUR CODE HERE
	return 1;
}



int GPS::getData()
{
	ReadData = gcnew array<unsigned char>(256);
	Stream->Read(ReadData, 0, ReadData->Length);
	return 1;
}

int GPS::processData()
{
	//Console::WriteLine(ReadData);
	GPSData gpsdata;
	
	structPtr = (unsigned char*)&gpsdata;
	unsigned int a= (sizeof(GPSData));
	for (int i = 0; i < 112; i++) {
		structPtr[i] = ReadData[i];
	}
	east = gpsdata.easting;
	north = gpsdata.northing;
	height = gpsdata.height;
	crc = gpsdata.checksum;
	unsigned int checksum = CalculateBlockCRC32(112 - 4, structPtr);
	if (crc == checksum) {
		return 1;
	}
	else {
		return 0;
	}

	// YOUR CODE HERE
	return 1;
}
int GPS::checkData()
{
	bool hasHeader = false;
	unsigned int Header = 0;
	int i = 0;
	int Start; //Start of data
	unsigned char Data;

	do
	{
		Data = ReadData[i++];
		Header = ((Header << 8) | Data);
		if (Header == 0xaa44121c) {
			hasHeader = true;
		}
	} while (Header != 0xaa44121c && i < ReadData->Length);
	return hasHeader;
}
int GPS::sendDataToSharedMemory()
{
	// YOUR CODE HERE
	GPSdata->easting = east;
	GPSdata->northing = north;
	GPSdata->height = height;
	return 1;
}
bool GPS::getShutdownFlag()
{
	bool shutdown = PMdata->Shutdown.Flags.GPS;
	return shutdown;
	// YOUR CODE HERE
	//return 1;
}
int GPS::setHeartbeat(bool heartbeat)
{
	PMdata->Heartbeat.Flags.GPS = heartbeat;
	// YOUR CODE HERE
	return 1;
}
GPS::~GPS()
{
	// YOUR CODE HERE
	Stream->Close();
	Client->Close();
}


unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

unsigned long CalculateBlockCRC32(unsigned long ulCount, /* Number of bytes in the data block */
	unsigned char* ucBuffer) /* Data block */
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}
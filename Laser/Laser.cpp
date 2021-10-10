#include "Laser.h"


int Laser::connect(String^ hostName, int portNumber)
{

	this->Client = gcnew TcpClient(hostName, portNumber);
	Client->NoDelay = TRUE;
	Client->ReceiveTimeout = 2000;
	Client->SendTimeout = 1000;
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	Stream = Client->GetStream();


	return 1;
};
int Laser::Auth(String^ zid) {  //gcnew string
	array<unsigned char>^ SendZID;
	SendZID = System::Text::Encoding::ASCII->GetBytes(zid);
	Stream->Write(SendZID, 0, SendZID->Length);
	array<unsigned char>^ IDAuthentication;
	IDAuthentication = gcnew array<unsigned char>(512);
	Stream->Read(IDAuthentication, 0, IDAuthentication->Length);

	return 1;
};
void Laser::printData() {
	Console::WriteLine("print data");
};
int Laser::setupSharedMemory()
{
	Console::WriteLine("Setting up shared memory");
//	Console::WriteLine("Setting up shared memory");
//	ProcessManagementData = new SMObject(_TEXT("PM_SM"), sizeof(ProcessManagement));
//	SensorData = new SMObject(_TEXT("Laser_SM"), sizeof(SM_Laser));
//	ProcessManagementData->SMAccess();
//	SensorData->SMAccess();
//
//	if (SensorData->SMAccessError)
//	{
//		Console::WriteLine("Share memory access failed");
//		return -2;
//	}
//
//	PMdata = (ProcessManagement*)ProcessManagementData->pData;
//	Laser_info = (SM_Laser*)SensorData->pData;
//	Console::WriteLine("Setting up shared memory finished");
	return 1;
};
int Laser::getData()
{
	String^ RequestScan = nullptr;
	RequestScan = gcnew String("sRN LMDscandata");
	ReadData = gcnew array<unsigned char>(2048);

	//Command to LiDAR requesting a single scacn
	array<unsigned char>^ SendData;
	SendData = System::Text::Encoding::ASCII->GetBytes(RequestScan);
	Stream->WriteByte(0x02);//SOT
	Stream->Write(SendData, 0, SendData->Length);
	Stream->WriteByte(0x03);
	Stream->Read(ReadData, 0, ReadData->Length);
	return 1;
};
int Laser::processData()
{
	String^ LaserScan = nullptr;
	LaserScan = System::Text::Encoding::ASCII->GetString(ReadData);
	Fragments = LaserScan->Split(' ');

	StartAngle = System::Convert::ToInt32(Fragments[23], 16);
	Resolution = System::Convert::ToInt32(Fragments[24], 16) / 10000.0;
	NumRanges = System::Convert::ToInt32(Fragments[25], 16);

	Range = gcnew array<double>(NumRanges);
	RangeX = gcnew array<double>(NumRanges);
	RangeY = gcnew array<double>(NumRanges);

	for (int i = 0; i < NumRanges; i++)
	{
		Range[i] = System::Convert::ToInt32(Fragments[26 + i], 16);
		RangeX[i] = Range[i] * Math::Sin(i * Resolution * Math::PI / 180.0);
		RangeY[i] = Range[i] * Math::Cos(i * Resolution * Math::PI / 180.0);




		if (i < 5) { //debug purpose
			Console::WriteLine("{0,10:F3} {1,10:F3} {2,10:F3}", Range[i], RangeX[i], RangeY[i]);
		}
	}
	return 1;
}

int Laser::checkData() {
	Console::WriteLine("Setting up shared memory");
	return 1;
};
int Laser::sendDataToSharedMemory() {
	return 1;
};
bool Laser::getShutdownFlag() {
	return FALSE;
};
int Laser::setHeartbeat(bool heartbeat) {
	return 1;
};
Laser::~Laser() {
	Stream->Close();
	Client->Close();
	delete  SensorData;
	delete ProcessManagementData;
};
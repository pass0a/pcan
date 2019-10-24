#include <thread>
#include "pcan.h"
#include "HKMC_ASK_Client.h"

int closeflag;
int endthread;
std::thread tmp;

int Pcan::initPcan(int baudrate,int hwtype,int selectedio,int selectedinterrupt,PCAN_CALLBACK cb)
{
	return 0;
}

int Pcan::uninitPcan()
{
	return 0;
}

int Pcan::write(BYTE* data,BYTE *key)
{
	/**TPCANStatus stsResult;
	TPCANMsg CANMsg;
	CANMsg.ID = canid;
	CANMsg.LEN = (BYTE)len;
	CANMsg.MSGTYPE = PCAN_MESSAGE_STANDARD;
	for (int i = 0; i < len; i++)
	{
		CANMsg.DATA[i] = (BYTE)data[i];
	}
	stsResult=CAN_Write(m_PcanHandle, &CANMsg);
	return stsResult;**/

	HINSTANCE hDll = LoadLibrary(_T("HKMC_AdvancedSeedKey_Win32.dll"));
	KeyGenerateAPI ASK_KeyGenerate = (KeyGenerateAPI)GetProcAddress(hDll, "ASK_KeyGenerate");
	// BYTE seed[8] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 };
	// BYTE key[8];
	SEEDKEY_RT ret;
	VERSION_INFO ver;
	ret = ASK_KeyGenerate(data, key);
	if (ret) {
		printf("KEY generate fail\r\n");
	}
	else
	{
		for (int j = 0; j<8; j++) {
			printf("%x ", key[j]);
		}
	}
	FreeLibrary(hDll);
	return ret;
}
void Pcan::read(PCAN_CALLBACK cb)
{
	tmp = std::thread(std::bind(&Pcan::msgthread,this,cb));
}

void Pcan::msgthread(PCAN_CALLBACK cb)
{
}

void Pcan::set_equipment() {
}

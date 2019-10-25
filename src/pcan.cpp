#include <thread>
#include <Windows.h>
#include "pcan.h"
#include "PCANBasic.h"

TPCANHandle m_HandlesArray[59];
TPCANHandle m_PcanHandle;
int closeflag;
int endthread;
std::thread tmp;

int Pcan::initPcan(int baudrate,int hwtype,int selectedio,int selectedinterrupt,PCAN_CALLBACK cb)
{
	closeflag = 1;
	TPCANBaudrate m_Baudrate = baudrate;
	TPCANType m_HwType = hwtype;
	TPCANStatus stsResult;
	set_equipment();
	int selectedIO = selectedio;
	int selectedInterrupt = selectedinterrupt;
	int cBuffer = 5;
	int iBuffer;
	
	for (int i = 0; i < (sizeof(m_HandlesArray) / sizeof(TPCANHandle)); i++)
	{
		if (m_HandlesArray[i] > PCAN_DNGBUS1) {
			stsResult = CAN_GetValue((TPCANHandle)m_HandlesArray[i], PCAN_CHANNEL_CONDITION, (void*)&iBuffer, sizeof(iBuffer));
			if (((stsResult) == PCAN_ERROR_OK) && ((iBuffer & PCAN_CHANNEL_AVAILABLE) == PCAN_CHANNEL_AVAILABLE))
			{
				stsResult = CAN_GetValue((TPCANHandle)m_HandlesArray[i], PCAN_CHANNEL_FEATURES, (void*)&iBuffer, sizeof(iBuffer));
				if (stsResult == PCAN_ERROR_OK)
				{
					m_PcanHandle = m_HandlesArray[i];
					stsResult = CAN_Initialize(m_PcanHandle, m_Baudrate, m_HwType, selectedIO, selectedInterrupt);
					if (stsResult == PCAN_ERROR_OK)
					{
						stsResult = CAN_SetValue(m_PcanHandle, PCAN_TRACE_SIZE, (void*)&cBuffer, sizeof(cBuffer));
						if (stsResult == PCAN_ERROR_OK)
						{
							read(cb);
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

int Pcan::uninitPcan()
{
    if (!endthread) {
        closeflag = 0;
        if (tmp.joinable()) {
            tmp.join();
        }
    }
	TPCANStatus stsResult;
	stsResult=CAN_Uninitialize(m_PcanHandle);
	return stsResult;
}

int Pcan::write(const char* data,int len,int canid)
{
	TPCANStatus stsResult;
	TPCANMsg CANMsg;
	CANMsg.ID = canid;
	CANMsg.LEN = (BYTE)len;
	CANMsg.MSGTYPE = PCAN_MESSAGE_STANDARD;
	for (int i = 0; i < len; i++)
	{
		CANMsg.DATA[i] = (BYTE)data[i];
	}
	stsResult=CAN_Write(m_PcanHandle, &CANMsg);
	return stsResult;
}
void Pcan::read(PCAN_CALLBACK cb)
{
	tmp = std::thread(std::bind(&Pcan::msgthread,this,cb));
}

void Pcan::msgthread(PCAN_CALLBACK cb)
{
	TPCANMsg CANMsg;
	TPCANTimestamp CANTimeStamp;
	//CANTimeStamp.millis = 200;
	TPCANStatus stsResult;
    endthread = 0;
	do
	{
		stsResult = CAN_Read(m_PcanHandle, &CANMsg, &CANTimeStamp);
		if (stsResult == PCAN_ERROR_OK/*PCAN_ERROR_QRCVEMPTY*/) {
			TPCANMsgFD newMsg;
			newMsg = TPCANMsgFD();
			newMsg.ID = CANMsg.ID;
			newMsg.DLC = CANMsg.LEN;
			int Len = CANMsg.LEN > 8 ? 8 : CANMsg.LEN;
			for (int i = 0; i < Len; i++)
				newMsg.DATA[i] = CANMsg.DATA[i];
			newMsg.MSGTYPE = CANMsg.MSGTYPE;
            cb(newMsg.DATA, Len);
		}
	} while (closeflag);
	endthread = 1;
}

void Pcan::set_equipment() {
	m_HandlesArray[0] = PCAN_ISABUS1;
	m_HandlesArray[1] = PCAN_ISABUS2;
	m_HandlesArray[2] = PCAN_ISABUS3;
	m_HandlesArray[3] = PCAN_ISABUS4;
	m_HandlesArray[4] = PCAN_ISABUS5;
	m_HandlesArray[5] = PCAN_ISABUS6;
	m_HandlesArray[6] = PCAN_ISABUS7;
	m_HandlesArray[7] = PCAN_ISABUS8;
	m_HandlesArray[8] = PCAN_DNGBUS1;
	m_HandlesArray[9] = PCAN_PCIBUS1;
	m_HandlesArray[10] = PCAN_PCIBUS2;
	m_HandlesArray[11] = PCAN_PCIBUS3;
	m_HandlesArray[12] = PCAN_PCIBUS4;
	m_HandlesArray[13] = PCAN_PCIBUS5;
	m_HandlesArray[14] = PCAN_PCIBUS6;
	m_HandlesArray[15] = PCAN_PCIBUS7;
	m_HandlesArray[16] = PCAN_PCIBUS8;
	m_HandlesArray[17] = PCAN_PCIBUS9;
	m_HandlesArray[18] = PCAN_PCIBUS10;
	m_HandlesArray[19] = PCAN_PCIBUS11;
	m_HandlesArray[20] = PCAN_PCIBUS12;
	m_HandlesArray[21] = PCAN_PCIBUS13;
	m_HandlesArray[22] = PCAN_PCIBUS14;
	m_HandlesArray[23] = PCAN_PCIBUS15;
	m_HandlesArray[24] = PCAN_PCIBUS16;
	m_HandlesArray[25] = PCAN_USBBUS1;
	m_HandlesArray[26] = PCAN_USBBUS2;
	m_HandlesArray[27] = PCAN_USBBUS3;
	m_HandlesArray[28] = PCAN_USBBUS4;
	m_HandlesArray[29] = PCAN_USBBUS5;
	m_HandlesArray[30] = PCAN_USBBUS6;
	m_HandlesArray[31] = PCAN_USBBUS7;
	m_HandlesArray[32] = PCAN_USBBUS8;
	m_HandlesArray[33] = PCAN_USBBUS9;
	m_HandlesArray[34] = PCAN_USBBUS10;
	m_HandlesArray[35] = PCAN_USBBUS11;
	m_HandlesArray[36] = PCAN_USBBUS12;
	m_HandlesArray[37] = PCAN_USBBUS13;
	m_HandlesArray[38] = PCAN_USBBUS14;
	m_HandlesArray[39] = PCAN_USBBUS15;
	m_HandlesArray[40] = PCAN_USBBUS16;
	m_HandlesArray[41] = PCAN_PCCBUS1;
	m_HandlesArray[42] = PCAN_PCCBUS2;
	m_HandlesArray[43] = PCAN_LANBUS1;
	m_HandlesArray[44] = PCAN_LANBUS2;
	m_HandlesArray[45] = PCAN_LANBUS3;
	m_HandlesArray[46] = PCAN_LANBUS4;
	m_HandlesArray[47] = PCAN_LANBUS5;
	m_HandlesArray[48] = PCAN_LANBUS6;
	m_HandlesArray[49] = PCAN_LANBUS7;
	m_HandlesArray[50] = PCAN_LANBUS8;
	m_HandlesArray[51] = PCAN_LANBUS9;
	m_HandlesArray[52] = PCAN_LANBUS10;
	m_HandlesArray[53] = PCAN_LANBUS11;
	m_HandlesArray[54] = PCAN_LANBUS12;
	m_HandlesArray[55] = PCAN_LANBUS13;
	m_HandlesArray[56] = PCAN_LANBUS14;
	m_HandlesArray[57] = PCAN_LANBUS15;
	m_HandlesArray[58] = PCAN_LANBUS16;
}

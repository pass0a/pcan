#ifndef __PCAN_HEADER_PCAN_H__
#define __PCAN_HEADER_PCAN_H__
#include <Windows.h>
#include "PCANBasic.h"

class Pcan {
public:
    int open(int baudrate, int hwtype, int selectedio, int selectedinterrupt);
    TPCANStatus Pcan::close();
    TPCANStatus Pcan::write(TPCANMsg& CANMsg);
    TPCANStatus Pcan::read(TPCANMsg& CANMsg);
private:
    void set_equipment();
    TPCANHandle m_HandlesArray[59];
    TPCANHandle m_PcanHandle;
};
#endif // !__PCAN_HEADER_PCAN_H__

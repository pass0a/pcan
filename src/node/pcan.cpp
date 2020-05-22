#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "pcan.h"

using namespace Napi;
using namespace std;
class PcanWorker : public AsyncProgressWorker<TPCANMsg> {
public:
    PcanWorker(Function& callback)
        : AsyncProgressWorker(callback), _needRead(0), err(0), needClose(0) {
    }

    ~PcanWorker() {
        std::cout << "~PcanWorker" << std::endl;
    }
    void set_equipment() {
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
    int open(int baudrate, int hwtype, int selectedio, int selectedinterrupt)
    {
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
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        return 0;
    }
    // This code will be executed on the worker thread
    void Execute(const ExecutionProgress& progress) {
        std::unique_lock<std::mutex> lock(_cvm);
        TPCANTimestamp CANTimeStamp;
        while (!needClose)
        {
            if (_needRead <= 0) {
                _needRead = 0;
                _cv.wait(lock);
            }

            err = CAN_Read(m_PcanHandle, &CANMsg, &CANTimeStamp);
            if (err) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            else {
                _needRead--;
                progress.Send(&CANMsg, 1);
            }
        }
    }
    bool isOpened() {
        return false;
        //return gp.isOpened();
    }
    void OnOK() {
        HandleScope scope(Env());
    }

    void OnProgress(const TPCANMsg* msg, size_t  count) {
        if (msg && count) {
            if (count > 1) { printf("has more!!!!"); }
            HandleScope scope(Env());
            auto obj = Object::New(Env());
            obj.Set("id", Number::New(Env(), msg->ID));
            obj.Set("type", Number::New(Env(), msg->MSGTYPE));
            obj.Set("data", Buffer<BYTE>::New(Env(), const_cast<BYTE*>(msg->DATA), 8));
            obj.Set("dlc", Number::New(Env(), msg->LEN));
            Callback().Call({ obj });
        }
    }
    void close() {
        needClose = true;
        _cv.notify_one();
        CAN_Uninitialize(m_PcanHandle);
    }
    void read() {
        _needRead++;
        _cv.notify_one();
    }
    DWORD write(TPCANMsg& msg) {
        return CAN_Write(m_PcanHandle, &msg);
    }

private:
    TPCANHandle m_HandlesArray[59];
    TPCANHandle m_PcanHandle;
    bool needClose;
    int err;
    std::condition_variable _cv;
    std::mutex _cvm;

    int _needRead;
    TPCANMsg CANMsg;
};
std::set<PcanWorker*> glist;
int getNumber(Object& obj, const char* key, int dv) {
    return obj.Has(key) ? obj.Get(key).ToNumber() : dv;
}
Value _pcanOpen(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info[0].IsObject() && info[1].IsFunction()) {
        auto obj = info[0].ToObject();
        int baudrate = getNumber(obj, "baudrate", 0x432f),
            hardware_type = getNumber(obj, "hardware_type", 0x01),
            io_port = getNumber(obj, "io_port", 0x100),
            interrupt = getNumber(obj, "interrupt", 0x03);
        Function cb = info[1].As<Function>();
        PcanWorker *wk = new PcanWorker(cb);
        if (wk->open(baudrate, hardware_type, io_port, interrupt)) {
            wk->Queue();
            glist.insert(wk);
            return External<PcanWorker>::New(info.Env(), wk);
        }
        else {
            delete wk;
            /*Napi::Error::New(env, "cound not open this pcan port").ThrowAsJavaScriptException();*/
        }
    }
    else {
        Napi::Error::New(env, "call pcanOpen by incorrect parameter").ThrowAsJavaScriptException();
    }
    return env.Undefined();
}
Value _pcanClose(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        PcanWorker* wk = info[0].As<External<PcanWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            wk->close();
            glist.erase(wk);
        }
    }
    return env.Undefined();
}
Value _pcanIsOpened(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        auto wk = info[0].As<External<PcanWorker>>().Data();
        if (glist.find(wk) != glist.end())ret = wk->isOpened();
    }
    return Napi::Boolean::New(env, ret);
}

Value _pcanRead(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    bool ret = false;
    if (info[0].IsExternal()) {
        PcanWorker* wk = info[0].As<External<PcanWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            wk->read();
        }
    }
    return env.Undefined();
}

Value _pcanWrite(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info[0].IsExternal() && info[1].IsObject()) {
        PcanWorker* wk = info[0].As<External<PcanWorker>>().Data();
        if (glist.find(wk) != glist.end())
        {
            auto obj = info[1].ToObject();
            if (obj.Has("id") && obj.Has("data")) {
                TPCANMsg msg;
                auto data = obj.Get("data").As<Buffer<BYTE>>();
                if (data.Length() - data.ByteOffset() < 7) {
                    Napi::Error::New(info.Env(), "msg.data length less 8 bytes").ThrowAsJavaScriptException();
                }
                memcpy(msg.DATA, data.Data(), 8);
                msg.ID = getNumber(obj, "id", 0);
                msg.MSGTYPE = getNumber(obj, "type", 0);
                msg.LEN = getNumber(obj, "dlc", 8);
                Napi::Number::New(info.Env(), wk->write(msg));
            }
            else {
                Napi::Error::New(info.Env(), "pcan_send must has id and data").ThrowAsJavaScriptException();
            }
        }
        else {
            Napi::Error::New(info.Env(), "counld not find pcan handle").ThrowAsJavaScriptException();
        }
    }
    return env.Undefined();
}
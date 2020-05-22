#include <napi.h>
#include "pcan.h"
#include <iostream>
#include <set>

using namespace Napi;
//
//class PcanWorker : public AsyncProgressWorker<TPCANMsg> {
//public:
//    bool isOpened() {
//        return opened;
//    }
//    PcanWorker(Function& callback, int baud, int type, int port, int iterrupt)
//        : AsyncProgressWorker(callback), err(0), needClose(0) {
//        opened = gp.open(baud, type, port, iterrupt);
//    }
//    ~PcanWorker() {
//        gp.close();
//    }
//    // This code will be executed on the worker thread
//    void Execute(const ExecutionProgress& progress) {
//        // Need to simulate cpu heavy task
//        TPCANMsg msg;
//        while (!needClose || err != PCAN_ERROR_QRCVEMPTY)
//        {
//            err = gp.read(msg);
//            if (err) {
//                std::this_thread::sleep_for(std::chrono::milliseconds(1));
//            }
//            else {
//                progress.Send(&msg, 1);
//            }
//        }
//    }
//
//    void OnOK() {
//        HandleScope scope(Env());
//        if (err != PCAN_ERROR_QRCVEMPTY) {
//            char msg[100];
//            sprintf(msg, "PCAN Error:%d", err);
//            TypeError::New(Env(), msg).ThrowAsJavaScriptException();
//        }
//    }
//
//    void OnProgress(const TPCANMsg* msg, size_t  count) {
//        if (msg && count) {
//            if (count > 1) { printf("has more!!!!"); }
//            HandleScope scope(Env());
//            auto obj = Object::New(Env());
//            obj.Set("id", Number::New(Env(), msg->ID));
//            obj.Set("type", Number::New(Env(), msg->MSGTYPE));
//            obj.Set("data", Buffer<BYTE>::New(Env(), const_cast<BYTE*>(msg->DATA), 8));
//            obj.Set("dlc", Number::New(Env(), msg->LEN));
//            Callback().Call({ obj });
//        }
//    }
//    void close() {
//        needClose = true;
//    }
//    TPCANStatus send(TPCANMsg& msg) {
//        return gp.write(msg);
//    }
//private:
//    std::string echo;
//    Pcan gp;
//    bool needClose;
//    TPCANStatus err;
//    bool opened;
//};
//std::set<PcanWorker*> glist;
//int getNumber(Object& obj, const char* key, int dv) {
//    return obj.Has(key) ? obj.Get(key).ToNumber() : dv;
//}
//Value pcan_open(const CallbackInfo& info) {
//    int ret = -1;
//    if (info[0].IsObject() && info[1].IsFunction()) {
//        auto obj = info[0].ToObject();
//        int baudrate = getNumber(obj, "baudrate", 0x432f),
//            hardware_type = getNumber(obj, "hardware_type", 0x01),
//            io_port = getNumber(obj, "io_port", 0x100),
//            interrupt = getNumber(obj, "interrupt", 0x03);
//        Function cb = info[1].As<Function>();
//
//        PcanWorker *wk = new PcanWorker(cb, baudrate, hardware_type, io_port, interrupt);
//        if (wk->isOpened()) {
//            wk->Queue();
//            glist.insert(wk);
//            return External<PcanWorker>::New(info.Env(), wk);
//        }
//        delete wk;
//    }
//    else {
//        Napi::Error::New(info.Env(), "call cameraOpen by incorrect parameter").ThrowAsJavaScriptException();
//    }
//    return info.Env().Undefined();
//}
//Value pcan_close(const CallbackInfo& info) {
//    // We need to validate the arguments here
//
//    auto env = info.Env();
//    bool ret = false;
//    if (info[0].IsExternal()) {
//        PcanWorker* wk = info[0].As<External<PcanWorker>>().Data();
//        if (glist.find(wk) != glist.end())
//        {
//            wk->close();
//            glist.erase(wk);
//        }
//    }
//    return env.Undefined();
//}
//Value pcan_send(const CallbackInfo& info) {
//    // We need to validate the arguments here
//    if (info[0].IsExternal() && info[1].IsObject()) {
//        PcanWorker* wk = info[0].As<External<PcanWorker>>().Data();
//        if (glist.find(wk) != glist.end())
//        {
//            auto obj = info[1].ToObject();
//            if (obj.Has("id") && obj.Has("data")) {
//                TPCANMsg msg;
//                auto data = obj.Get("data").As<Buffer<BYTE>>();
//                if (data.Length() - data.ByteOffset() < 7) {
//                    Napi::Error::New(info.Env(), "msg.data length less 8 bytes").ThrowAsJavaScriptException();
//                }
//                memcpy(msg.DATA, data.Data(), 8);
//                msg.ID = getNumber(obj, "id", 0);
//                msg.MSGTYPE = getNumber(obj, "type", 0);
//                msg.LEN = getNumber(obj, "dlc", 8);
//                wk->send(msg);
//            }
//            else {
//                Napi::Error::New(info.Env(), "pcan_send must has id and data").ThrowAsJavaScriptException();
//            }
//        }
//        else {
//            Napi::Error::New(info.Env(), "counld not find pcan handle").ThrowAsJavaScriptException();
//        }
//    }
//    return Number::New(info.Env(), 0);
//}
Object init(Env env, Object exports) {
    exports["open"] = Function::New(env, _pcanOpen);
    exports["close"] = Function::New(env, _pcanClose);
    exports["read"] = Function::New(env, _pcanRead);
    exports["write"] = Function::New(env, _pcanWrite);
    return exports;
}
NODE_API_MODULE(cvip, init)
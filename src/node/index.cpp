#include <napi.h>
#include "pcan.h"

using namespace Napi;

class PcanWorker : public AsyncProgressWorker<TPCANMsg> {
public:
    PcanWorker(Function& callback, int baud, int type, int port, int iterrupt)
        : AsyncProgressWorker(callback), err(0), needClose(0) {
        gp.open(baud, type, port, iterrupt);
    }

    ~PcanWorker() {
        gp.close();
    }
    // This code will be executed on the worker thread
    void Execute(const ExecutionProgress& progress) {
        // Need to simulate cpu heavy task
        TPCANMsg msg;
        while (!needClose)
        {
            err = gp.read(msg);
            if (err)std::this_thread::sleep_for(std::chrono::seconds(20));
            progress.Send(&msg, 1);
        }
        printf("Error:%d", err);
        /*for (uint32_t i = 0; i < 2; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }*/
    }

    void OnOK() {
        HandleScope scope(Env());
        Callback().Call({ Env().Null(), String::New(Env(), "over") });
    }

    void OnProgress(const TPCANMsg* msg, size_t /* count */) {
        HandleScope scope(Env());
        Callback().Call({ Number::New(Env(), msg->ID) });
    }
    void close() {
        needClose = true;
    }
    TPCANStatus send(TPCANMsg& msg) {
        return gp.write(msg);
    }
private:
    std::string echo;
    Pcan gp;
    bool needClose;
    TPCANStatus err;
};
PcanWorker* wk;
int getNumber(Object& obj, const char* key, int dv) {
    return obj.Has(key) ? obj.Get(key).ToNumber() : dv;
}
Value pcan_open(const CallbackInfo& info) {
    int ret = -1;
    if (info[0].IsObject() && info[1].IsFunction()) {
        auto obj = info[0].ToObject();
        int baudrate = getNumber(obj, "baudrate", 0x432f),
            hardware_type = getNumber(obj, "hardware_type", 0x01),
            io_port = getNumber(obj, "io_port", 0x100),
            interrupt = getNumber(obj, "interrupt", 0x03);
        Function cb = info[1].As<Function>();
        if (!wk) {
            wk = new PcanWorker(cb, baudrate, hardware_type, io_port, interrupt);
            wk->Queue();
            ret = 0;
        }
    }
    return Number::New(info.Env(), ret);
}
Value pcan_close(const CallbackInfo& info) {
    // We need to validate the arguments here

    if (wk)wk->close();
    return info.Env().Undefined();
}
Value pcan_send(const CallbackInfo& info) {
    // We need to validate the arguments here
    if (info[0].IsObject()) {
        auto obj = info[0].ToObject();
        TPCANMsg msg;
        if (wk)wk->send(msg);
    }
    return Number::New(info.Env(), 0);
}
Object init(Env env, Object exports) {
    exports["open"] = Function::New(env, pcan_open);
    exports["close"] = Function::New(env, pcan_close);
    exports["send"] = Function::New(env, pcan_send);
    return exports;
}
NODE_API_MODULE(cvip, init)
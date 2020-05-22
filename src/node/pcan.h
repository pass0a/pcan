#ifndef __PCAN_HEADER_PCAN_H__
#define __PCAN_HEADER_PCAN_H__
#include <Windows.h>
#include "PCANBasic.h"
#include <napi.h>
using namespace Napi;

Value _pcanOpen(const Napi::CallbackInfo& info);
Value _pcanClose(const Napi::CallbackInfo& info);
Value _pcanIsOpened(const Napi::CallbackInfo& info);
Value _pcanRead(const Napi::CallbackInfo& info);
Value _pcanWrite(const Napi::CallbackInfo& info);

#endif // !__PCAN_HEADER_PCAN_H__

'use strict';

Object.defineProperty(exports, '__esModule', { value: true });

if ('win32' == process.platform) {
    process.env.Path = process.env.Path + ';' + __dirname;
}
else {
    process.env.Path = process.env.Path;
}
var pcan = require('./pcan.passoa');

function initPcan(info,fn) {
    return pcan.init(info.baudrate,info.hardware_type,info.io_port,info.interrupt,function(ev,data){
        fn(ev,data);
    });
}
function uninitPcan() {
    return pcan.uninit();
}
function send(data,id) {
    return pcan.send(data,id);
}

exports.initPcan = initPcan;
exports.uninitPcan = uninitPcan;
exports.send = send;

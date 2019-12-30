if ('win32' == process.platform) {
	process.env.Path = process.env.Path + ';' + __dirname;
} else {
	process.env.Path = process.env.Path;
}
var pcan = require('./pcan.passoa');

interface PcanInfo {
	baudrate: number;
	hardware_type: number;
	io_port: number;
	interrupt: number;
}
export function initPcan(info: PcanInfo, cb: (ev: string, data: Buffer) => void) {
	return pcan.init(info.baudrate, info.hardware_type, info.io_port, info.interrupt, cb);
}
export function uninitPcan() {
	return pcan.uninit();
}
export function send(data: Buffer, id: number) {
	return pcan.send(data, id);
}
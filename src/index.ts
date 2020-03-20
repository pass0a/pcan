if ('win32' == process.platform) {
	process.env.Path = process.env.Path + ';' + __dirname;
} else {
	process.env.Path = process.env.Path;
}
var pcan = require('./pcan.node');

interface PcanInfo {
	baudrate: number;
	hardware_type: number;
	io_port: number;
	interrupt: number;
}
export function open(info: PcanInfo, cb: (id: number, type: number, ev: Buffer, dlc: number) => void) {
	console.log(info);
	return pcan.open(info, cb);
}
export function close() {
	return pcan.close();
}
export function send(data: Buffer, id: number) {
	return pcan.send(data, id);
}

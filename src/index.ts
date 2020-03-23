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
interface PCANMsg {
	id: number;
	type: number;
	msg: Buffer;
	dlc: number;
}
export function open(info: PcanInfo, cb: (msg: PCANMsg) => void) {
	return pcan.open(info, cb);
}
export function close() {
	return pcan.close();
}
export function send(msg: PCANMsg) {
	return pcan.send(msg);
}

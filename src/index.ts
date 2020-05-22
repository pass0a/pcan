import { Duplex } from 'stream';

var bindings = require('bindings')('pcan');

export interface PcanInfo {
	baudrate: number;
	hardware_type: number;
	io_port: number;
	interrupt: number;
}
export interface PCANMsg {
	id: number;
	type?: number;
	data: Buffer;
	dlc?: number;
}
export class Pcan extends Duplex {
	private inst: any = null;
	constructor(info: PcanInfo) {
		super({ objectMode: true });
		this.inst = bindings.open(info, (data: any) => {
			this.push(data);
		});
	}
	_read(size: number) {
		if (this.inst) {
			bindings.read(this.inst);
		} else {
			this.push(null);
		}
	}
	_destroy() {
		bindings.close(this.inst);
	}
	_final() {}
	_write(chunk: any, encoding: string, next: () => void) {
		bindings.write(this.inst, chunk);
		next();
	}
	isOpened() {
		return this.inst ? true : false;
	}
}

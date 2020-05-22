import { Pcan } from '../src/index';
function sleep(time: number) {
	return new Promise((resolve, reject) => {
		setTimeout(() => {
			resolve(0);
		}, time);
	});
}
async function main() {
	let tmp;
	for (let index = 0; index < 10; index++) {
		tmp = new Pcan({ baudrate: 0x432f, hardware_type: 1, interrupt: 3, io_port: 0x100 });
		tmp.on('data', (msg) => {
			console.log(msg);
		});
		var data = Buffer.from([ 0x10, 0xaa, 0x11, 0x21, 0xa3, 0x2f ]);
		tmp.write({ id: 0, data: data });
		console.log('---->', tmp.isOpened());
		await sleep(2000);
		console.log('destory');
		tmp.destroy();
	}
}
main();

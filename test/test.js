try {
	var pcan = require('../lib');
	var tmp = Buffer.from([ 0x10, 0xaa, 0x11, 0x21, 0xa3, 0x2f ]);
	var config = {
		baudrate: 0x432f,
		hardware_type: 0x01,
		io_port: 0x100,
		interrupt: 3
	};
	console.log(
		pcan.open(config, function(msg) {
			console.log(msg);
			// console.log(data[0]);
			// console.log(data[1]);
			// console.log(data[2]);
			// console.log(data[3]);
			// console.log(data[4]);
		})
	);
	pcan.send({ id: 0, data: tmp });
	setTimeout(() => {
		pcan.close();
	}, 2000);
} catch (e) {
	console.log(e.stack);
}

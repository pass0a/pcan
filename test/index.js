var pcan = require("pcan.passoa");
var tmp=new Buffer([0x10,0xAA,0x11,0x21,0xA3,0x2F]);
console.log(pcan.init(0x001C, 0x01,0x100,3,function(ev,data){
    console.log(ev,data);
    console.log(data[0]);
    console.log(data[1]);
    console.log(data[2]);
    console.log(data[3]);
    console.log(data[4]);
}));
pcan.send(tmp);
pcan.send(tmp);
console.log("!!!!");
sleep(5000);
pcan.uninit();
// pcan.uninit();

typedef int (*PCAN_CALLBACK)(unsigned char*, int);
class Pcan {
public:
	int initPcan(int baudrate, int hwtype, int selectedio, int selectedinterrupt,PCAN_CALLBACK cb);
	int uninitPcan();
	int write(const char* data,int len,int canid);
	void read(PCAN_CALLBACK cb);
private:
	void msgthread(PCAN_CALLBACK cb);
	void set_equipment();
};
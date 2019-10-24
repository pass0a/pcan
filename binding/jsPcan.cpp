#include "jsPcan.h"
#include "../src/pcan.h"
#include "memory"

extern struct pa_plugin gp;
Pcan p;
int ref_;
pa_context* ctx_;
void* g_ioptr=0;
void onReaded(const void* buf, int len);
void bindRef();

class stRead:public FnPtr {
public:
    stRead(unsigned char* buf, int len) {
        buf_ = new unsigned char[len];
		memcpy(buf_, buf, len);
        len_ = len;
    }
    virtual void run() {
        onReaded(buf_,len_);
    }
    virtual ~stRead() {
        delete buf_;
    }
private:
    unsigned char* buf_;
    int len_;
};
int pcan_callback(unsigned char* buf,int len) {
    gp.post_io(new stRead(buf, len));
	return 0;
}
int init(pa_context* ctx) {
	ctx_ = ctx;
    g_ioptr=gp.keep_io();
	bindRef();
	int ret=0;
	if (gp.is_number(ctx, 0) && gp.is_number(ctx, 1) && gp.is_number(ctx, 2) && gp.is_number(ctx, 3))
	{
		ret = p.initPcan(gp.get_int(ctx, 0), gp.get_int(ctx, 1), gp.get_int(ctx, 2), gp.get_int(ctx, 3),pcan_callback);
	}
	gp.push_int(ctx, ret);
	return 1;
}
void __uninit() {
    int ret = p.uninitPcan();
    gp.release_io(g_ioptr);
}
int uninit(pa_context* ctx)
{
	int ret = p.uninitPcan();
    gp.release_io(g_ioptr);
    gp.push_int(ctx, ret);
	return 1;
}

int send(pa_context* ctx) {
	int len = 0;
	BYTE* data = NULL;
	BYTE key[8];
	if (gp.is_buffer_data(ctx,0))
	{
		data = (BYTE*)gp.get_buffer_data(ctx, 0, &len);
		p.write(data, key);
		void *ptr=gp.push_fixed_buffer(ctx,8);
		memcpy(ptr, key, 8);
	}
	return 1;
}

void onReaded(const void* buf, int len) {
	gp.eval_string(ctx_, "(passoa_callbacks.call.bind(passoa_callbacks))");
	gp.push_int(ctx_, ref_);
	gp.push_string(ctx_, "data");
	gp.push_external_buffer(ctx_, (void*)buf, len);
	//gp.config_buffer(ctx_, -1, );
	gp.push_int(ctx_, len);
	gp.call(ctx_, 4);
	gp.pop(ctx_);
}

void bindRef() {
	gp.eval_string(ctx_, "(passoa_callbacks.push.bind(passoa_callbacks))");
	gp.dup(ctx_, -2);
	gp.call(ctx_, 1);
	if (gp.is_number(ctx_, -1)) {
		ref_ = gp.get_int(ctx_, -1);
	}
	gp.pop(ctx_);
}
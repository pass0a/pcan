#include "plugin.h"
#include <iostream>
#include "../src/pcan.h"
#include "jsPcan.h"
struct pa_plugin gp;

static const pa_function_list_entry my_module_funcs[] = {
    { "init", init, PA_VARARGS /*nargs*/ },
    { "send", send, PA_VARARGS /*nargs*/ },
    { "uninit", uninit, PA_VARARGS /*nargs*/ },
    { NULL, NULL, 0 }
};

int passoa_init(pa_plugin p) {
    gp = p;
	gp.put_function_list(p.ctx, -1, my_module_funcs);
	return 0;
}
int passoa_exit() {
    __uninit();
	return 0;
}
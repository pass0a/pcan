#include "jsLoop.h"

boost::asio::io_service* gio = NULL;

boost::asio::io_service& jsGetLoop() {
	return *gio;
}
void jsSetLoop(boost::asio::io_service* io) {
	gio = io;
}
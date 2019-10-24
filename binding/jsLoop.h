#include <boost/asio.hpp>

boost::asio::io_service& jsGetLoop();
void jsSetLoop(boost::asio::io_service* io);
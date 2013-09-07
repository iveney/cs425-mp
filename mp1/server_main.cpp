#include <iostream>
#include <boost/asio.hpp>
#include "server_main.h"
#include "server.h"

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;
    Server server(io_service);
    io_service.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

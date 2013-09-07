#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "client_main.h"
#include "client.h"

// TODO: implement command arguments to support:
// 1. pattern, required
// 2. log file list, required
// 3. configuration file (contains machine IP/hostname, optional)
//
// sample usage:
// ./dgrep [-c conf] -p "pattern" machine.1.log [machine.2.log ...]
int main(int argc, char *argv[]) {
  
  // Parse log file list
  // TODO: Need syntax checker (coz MP1 doc says naming is "machine.i.log")
  try {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    // DEMO: call the server to system call 'ls' and return the content to client
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);

    // TODO: make the port a parameter
    tcp::resolver::query query(argv[1], "12345");
    tcp::resolver::iterator iterator = resolver.resolve(query);

    std::string fn("TODO");
    Client client(fn, io_service, iterator);
    client.close();

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

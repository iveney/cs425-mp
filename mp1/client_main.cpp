#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "client_main.h"
#include "client.h"
#include "types.h"
#include "util.hpp"

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
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);

    const char* hosts[] = {
                        "localhost",
                        "linux-v1.ews.illinois.edu",
                        "linux-a1.ews.illinois.edu",
                        "linux-a2.ews.illinois.edu",
                        "linux-a3.ews.illinois.edu",
                       };
    const int NSERVERS = sizeof(hosts) / sizeof(char*);
    std::vector<ClientPtr> clients;
    std::string pattern("zigang");

    for(int i = 0; i < NSERVERS; i++) {
      tcp::resolver::query query(hosts[i], "12345");
      tcp::resolver::iterator iterator = resolver.resolve(query);
      std::string filename;
      filename = "machine." + to_string(i) + ".log";
      clients.push_back(ClientPtr(new Client(filename, pattern,
                                          io_service, iterator)));
    }

    io_service.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

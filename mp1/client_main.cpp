#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "client_main.h"
#include "client.h"
#include "types.h"
#include "util.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using namespace boost;
namespace po = boost::program_options;

// sample usage:
// 
int main(int argc, char *argv[]) {

  // Parse log file list
  try {

    string pattern;
    vector<string> filenames;
    string config;

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("config,c", po::value< string >(&config)->
       default_value("config"), "configuration file (optional)")
      ("pattern,p", po::value< string >(&pattern), "pattern")
      ("filenames", po::value< vector<string> >(&filenames),
       "log files to be searched")
      ;

    po::positional_options_description p;
    p.add("filenames", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help") || !vm.count("pattern") || !vm.count("filenames")) {
      cout << "Usage: ./dgrep [-c conf] -p <pattern> machine.1.log [machine.2.log ...]\n";
      cout << desc;
      return 1;
    }

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
}

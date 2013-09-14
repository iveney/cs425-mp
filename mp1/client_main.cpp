#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "client_main.h"
#include "client.h"
#include "name_parser.h"
#include "types.h"
#include "util.hpp"

using namespace std;
using namespace boost;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  string pattern;
  vector<string> filenames;
  string config;
  string port;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("config,c", po::value< string >(&config)->
     default_value("config"), "configuration file (optional)")
    ("port,d", po::value<string>(&port)->
     default_value("12345"), "port (optional)")
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
    cout << "Usage: ./dgrep [-c conf] [-d port] -p <pattern> machine.1.log [machine.2.log ...]\n";
    cout << desc;
    return 1;
  }

  vector<int> host_ids(filenames.size());
  NameParser nparser(config);
  bool result = nparser.getIds(filenames, host_ids);
  if (!result) {
    cerr << "Error in filenames." << endl;
    return 2;
  }

  try {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    vector<ClientPtr> clients;

    const vector<string>& hosts = nparser.hosts();
    for (int i = 0; i < filenames.size(); i++) {
      const string& hostname = hosts[host_ids[i]];
      cout << filenames[i] << " @ " << hostname << endl;

      tcp::resolver::query query(hostname.c_str(), port);
      tcp::resolver::iterator iterator = resolver.resolve(query);
      clients.push_back(ClientPtr(new Client(filenames[i], pattern,
              io_service, iterator)));
    }

    io_service.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

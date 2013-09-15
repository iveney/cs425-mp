#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "client_main.h"
#include "client.h"
#include "name_parser.h"
#include "types.h"
#include "query.h"
#include "util.hpp"

using namespace std;
using namespace boost;
namespace po = boost::program_options;
const char* usage =
"Usage: ./dgrep [-c conf] [-d port] [-t type] -p <pattern> <logs>\n";

int main(int argc, char *argv[]) {
  vector<string> filenames;
  string pattern;
  string config;
  string port;
  string type;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("config,c", po::value< string >(&config)->
     default_value("config"), "configuration file")
    ("port,d", po::value<string>(&port)->
     default_value("12345"), "port")
    ("type,t", po::value<string>(&type)->
     default_value("both"), "query type: key, value or both (default)")
    ("pattern,p", po::value< string >(&pattern)->required(), "pattern")
    ;

  // --filenames should be hidden
  po::options_description hidden("Hidden options");
  hidden.add_options()
    ("filenames", po::value< vector<string> >(&filenames)->required(),
     "log files to be searched")
    ;

  po::options_description cmd_opts;
  cmd_opts.add(desc).add(hidden);

  // file names are positional
  po::positional_options_description p;
  p.add("filenames", -1);

  try {
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(cmd_opts).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      cout << usage;
      cout << desc;
      return 0;
    }

    // validate "type"
    Query::QUERY_TYPE query_type;
    if (type == "both") {
      query_type = Query::BOTH;
    } else if (type == "key") {
      query_type = Query::KEY;
    } else if (type == "value") {
      query_type = Query::VALUE;
    } else {
      using namespace po;
      throw validation_error(validation_error::invalid_option_value);
    }

    // read configuration
    vector<int> host_ids(filenames.size());
    NameParser nparser(config);
    bool result = nparser.getIds(filenames, host_ids);
    if (!result) {
      cerr << "Error in filenames." << endl;
      return 1;
    }

    // create connections to each servers
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    vector<ClientPtr> clients;

    const vector<string>& hosts = nparser.hosts();
    for (int i = 0; i < filenames.size(); i++) {
      const string& hostname = hosts[host_ids[i]];
      const string& filename = filenames[i];
      cout << filename << " @ " << hostname << endl;

      Query send_query(query_type, pattern, filename);
      tcp::resolver::query query(hostname.c_str(), port);
      tcp::resolver::iterator iterator = resolver.resolve(query);
      clients.push_back(ClientPtr(new Client(send_query, io_service, iterator)));
    }

    io_service.run();
  } catch (po::validation_error& ve) {
    cerr << "Error: Type must be one of these: key, value, both" << endl;
    cout << usage;
    cout << desc;
    return 1;
  } catch (po::required_option& e) {
    cout << usage;
    cout << desc;
    return 0;
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

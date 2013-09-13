#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "client_main.h"
#include "client.h"
#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

using namespace std;

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
        
        if (argc < 2)
        {
            cerr << "Usage: client <host>" << endl;
            return 1;
        }
        
        std::string pattern;
        std::vector<std::string> filenames;
        std::string config;
        
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help", "produce help message")
        ("config,c", po::value< std::string >(&config)->default_value("config.txt"), "configuration file (optional)")
        ("pattern,p", po::value< std::string >(&pattern), "pattern")
        ("filenames", po::value< std::vector<std::string> >(&filenames), "log files to be searched")
        ;
        
        po::positional_options_description p;
        p.add("filenames", -1);
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);
        
        if (vm.count("help")) {
            cout << "Usage: options_description [options]\n";
            cout << desc;
            return 0;
        }
        if (vm.count("pattern") && vm.count("filenames"))
        {
            cout << "using config file: " << config << "\n";
            
            // DEMO: call the server to system call 'ls' and return the content to client
            boost::asio::io_service io_service;
            
            tcp::resolver resolver(io_service);
            
            // TODO: make the port a parameter
            tcp::resolver::query query("localhost", "12345");
            tcp::resolver::iterator iterator = resolver.resolve(query);
            
            std::string fn("TODO");
            Client client(fn, io_service, iterator);
            io_service.run();
            
            return 0;
        }
        
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    } 
    
}

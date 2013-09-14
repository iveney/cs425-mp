#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "client_main.h"
#include "client.h"
#include "name_parser.h"
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
        
        string pattern;
        vector<string> filenames;
        string config;
        
        po::options_description desc("Allowed options");
        desc.add_options()
        ("help", "produce help message")
        ("config,c", po::value< string >(&config)->default_value("config.txt"), "configuration file (optional)")
        ("pattern,p", po::value< string >(&pattern), "pattern")
        ("filenames", po::value< vector<string> >(&filenames), "log files to be searched")
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
            cout << "using pattern: " <<  pattern << "\n";
            
            vector<int> machineIndices(filenames.size());
            
            NameParser nparser;
            bool result;
            
            //Read in the configuration file 
            result = nparser.readInConfig(config);
            if(!result)
            {
                cout << "Error reading configuration file" << endl;
            }
            
            //Print the addresses read in from the config file
            string addr;
            for(int i = 0; i<4; i++)
            {
                result = nparser.getAddress(i, addr);
                if(!result)
                {
                    cout<< "error getting address for " << i << endl; 
                }
                cout << "Machine " << i << ": " << addr << "\n";
            }
            
            //Get the IDs of the machines requested for grep
            result = nparser.getIds(filenames, machineIndices);
            if(!result)
            {
                cout<<"Filenames incorrect, please check that all are of the format machine.i.log and that there are no more that 4 files listed"<<"\n";
            }
            
            cout << "Indices of queried logs: ";
            
            for(int i = 0; i<machineIndices.size(); i++)
            {
                cout << machineIndices[i] << " ";
            }
            
            cout << "\n";
            
            // DEMO: call the server to system call 'ls' and return the content to client
            boost::asio::io_service io_service;
            
            tcp::resolver resolver(io_service);
            
            // TODO: make the port a parameter
            tcp::resolver::query query("localhost", "12345");
            tcp::resolver::iterator iterator = resolver.resolve(query);
            
            string fn("TODO");
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

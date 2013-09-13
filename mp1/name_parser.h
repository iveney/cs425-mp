#ifndef _NAME_PARSER_H_
#define _NAME_PARSER_H_

#include <string>
#include <vector>
#include <fstream>
using namespace std;

class NameParser {
public:
    NameParser();
    bool getIds(vector<string> filenames, vector<int> & result);
    bool getMachineNumber(string filename, int & machineNumber);
    bool readInConfig(std::string filename);
    bool getAddress(int index, string& address);
    
private:
    vector<string> addresses;
    
};

#endif /* end of include guard: _NAME_PARSER_H_ */


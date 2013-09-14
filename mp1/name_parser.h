#ifndef _NAME_PARSER_H_
#define _NAME_PARSER_H_

#include <string>
#include <vector>
#include <fstream>
using namespace std;

class NameParser {
public:
    NameParser(const string& config);
    bool getIds(const vector<string>& filenames, vector<int> & result);
    bool getMachineNumber(const string& filename, int& machineNumber);
    bool readInConfig(const string& filename);
    const vector<string>& hosts() const {return hosts_;};
    
private:
    vector<string> hosts_;
};

#endif /* end of include guard: _NAME_PARSER_H_ */


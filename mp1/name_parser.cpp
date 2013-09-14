//
//  name_parser.cpp
//  
//
//  Created by Genevieve LaBelle on 9/11/13.
//  Copyright (c) 2013 University of Illinois Urbana-Champaign. All rights reserved.
//

#include <iostream>
#include <string>
#include "name_parser.h"

using namespace std;

NameParser::NameParser(const string& config) {
  hosts_.push_back("localhost");
  readInConfig(config);
}

// Each line is an IP/hostname
bool NameParser::readInConfig(const string& filename) {
    ifstream configFile(filename.c_str());
    if (configFile) {
      string line;
      while ( getline (configFile, line) ) {
        hosts_.push_back(line);
      }
      configFile.close();
      return true;
    }
    return false;
}

/* assumptions:
 * filenames is of the form "machine.i.log machine.i.log ..."
 * limiting number of files to 4 here
 */
bool NameParser::getIds(const vector<string>& filenames, vector<int>& result)
{
  int machineNumber;
  string address;

  int numFiles = filenames.size();

  for(int index=0; index<numFiles; index++) {
    if(getMachineNumber(filenames[index], machineNumber)) {
      result[index] = machineNumber;
    } else {
      //error, bad file name
      return false;
    }
  }
  return true;
}

bool NameParser::getMachineNumber(const string& filename, int& machineNumber) {
  // sample: machine.3.log
  //         0       8 10
  string machine_part = filename.substr(0, 7);
  string idx_part     = filename.substr(8, 1);
  string log_part     = filename.substr(10, 3);
  if (machine_part != "machine" || log_part != "log") {
    cerr << "File name '" << filename << "' invalid!" << endl;
    return false;
  }

  if(sscanf(idx_part.c_str(), "%d", &machineNumber) != 1)
    return false;

  return true;
}

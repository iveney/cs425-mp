//
//  name_parser.cpp
//  
//
//  Created by Genevieve LaBelle on 9/11/13.
//  Copyright (c) 2013 University of Illinois Urbana-Champaign. All rights reserved.
//

#include <iostream>
#include "name_parser.h"

using namespace std;

NameParser::NameParser()
{
    //open file from filepath with machine names and addresses
    //read names into names and addresses into ids
    addresses.resize(4);
    addresses[0].assign("empty");
}

bool NameParser::readInConfig(std::string filename)
{   
    char * cstr = new char [filename.length()+1];
    std::strcpy (cstr, filename.c_str());
    
    cout << cstr << endl;
    string line;
    ifstream configFile(cstr);
    int index = 0;
    if (configFile.is_open())
    {
        while ( index <= 3 && getline (configFile,line) )
        {
            addresses[index].assign(line);
            index++;
        }
        configFile.close();
        return true;
    }
    return false;
}

bool NameParser::getAddress(int index, string& address)
{
    if(index > 3)
    {
        return false;
    }
    address.assign(addresses[index]);
    return true;
}

//eventually return error

/* assumptions:
 * filenames is of the form "machine.i.log machine.i.log ..."
 * limiting number of files to 4 here
 */
bool NameParser::getIds(vector<std::string> filenames, vector<int>& result)
{
    int machineNumber;
    std::string address;
    
    int numFiles = filenames.size();
    
    if(numFiles < 5)
    {
        for(int index=0; index<numFiles; index++)
        {
            if(getMachineNumber(filenames[index], machineNumber))
            {
                result[index] = machineNumber;
            }
            else
            {
                //error, bad file name
                return false;
            }
        }
        
    }
    else
    {
        //return an error, too many filenames
        return false;
    }
    
    return true;
}

bool NameParser::getMachineNumber(std::string filename, int & machineNumber)
{
    string fname;
    
    fname = "machine.";
    fname += "1";
    fname += ".log";
    if(fname.compare(filename) == 0)
    {
        machineNumber = 0;
        return true;
    }
    
    fname = "machine.";
    fname += "2";
    fname += ".log";
    if(fname.compare(filename) == 0)
    {
        machineNumber = 1;
        return true;
    }
    
    fname = "machine.";
    fname += "3";
    fname += ".log";
    if(fname.compare(filename) == 0)
    {
        machineNumber = 2;
        return true;
    }
    
    fname = "machine.";
    fname += "4";
    fname += ".log";
    if(fname.compare(filename) == 0)
    {
        machineNumber = 3;
        return true;
    }
    
    return false;
}
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "gtest/gtest.h"
#include "query.h"
#include "client.h"
#include "name_parser.h"

using namespace std;

namespace {

// the following tests assumes "provided" is linked to "logs/run".
// golden file under "logs/run/golden"
class GrepTest : public ::testing::Test {
 protected:
  GrepTest() {
    // read the configuration file
    string config = "config";
    NameParser nparser(config);
    hosts_  = nparser.hosts();
    port_   = "12345";
    golden_ = "logs/run/golden";
    null_.open("/dev/null");
  }

  string file_as_string(string filename) {
    ifstream ifs(filename.c_str());
    stringstream gold;
    gold << ifs.rdbuf();
    return gold.str();
  }

  // Objects declared here can be used by all tests in the test case for Foo.
  vector<string> hosts_;  // host mappings
  string port_;
  string golden_;
  ofstream null_;
};

// grep the keys from two hosts
TEST_F(GrepTest, GrepKeyTest) {
  string key = "two";
  Query query[] = {Query(), // since host[0] = localhost
                   Query(Query::KEY, key, "machine.1.log"),
                   Query(Query::KEY, key, "machine.2.log")};

  // create connections to each servers
  boost::asio::io_service io_service;
  vector<ClientPtr> clients;
  clients.push_back(
      ClientPtr(new Client(query[1], hosts_[1], port_, io_service, null_)));
  clients.push_back(
      ClientPtr(new Client(query[2], hosts_[2], port_, io_service, null_)));
  io_service.run();

  // read in golden files
  string prefix = golden_ + "/GrepKeyTest";
  string f1 = prefix + "/1";
  string f2 = prefix + "/2";
  string gold[2];
  gold[0] = file_as_string(f1);
  gold[1] = file_as_string(f2);

  // compare with golden
  EXPECT_EQ(clients[0]->result(), gold[0]);
  EXPECT_EQ(clients[1]->result(), gold[1]);
}

TEST_F(GrepTest, GrepValueTest) {
  string value = "by quoting him but i have to risk it because his email was such a";
  Query query[] = {Query(), // since host[0] = localhost
                   Query(Query::VALUE, value, "machine.1.log"),
                   Query(Query::VALUE, value, "machine.2.log")};

  // create connections to each servers
  boost::asio::io_service io_service;
  vector<ClientPtr> clients;
  clients.push_back(
      ClientPtr(new Client(query[1], hosts_[1], port_, io_service, null_)));
  clients.push_back(
      ClientPtr(new Client(query[2], hosts_[2], port_, io_service, null_)));
  io_service.run();

  // read in golden files
  string prefix = golden_ + "/GrepValueTest";
  string f1 = prefix + "/1";
  string f2 = prefix + "/2";
  string gold[2];
  gold[0] = file_as_string(f1);
  gold[1] = file_as_string(f2);

  // compare with golden
  EXPECT_EQ(clients[0]->result(), gold[0]);
  EXPECT_EQ(clients[1]->result(), gold[1]);
}
} // namespace

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "query.h"
#include "types.h"
using boost::asio::ip::tcp;

class Client {
public:
  Client (Query query,
          const std::string& hostname,
          const std::string& port,
          boost::asio::io_service& io_service,
          std::ostream& os = std::cout); 

  void close();
  const std::string& result() const {return result_;}

private:
  enum {WAIT_TIME = 5};
  enum {MAX_TRIAL = 3};
  void do_connect();
  void retry();
  void handle_connect(const boost::system::error_code& error);
  void handle_write(const boost::system::error_code& error);
  void handle_read(const boost::system::error_code& error);

  Query query_;
  std::string result_;
  std::string hostname_;

  TcpConnPtr connection_; // connection to server
  tcp::resolver::iterator endpoint_it_;
  boost::asio::io_service& io_service_;

  int trial_;
  std::ostream& os_;
};

#endif /* end of include guard: _CLIENT_H_ */


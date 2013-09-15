#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include "message.h"
#include "query.hpp"

class Client {
public:
  enum { MAX_LENGTH = 8192 };

  Client (Query query,
          boost::asio::io_service& io_service, 
          tcp::resolver::iterator endpoint_iterator);

  void close();

private:
  void handle_connect(const boost::system::error_code& error);
  void handle_read(const boost::system::error_code& error);
  void handle_read_header(const boost::system::error_code& error);
  void handle_read_body(const boost::system::error_code& error);
  void do_close();

  Query query_;
  Message result_;

  boost::asio::io_service& io_service_;
  tcp::socket socket_;
};

#endif /* end of include guard: _CLIENT_H_ */


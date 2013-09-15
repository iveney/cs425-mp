#ifndef _SERVER_H_
#define _SERVER_H_

// TCP asynchronous server
//
// Author: Zigang Xiao <zxiao2@illinois.edu>
//

#include <boost/asio.hpp>
#include <string>
#include "types.h"

using boost::asio::ip::tcp;

class Server {
public:
  enum {LISTEN_PORT = 12345};
 Server(boost::asio::io_service& io_service);

private:
  void start_accept();
  void handle_accept(TcpConnPtr new_connection,
      const boost::system::error_code& error);
  void handle_read(TcpConnPtr conn, QueryPtr query,
      const boost::system::error_code& e);
  std::string do_grep(const Query& query);
  void handle_write(TcpConnPtr conn, const boost::system::error_code& e);

  tcp::acceptor acceptor_;
};

#endif /* end of include guard: _SERVER_H_ */


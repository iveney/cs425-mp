#ifndef _SERVER_H_
#define _SERVER_H_

// TCP asynchronous server
//
// Author: Zigang Xiao <zxiao2@illinois.edu>
//

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include "types.h"

class Server {
public:
  const unsigned short LISTEN_PORT = 12345;
  Server(boost::asio::io_service& io_service);

private:
  void start_accept();
  void handle_accept(TcpConnPtr new_connection,
      const boost::system::error_code& error);

  tcp::acceptor acceptor_;
};

#endif /* end of include guard: _SERVER_H_ */


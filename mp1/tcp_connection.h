#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include "types.h"

class TcpConnection
  : public boost::enable_shared_from_this<TcpConnection> {
public:

  static TcpConnPtr create(boost::asio::io_service& io_service) {
    return TcpConnPtr(new TcpConnection(io_service));
  }

  tcp::socket& socket() {
    return socket_;
  }

  void start();

private:
  TcpConnection(boost::asio::io_service& io_service)
    : socket_(io_service) {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/) {
  }

  tcp::socket socket_;
  std::string message_;
};

#endif /* end of include guard: _TCP_CONNECTION_H_ */


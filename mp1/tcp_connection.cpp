#include <boost/bind.hpp>
#include "tcp_connection.h"

// TODO: read the pattern and invoke grep here
void TcpConnection::start()
{
  message_ = std::string("Hello from server!");

  boost::asio::async_write(socket_, boost::asio::buffer(message_),
      boost::bind(&TcpConnection::handle_write, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

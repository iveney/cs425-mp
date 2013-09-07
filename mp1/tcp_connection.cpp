#include <boost/bind.hpp>
#include "tcp_connection.h"

void TcpConnection::start()
{
  message_ = std::string("hi");

  boost::asio::async_write(socket_, boost::asio::buffer(message_),
      boost::bind(&TcpConnection::handle_write, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

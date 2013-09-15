#include <iostream>
#include <boost/bind.hpp>
#include "tcp_connection.h"
#include "message.h"

void TcpConnection::start() {
  boost::asio::async_read(socket_,
      boost::asio::buffer(pattern_.data(), Message::header_length),
      boost::bind(&TcpConnection::handle_read_header, shared_from_this(),
        boost::asio::placeholders::error));
}

void TcpConnection::do_close() {
  socket_.close();
}

// handle_read_header and handle_read_body: reads a Message object
void TcpConnection::handle_read_header(const boost::system::error_code& error) {
  if (!error && pattern_.decode_header()) {
    boost::asio::async_read(socket_,
        boost::asio::buffer(pattern_.body(), pattern_.body_length()),
        boost::bind(&TcpConnection::handle_read_body, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    // when there are more cases, use switch:
    // http://stackoverflow.com/a/9225181/577704
    if (error.value() == boost::asio::error::eof) {
      std::cout << "Connection closed by peer.\n";
      do_close();
    } else {
      std::cerr << "[handle_read_header] " << error.message() << std::endl;
    }
  }
}

void TcpConnection::handle_read_body(const boost::system::error_code& error) {
  if (!error) {
    // now we have the pattern, do the grep
    std::cout << "Pattern received: ";
    std::cout.write(pattern_.body(), pattern_.body_length());
    std::cout << "\n";

    std::string pattern(pattern_.body());

    // send the result back to client
    Message msg(do_grep(pattern));
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg.data(), msg.length()),
        boost::bind(&TcpConnection::handle_write, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));

    // issue next read...
    boost::asio::async_read(socket_,
        boost::asio::buffer(pattern_.data(), Message::header_length),
        boost::bind(&TcpConnection::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    std::cerr << "[handle_read_body] " << error.message() << std::endl;
  }
}


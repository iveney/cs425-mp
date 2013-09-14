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

void TcpConnection::handle_read_header(const boost::system::error_code& error) {
  if (!error && pattern_.decode_header()) {
    boost::asio::async_read(socket_,
        boost::asio::buffer(pattern_.body(), pattern_.body_length()),
        boost::bind(&TcpConnection::handle_read_body, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    std::cerr << "[handle_read_header] " << error.message() << std::endl;
  }
}

void TcpConnection::handle_read_body(const boost::system::error_code& error) {
  if (!error) {
    // now we have the pattern, do the grep
    std::cout << "String received: ";
    std::cout.write(pattern_.body(), pattern_.body_length());
    std::cout << "\n";

    std::string pattern(pattern_.body());

    // send the result back to client
    std::string message = do_grep(pattern);
    boost::asio::async_write(socket_, boost::asio::buffer(message),
        boost::bind(&TcpConnection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));

    // continue reading...
    boost::asio::async_read(socket_,
        boost::asio::buffer(pattern_.data(), Message::header_length),
        boost::bind(&TcpConnection::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    std::cerr << "[handle_read_body] " << error.message() << std::endl;
  }
}

std::string TcpConnection::do_grep(const std::string& pattern) {
  std::string command;
  command = "grep " + pattern + " logs/basic/`hostname`";
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe) {
    std::cerr << "Error opening file\n";
    return "Error";
  }

  char buffer[128];
  std::string result = "";
  while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
  }

  return result;
}

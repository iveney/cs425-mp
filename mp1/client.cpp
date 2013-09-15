#include <iostream>
#include <boost/bind.hpp>
#include "client.h"

Client::Client (std::string filename,
                std::string pattern,
                boost::asio::io_service& io_service, 
                tcp::resolver::iterator endpoint_iterator)
  : filename_(filename), pattern_(pattern),
    io_service_(io_service), socket_(io_service) {
    
  boost::asio::async_connect(socket_, endpoint_iterator,
      boost::bind(&Client::handle_connect, this,
        boost::asio::placeholders::error));
}

Client::Client (Query query,
                boost::asio::io_service& io_service, 
                tcp::resolver::iterator endpoint_iterator)
  : query_(query), io_service_(io_service), socket_(io_service) {
    
  boost::asio::async_connect(socket_, endpoint_iterator,
      boost::bind(&Client::handle_connect, this,
        boost::asio::placeholders::error));
}

// upon connection, send the type, pattern and filename to server
void Client::handle_connect(const boost::system::error_code& error)
{
  if (!error) {
    tcp::endpoint endpoint = socket_.remote_endpoint();
    boost::asio::ip::address addr = endpoint.address();
    unsigned short port = endpoint.port();
    std::cout << "Connected: " << addr << ":" << port << std::endl;

    Message msg(pattern_);
    
    socket
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg.data(), msg.length()),
        boost::bind(&Client::handle_read, this,
          boost::asio::placeholders::error));
  } else {
    std::cout << "[handle_connect] " << error.message() << std::endl;
    do_close();
  }
}

// upon sending the pattern, read the content sent back by server
void Client::handle_read(const boost::system::error_code& error) {
  if (!error) {
    boost::asio::async_read(socket_,
        boost::asio::buffer(result_.data(), Message::header_length),
        boost::bind(&Client::handle_read_header, this,
          boost::asio::placeholders::error));
  } else {
    std::cout << "[handle_read] " << error.message() << std::endl;
    do_close();
  }
}

void Client::handle_read_header(const boost::system::error_code& error) {
  if (!error && result_.decode_header()) {
    boost::asio::async_read(socket_,
        boost::asio::buffer(result_.body(), result_.body_length()),
        boost::bind(&Client::handle_read_body, this, 
          boost::asio::placeholders::error));
  } else {
    std::cout << "[handle_read_header] " << error.message() << std::endl;
    do_close();
  }
}

void Client::handle_read_body(const boost::system::error_code& error) {
  if (!error && result_.decode_header()) {

    // now we have the result
    std::cout << filename_ << ":\n";
    std::cout.write(result_.body(), result_.body_length());
    std::cout << "\n";

    do_close();
  } else {
    std::cout << "[handle_read_header] " << error.message() << std::endl;
    do_close();
  }
}

void Client::close() {
  io_service_.post(boost::bind(&Client::do_close, this));
}

void Client::do_close() {
  socket_.close();
}

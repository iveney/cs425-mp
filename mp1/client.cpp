#include <iostream>
#include <boost/bind.hpp>
#include "client.h"

// TODO: extend to connect to a list of servers
Client::Client (std::string pattern,
                boost::asio::io_service& io_service, 
                tcp::resolver::iterator endpoint_iterator)
  : pattern_(pattern), io_service_(io_service), socket_(io_service) {
    
  boost::system::error_code error;
  boost::asio::connect(socket_, endpoint_iterator);
  handle_connect(error);
    
  // boost::asio::async_connect(socket_, endpoint_iterator,
  //     boost::bind(&Client::handle_connect, this,
  //       boost::asio::placeholders::error));
}

// upon connection, send the pattern to server
void Client::handle_connect(const boost::system::error_code& error)
{
  std::cout << "handle_connect\n";
  if (!error) {
    boost::asio::async_write(socket_,
        boost::asio::buffer(pattern_),
        boost::bind(&Client::handle_read, this,
          boost::asio::placeholders::error));
  } else {
    std::cout << "Error!" << std::endl;
    do_close();
  }
}

// upon sending the pattern, read the content sent back by server
void Client::handle_read(const boost::system::error_code& error) {
  if (!error) {
    boost::system::error_code e;
    socket_.read_some(boost::asio::buffer(buffer), e);
    if (e == boost::asio::error::eof)
      do_close(); // Connection closed cleanly by peer.
    else if (e)
      throw boost::system::system_error(e); // Some other error.

    std::cout << "Result is:\n" << buffer << std::endl;
    // boost::asio::async_read(socket_,
    //     boost::asio::buffer(buffer, MAX_LENGTH),
    //     boost::bind(&Client::handle_output, this,
    //       boost::asio::placeholders::error));
  } else {
    std::cout << "Error!" << std::endl;
    do_close();
  }
}

void Client::handle_output(const boost::system::error_code& error) {
  if (!error) {
    std::cout << "Result is:\n" << buffer << std::endl;
  } else {
    std::cout << "Error!" << std::endl;
    do_close();
  }
}

void Client::close() {
  io_service_.post(boost::bind(&Client::do_close, this));
}

void Client::do_close() {
  socket_.close();
}

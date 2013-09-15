#include <iostream>
#include <boost/bind.hpp>
#include "tcp_connection.hpp"
#include "client.h"

Client::Client (Query query,
                boost::asio::io_service& io_service, 
                tcp::resolver::iterator endpoint_iterator)
  : query_(query)/*, socket_(io_service)*/ {
    
  connection_ = TcpConnection::create(io_service);
  boost::asio::async_connect(connection_->socket(), endpoint_iterator,
      boost::bind(&Client::handle_connect, this,
        boost::asio::placeholders::error));
}

// upon connection, send the query to server
void Client::handle_connect(const boost::system::error_code& error)
{
  if (!error) {
    tcp::endpoint endpoint = connection_->socket().remote_endpoint();
    boost::asio::ip::address addr = endpoint.address();
    unsigned short port = endpoint.port();
    std::cout << "Connected: " << addr << ":" << port << std::endl;

    connection_->async_write(query_, 
        boost::bind(&Client::handle_write, this,
          boost::asio::placeholders::error));
    // Message msg(query_.pattern_);
    // 
    // boost::asio::async_write(socket_,
    //     boost::asio::buffer(msg.data(), msg.length()),
    //     boost::bind(&Client::handle_read, this,
    //       boost::asio::placeholders::error));
  } else {
    std::cout << "[handle_connect] " << error.message() << std::endl;
    connection_->do_close();
  }
}

// upon sending the query, read the content sent back by server
void Client::handle_write(const boost::system::error_code& error) {
  if (!error) {
    connection_->async_read(result_,
        boost::bind(&Client::handle_read, this,
          boost::asio::placeholders::error));
    // boost::asio::async_read(socket_,
    //     boost::asio::buffer(result_.data(), Message::header_length),
    //     boost::bind(&Client::handle_read_header, this,
    //       boost::asio::placeholders::error));
  } else {
    std::cout << "[handle_write] " << error.message() << std::endl;
    connection_->do_close();
  }
}

void Client::handle_read(const boost::system::error_code& error) {
  if (!error) {
    // print out the result and close connection
    std::cout << query_.filename_ << ":\n";
    std::cout << result_ << "\n";

    connection_->do_close();
  } else {
    std::cout << "[handle_read] " << error.message() << std::endl;
    connection_->do_close();
  }
}

// void Client::handle_read_header(const boost::system::error_code& error) {
//   if (!error && result_.decode_header()) {
//     boost::asio::async_read(socket_,
//         boost::asio::buffer(result_.body(), result_.body_length()),
//         boost::bind(&Client::handle_read_body, this, 
//           boost::asio::placeholders::error));
//   } else {
//     std::cout << "[handle_read_header] " << error.message() << std::endl;
//     do_close();
//   }
// }
// 
// void Client::handle_read_body(const boost::system::error_code& error) {
//   if (!error && result_.decode_header()) {
// 
//     // now we have the result
//     std::cout << query_.filename_ << ":\n";
//     std::cout.write(result_.body(), result_.body_length());
//     std::cout << "\n";
// 
//     do_close();
//   } else {
//     std::cout << "[handle_read_header] " << error.message() << std::endl;
//     do_close();
//   }
// }

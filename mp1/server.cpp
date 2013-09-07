#include <iostream>
#include <boost/bind.hpp>
#include "server.h"
#include "tcp_connection.h"

Server::Server(boost::asio::io_service& io_service)
          : acceptor_(io_service, tcp::endpoint(tcp::v4(), Server::LISTEN_PORT)) {
  start_accept();
}

void Server::start_accept()
{
  std::cout << "Listening on port " << LISTEN_PORT << std::endl;
  TcpConnPtr new_connection =
    TcpConnection::create(acceptor_.get_io_service());

  acceptor_.async_accept(new_connection->socket(),
      boost::bind(&Server::handle_accept, this, new_connection,
        boost::asio::placeholders::error));
}

void Server::handle_accept(TcpConnPtr new_connection,
    const boost::system::error_code& error)
{
  if (!error) {
    // do the real job here
    new_connection->start();
  }

  start_accept();
}

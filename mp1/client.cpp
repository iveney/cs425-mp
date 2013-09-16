#include <iostream>
#include <iomanip>
#include <boost/bind.hpp>
#include "tcp_connection.hpp"
#include "query.h"
#include "client.h"
#include "timer.hpp"
using std::cout;
using std::endl;

Client::Client (Query query,
                const std::string& hostname,
                const std::string& port,
                boost::asio::io_service& io_service,
                std::ostream& os)
  : query_(query), hostname_(hostname), io_service_(io_service), trial_(0), os_(os) {
  tcp::resolver resolver(io_service);
  tcp::resolver::query resolve_query(hostname.c_str(), port);
  endpoint_it_ = resolver.resolve(resolve_query);
  do_connect();
}

void Client::do_connect() {
  connection_ = TcpConnection::create(io_service_);
  boost::asio::async_connect(connection_->socket(), endpoint_it_,
      boost::bind(&Client::handle_connect, this,
        boost::asio::placeholders::error));
}

// upon connection, send the query to server
void Client::handle_connect(const boost::system::error_code& error)
{
  if (!error) {
    connection_->async_write(query_, 
        boost::bind(&Client::handle_write, this,
          boost::asio::placeholders::error));
  } else {
    cout << "[handle_connect] " << error.message() << endl;
    connection_->do_close();
  }
}

// upon sending the query, read the content sent back by server
void Client::handle_write(const boost::system::error_code& error) {
  if (!error) {
    connection_->async_read(result_,
        boost::bind(&Client::handle_read, this,
          boost::asio::placeholders::error));
  } else {
    connection_->do_close();
    retry();
  }
}

void Client::handle_read(const boost::system::error_code& error) {
  if (!error) {
    // print out the result and close connection
    os_ << query_.filename_ << ":\n";
    os_ << result_ << "\n";

    connection_->do_close();
  } else {
    connection_->do_close();
    retry();
  }
}

void Client::retry() {
  os_ << "Connection to " << hostname_ << " failed. \n";
  if (trial_ >= MAX_TRIAL) {
    cout << "Already reached maximum trials (" << MAX_TRIAL << ").\n";
    return;
  }

  // wait 5 seconds and reconnect
  time_t_timer timer(io_service_);
  timer.expires_from_now(WAIT_TIME);
  os_ << "Wait " << WAIT_TIME << "s to retry: " << ++trial_ << " attempts.\n";
  timer.wait();
  do_connect();
}

#include "server.h"
#include "tcp_connection.hpp"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include "query.h"
using std::string;
using std::cout;
using std::endl;

Server::Server(boost::asio::io_service& io_service)
          : acceptor_(io_service, tcp::endpoint(tcp::v4(), Server::LISTEN_PORT)) {
  cout << "Listening on port " << LISTEN_PORT << endl;
  start_accept();
}

void Server::start_accept()
{
  TcpConnPtr connection =
    TcpConnection::create(acceptor_.get_io_service());

  acceptor_.async_accept(connection->socket(),
      boost::bind(&Server::handle_accept, this, connection,
        boost::asio::placeholders::error));
}

void Server::handle_accept(TcpConnPtr connection,
    const boost::system::error_code& error)
{
  if (!error) {
    tcp::endpoint endpoint = connection->socket().remote_endpoint();
    boost::asio::ip::address addr = endpoint.address();
    unsigned short port = endpoint.port();
    cout << "Connected: " << addr << ":" << port << endl;

    // Successfully established connection. Start operation to read the query
    QueryPtr query(new Query());
    connection->async_read(*query,
        boost::bind(&Server::handle_read, this,
          connection, query,
          boost::asio::placeholders::error));
  } else {
    std::cerr << "[handle_accept] " << error.message() << endl;
  }

  // go back to accept new queries
  start_accept();
}

void Server::handle_read(TcpConnPtr conn, QueryPtr query,
    const boost::system::error_code& e) {
  if (!e) {
    // now we have the pattern, do the grep
    cout << *query << endl;

    string result = do_grep(*query);

    conn->async_write(result,
        boost::bind(&Server::handle_write, this, conn,
          boost::asio::placeholders::error));
    // when there are more cases, use switch:
    // http://stackoverflow.com/a/9225181/577704
  } else if (e.value() == boost::asio::error::eof) {
      cout << "Connection closed by peer.\n";
  } else {
    std::cerr << "[handle_read] " << e.message() << endl;
  }

}

string Server::do_grep(const Query& query) {
  string command;
  command = "grep " + query.pattern_ + " logs/basic/" + query.filename_;
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe) {
    std::cerr << "Error opening file\n";
    return "Error";
  }

  // read in all the outputs into buffer
  char buffer[128];
  string result = "";
  while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
  }

  return result;
}

/// Handle completion of a write operation.
void Server::handle_write(TcpConnPtr conn, const boost::system::error_code& e)
{
  // Nothing to do. The socket will be closed automatically when the last
  // reference to the connection object goes away.
}


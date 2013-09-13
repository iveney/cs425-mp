#ifndef _TYPES_H_
#define _TYPES_H_

#include <boost/shared_ptr.hpp>
class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnPtr;

class Server;
class Message;

class Client;
typedef boost::shared_ptr<Client> ClientPtr;

#endif /* end of include guard: _TYPES_H_ */



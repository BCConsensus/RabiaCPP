#include <boost/asio.hpp>
#include <stdexcept>
#include <vector>

#define PORT 2000

using boost::asio::awaitable;
using boost::asio::use_awaitable;
using boost::asio::ip::address;
using boost::asio::ip::tcp;

class Connection {
public:
  tcp::socket socket;

  Connection(tcp::socket _socket) : socket(std::move(_socket)) {}

  awaitable<void> write(boost::asio::const_buffer _data) {
    co_await boost::asio::async_write(socket, _data, use_awaitable);
  }

  awaitable<void> read(boost::asio::mutable_buffer _data) {
    co_await boost::asio::async_read(socket, _data, use_awaitable);
  }

  void close() {
    boost::system::error_code ec;
    socket.close(ec);
    if (ec) {
      throw std::runtime_error("Failed to close socket");
    }
  }
};

// TODO: connection to self
class Multicaster {

  std::vector<Connection> connections;

  std::vector<tcp::endpoint> peers;

  // TODO: multicaster should contain a list of inbound and outbound connections
  // need to co_spawn some function that listens for incoming connections and
  // accepts them then stop that coroutine when the connects are made and
  // multicaster is built

  // TODO: make two connections to each peer (one from current node, one from
  // peer) so that one is for sending messages and one is for receiving
  Multicaster(std::string addr, std::vector<std::string> _peers) {
    boost::asio::io_context io_context;
    _peers.push_back(addr);
    for (auto peer : _peers) {
      address address = address::from_string(peer);
      tcp::endpoint endpoint(address, PORT);

      // make a socket for each peer
      tcp::socket socket(io_context);
      boost::asio::connect(socket, endpoint);
      connections.push_back(Connection(std::move(socket)));
    }
  }

  // reads from one connection to the buffer and increments which connection it
  // read from
  void read() {
    // ...
  }

  // writes buffer to all connections
  void write() {
    // ...
  }

  void close() {
    for (auto &connection : connections) {
      connection.close();
    }
  }
};

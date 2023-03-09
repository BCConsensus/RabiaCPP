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

  Multicaster(std::string addr, std::vector<std::string> _peers) {
    boost::asio::io_context io_context;
    for (auto peer : _peers) {
      address address = address::from_string(peer);
      tcp::endpoint endpoint(address, PORT);

      // make a socket for each peer
      tcp::socket socket(io_context);
      connections.push_back(Connection(std::move(socket)));
    }
  }

  void read() {
    // ...
  }

  void write() {
    // ...
  }

  void close() {
    for (auto &connection : connections) {
      connection.close();
    }
  }
};

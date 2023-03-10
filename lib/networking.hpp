#include <boost/asio.hpp>
#include <boost/asio/detached.hpp>
#include <stdexcept>
#include <vector>

#define PORT 2000

using boost::asio::awaitable;
using boost::asio::detached;
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

namespace this_coro = boost::asio::this_coro;

class Multicaster {

  std::vector<Connection> inbounds;
  std::vector<Connection> outbounds;

  int read_index = 0;

  awaitable<void> acceptor() {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), PORT});
    while (true) {
      tcp::socket socket = co_await acceptor.async_accept(use_awaitable);

      if (inbounds.size() == outbounds.size()) {
        // we have enough connections to stop accepting new ones
        break;
      }

      outbounds.push_back(Connection(std::move(socket)));
    }
  }

  // TODO: multicaster should contain a list of inbound and outbound connections
  // need to co_spawn some function that listens for incoming connections and
  // accepts them then stop that coroutine when the connects are made and
  // multicaster is built

  // TODO: need to start the acceptor before trying to connect to other peers
public:
  Multicaster(std::string addr, std::vector<std::string> _peers) {
    boost::asio::io_context io_context;
    _peers.push_back(addr);
    for (auto peer : _peers) {
      address address = address::from_string(peer);
      tcp::endpoint endpoint(address, PORT);

      // make a socket for each peer
      tcp::socket socket(io_context);
      boost::asio::connect(socket, endpoint);
      outbounds.push_back(Connection(std::move(socket)));
    }

    co_spawn(io_context, acceptor(), detached);
  }

  // reads from one connection to the buffer and increments which connection it
  // read from
  awaitable<void> read(boost::asio::mutable_buffer buffer) {
    auto connection = &inbounds.at(read_index);
    read_index++;
    co_await connection->read(buffer);
  }

  // writes buffer to all connections
  awaitable<void> write(boost::asio::const_buffer buffer) {
    for (auto &connection : outbounds) {
      co_await connection.write(buffer);
    }
  }

  void close() {
    for (auto &connection : outbounds) {
      connection.close();
    }
  }
};

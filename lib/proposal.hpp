#include <boost/asio/buffer.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <chrono>
#include <cstdint>
#include <cstring>

class Proposal {
  boost::asio::const_buffer data;
  std::time_t timestamp;
  boost::uuids::uuid id;

  Proposal(boost::asio::const_buffer _data, std::time_t _timestamp) {
    id = boost::uuids::random_generator()();
    data = _data;
    timestamp = _timestamp;
  }

  // override < operator so that std::less in the node's priority queue works
  bool operator<(const Proposal &other) const {
    return timestamp < other.timestamp;
  }

  // "add 1 to confirms"
};

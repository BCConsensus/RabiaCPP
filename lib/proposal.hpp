#include <boost/asio/buffer.hpp>
#include <chrono>
#include <cstdint>
#include <cstring>

class Proposal {
  boost::asio::const_buffer data;
  std::time_t timestamp;

  // TODO: maybe use ulid? then map ID to data
  // also could use bitfields
  // or just use a struct with timestamp and id
  Proposal(boost::asio::const_buffer _data, std::time_t _timestamp) {
    data = _data;
    timestamp = _timestamp;
  }

  // override < operator so that std::less in the node's priority queue works
  bool operator<(const Proposal &other) const {
    return timestamp < other.timestamp;
  }

  // "add 1 to confirms"
};

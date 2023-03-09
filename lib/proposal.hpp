#include <boost/asio/buffer.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <chrono>
#include <cstdint>
#include <cstring>

#include <random>

using namespace std::chrono;

class Proposal {
  uint32_t id;
  uint64_t timestamp;

  static std::random_device rd;

  Proposal() {
    timestamp =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch())
            .count();

    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint32_t> dis;
    id = dis(gen);
  }

  // override < operator so that std::less in the node's priority queue works
  // need to compare timestamps first, then ids
  bool operator<(const Proposal &other) const {
    if (timestamp < other.timestamp) {
      return true;
    } else if (timestamp == other.timestamp) {
      return id < other.id;
    } else {
      return false;
    }
  }
};

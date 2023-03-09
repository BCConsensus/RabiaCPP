#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <queue>
#include <string>

#include "proposal.hpp"

using boost::asio::ip::address;
using boost::asio::ip::tcp;

class Node {
  // virtual uint32_t Size() = 0;
  // virtual int Propose(uint64_t id, const uint8_t* data, uint32_t size) = 0;
  // virtual int Run() = 0;
  // virtual int Repair(uint64_t index, uint64_t& out_id, uint8_t*& out_data,
  // uint32_t& out_size) = 0; virtual int Consume(std::function<int(uint64_t,
  // uint64_t, const uint8_t*, uint32_t)> block) = 0;
  Node(std::string _address, std::vector<std::string> _peers) {
    addr = _address;
    peers = _peers;
    lindex = 0;
  }

  // peers - array of addresses of other nodes
  std::vector<std::string> peers;

  // addr - address of this node
  std::string addr;

  // PQ - instance of priority queue
  // TODO: switch to blocking queue that waits until something is in queue
  std::priority_queue<Proposal, std::vector<Proposal>, std::less<Proposal>> pq;

  // lindex - current slot index in the log
  uint16_t lindex;

  // messages
  // std::map<uint64_t, char[]> messages;

  // TODO: need message IDs to create map of requests to not process

  void run() {
    while (true) {
      auto proposal = pq.top();
      pq.pop();

      // auto output =
    }
  }

  enum Null { null };
  enum Unknown { unknown };

  // weak-mvc function
  // takes a proposal and the node's current slot index in log
  // TODO: std::variant instead because Proposal, null, unknown
  // TODO: co_await write as well
  // reminder: just because co_await read succeeds doesn't mean it got all the
  // data must check how many bytes it got
  std::variant<Proposal, Null, Unknown> SMR(Proposal proposal) {
    // https://www.boost.org/doc/libs/1_81_0/doc/html/boost_asio/example/cpp20/coroutines/echo_server.cpp
  }
};

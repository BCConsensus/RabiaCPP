#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdint>
#include <functional>
#include <map>
#include <queue>
#include <string>

#include "proposal.hpp"

#define PORT 2000

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

    // create endpoints for each peer's address
    for (auto peer : _peers) {
      address address = address::from_string(peer);
      tcp::endpoint endpoint(address, PORT);
      peers.push_back(endpoint);
    }
    lindex = 0;
  }

  // peers - array of addresses of other nodes
  std::vector<boost::asio::ip::tcp::endpoint> peers;

  // addr - address of this node
  std::string addr;

  // PQ - instance of priority queue
  std::priority_queue<Proposal, std::vector<Proposal>, std::less<Proposal>> pq;

  // lindex - current slot index in the log
  uint16_t lindex;

  // messages
  std::map<uint64_t, char[]> messages;

  void run() {
    while (true) {
      auto proposal = pq.top();
      pq.pop();

      // auto output =
    }
  }

  // weak-mvc function
  // takes a proposal and the node's current slot index in log
  std::optional<Proposal> SMR(Proposal proposal, uint16_t lindex) {
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);

    // async connect to each peer, send proposal, wait for responses
  }
};
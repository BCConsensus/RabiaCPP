#include <cstdint>

class Log {
  uint32_t size;
  uint16_t n;
  uint16_t f;
  uint16_t majority;

  uint64_t *logs;

  uint16_t *indices;
  uint64_t *proposals;

  uint8_t *statesZero;
  uint8_t *statesOne;

  uint8_t *votesZero;
  uint8_t *votesOne;
  uint8_t *votesLost;

  Log(uint16_t n, uint32_t size) {
    auto majority = (n / 2) + 1;

    this->size = size;
    this->n = n;
    this->f = n - majority;
    this->majority = majority;

    this->logs = new uint64_t[size];
    this->indices = new uint16_t[size];
    this->proposals = new uint64_t[majority * size];
    this->statesZero = new uint8_t[size * 256];
    this->statesOne = new uint8_t[size * 256];
    this->votesZero = new uint8_t[size * 256];
    this->votesOne = new uint8_t[size * 256];
    this->votesLost = new uint8_t[size * 256];
  }
};

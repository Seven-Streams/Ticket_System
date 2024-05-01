#include "store.hpp"
#ifndef ORDER_HPP
#define ORDER_HPP
class Order {
private:
  unsigned long long id_hash1 = 0;
  unsigned long long id_hash2 = 0;
  int start_station = 0;
  int end_station = 0;
  int number = 0;
  int status = 0;
  // status = 0, pending; status = 1, success; status = 2, refunded.
public:
  Order() = default;
  ~Order() = default;
};
class IndexTime {
private:
  int index = 0;
  int stamp = 0;

public:
  IndexTime() = default;
  ~IndexTime() = default;
  IndexTime(int _index, int _time) {
    index = _index;
    stamp = _time;
  }
  bool operator<(const IndexTime &);
  bool operator>(const IndexTime &);
  bool operator==(const IndexTime &);
};
#endif
#include "store.hpp"
#include "utility.hpp"
#include "account.hpp"
#include "valid.hpp"
#include "exception.hpp"
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
void QueryOrder(std::string &);
#endif
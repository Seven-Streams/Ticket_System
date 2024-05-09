#include "account.hpp"
#include "exception.hpp"
#include "store.hpp"
#include "utility.hpp"
#include "valid.hpp"
#include <string>
#include "train.hpp"
#ifndef ORDER_HPP
#define ORDER_HPP
class OrderByTrain {
private:
  int start_station = 0;
  int end_station = 0;
  int number = 0;
  int start_month = 0;
  int start_day = 0;
  int stamp = 0;
  unsigned long long user_hash1, user_hash2 = 0;
public:
  OrderByTrain() = default;
  ~OrderByTrain() = default;
  bool operator<(const OrderByTrain&) const;
  bool operator>(const OrderByTrain&) const;
  bool operator==(const OrderByTrain&) const;
  friend void Buy(std::string&, int);
  friend void Refund(std::string &);
};
class OrderByUser {
private:
  char Train_ID[21] = "";
  char start_station[41] = "";
  char end_station[41] = "";
  int number = 0;
  Time start_time, end_time;
  int stamp = 0;
  int status = 0;
  int price = 0;
  int out_month = 0;
  int out_day = 0;
  int start_index = 0;
  int end_index = 0;
//status = 1, OK; 2, pending; 3, refunded;
public:
  OrderByUser() = default;
  OrderByUser(const OrderByUser& rhs) = default;
  ~OrderByUser() = default;
  bool operator<(const OrderByUser &) const;
  bool operator>(const OrderByUser &) const;
  bool operator==(const OrderByUser &) const;
  void Print();
  friend void QueryOrder(std::string&);
  friend void Buy(std::string &, int);
  friend void Refund(std::string &);
};
void QueryOrder(std::string &);
void Buy(std::string &, int);
void Refund(std::string &);
#endif
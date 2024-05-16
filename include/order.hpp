#include "account.hpp"
#include "train.hpp"
#include "utility.hpp"
#include <cstring>
#include <string>
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
  bool operator<(const OrderByTrain &) const;
  bool operator>(const OrderByTrain &) const;
  bool operator==(const OrderByTrain &) const;
  friend void Buy(std::string &, const int);
  friend void Refund(std::string &);
};
class OrderByUser {
private:
  char Train_ID[24] = "";
  char start_station[44] = "";
  char end_station[44] = "";
  int number = 0;
  Time start_time, end_time;
  int stamp = 0;
  int status = 0;
  int price = 0;
  int out_month = 0;
  int out_day = 0;
  int start_index = 0;
  int end_index = 0;
  // status = 1, OK; 2, pending; 3, refunded;
public:
  OrderByUser() = default;
  OrderByUser(const OrderByUser &rhs) = default;
  ~OrderByUser() = default;
  bool operator<(const OrderByUser &) const;
  bool operator>(const OrderByUser &) const;
  bool operator==(const OrderByUser &) const;
  void Print() const;
  friend void QueryOrder(std::string &);
  friend void Buy(std::string &, int);
  friend void Refund(std::string &);
};
void QueryOrder(std::string &);
void Buy(std::string &, const int);
void Refund(std::string &);
#endif
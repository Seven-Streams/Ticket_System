#include "account.hpp"
#include "exception.hpp"
#include "store.hpp"
#include "utility.hpp"
#include "valid.hpp"
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
public:
  OrderByTrain(const OrderByTrain&) = default;
  OrderByTrain() = default;
  ~OrderByTrain() = default;
  bool operator<(const OrderByTrain&) const;
  bool operator>(const OrderByTrain&) const;
  bool operator==(const OrderByTrain&) const;
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
//status = 1, OK; 2, pending; 3, refunded;
public:
  OrderByUser() = default;
  OrderByUser(const OrderByUser& rhs) = default;
  ~OrderByUser() = default;
  bool operator<(const OrderByUser &) const;
  bool operator>(const OrderByUser &) const;
  bool operator==(const OrderByUser &) const;
  void Print();
};
void QueryOrder(std::string &);
#endif
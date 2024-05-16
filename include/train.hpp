#include "BPT.hpp"
#include "map.hpp"
#include "memoryriver.hpp"
#include "priority_queue.hpp"
#include "utility.hpp"
#include <cstring>
#include <iostream>
#include <string>
#ifndef TRAIN_HPP
#define TRAIN_HPP
using std::string;
const int smell = 114514191;
class TrainDayIndex {
private:
  int month = 0;
  int day = 0;
  int index = 0;

public:
  TrainDayIndex() = default;
  ~TrainDayIndex() = default;
  bool operator<(const TrainDayIndex &) const;
  bool operator>(const TrainDayIndex &) const;
  bool operator==(const TrainDayIndex &) const;
  friend void ReleaseTrain(string &);
  friend void QueryTrain(string &);
  friend void QueryTicket(string &);
  friend void QueryTransfer(string &);
  friend void Buy(std::string &, const int);
  friend void Refund(std::string &);
};
class AskData {
private:
  int start_index;
  int end_index;
  int price = smell;
  int time = smell;
  Time start_time;
  Time end_time;
  Time out_time;
  string ID = "";
  int seat;

public:
  friend void QueryTicket(string &);
  friend void QueryTransfer(string &);
  friend class SortTrainByTime;
  friend class SortTrainByCost;
  friend class CompareTransferByCost;
  friend class CompareTransferByTime;
};
class TransferData {
private:
  AskData line1, line2;
  int interval;
  string transfer = "";

public:
  friend void QueryTransfer(string &);
  friend class CompareTransferByCost;
  friend class CompareTransferByTime;
};
class TrainInfo {
private:
  char ID[21] = "";
  char stations[100][41];
  int station_number = 0;
  int seat_number = 0;
  int price[100];
  int start_hour = 0;
  int start_minute = 0;
  int travel[100];
  int stop[100];
  // The source is the 0 station.
  int sale_month = 0;
  int sale_day = 0;
  int des_month = 0;
  int des_day = 0;
  char type;
  bool released = false;

public:
  TrainInfo() { memset(stations, '\0', sizeof(stations)); }
  ~TrainInfo() = default;
  bool IsReleased() const;
  int FindIndex(const char *) const;
  int AskPrice(const int &, const int &) const;
  Time AskOutTime(const int &index, const int &month, const int &day) const;
  bool IsSaleTime(const int &, const int &) const;
  Time AskLeaveTime(const int &, const int &, const int &) const;
  Time AskArriveTime(const int &, const int &, const int &) const;
  bool CheckAvailable(const Time &) const;
  int AskTime(const int &, const int &) const;
  friend void AddTrain(string &);
  friend void ReleaseTrain(string &);
  friend void DeleteTrain(string &);
  friend void QueryTrain(string &);
  friend void QueryTicket(string &);
  friend void QueryTransfer(string &);
  friend void Buy(std::string &, int);
};
class TrainDay {
private:
  int month = 0;
  int day = 0;
  int ticket[100];

public:
  TrainDay() = default;
  ~TrainDay() = default;
  TrainDay(const int &_m, const int &_d, const int &_s);
  bool operator>(const TrainDay &rhs) const;
  bool operator<(const TrainDay &rhs) const;
  bool operator==(const TrainDay &rhs) const;
  friend void ReleaseTrain(string &);
  friend void Buy(std::string &, int);
  friend void QueryTrain(string &);
  friend void Refund(string &);
  friend void QueryTicket(string &);
  friend void QueryTransfer(string &);
};
class SortTrainByTime {
public:
  SortTrainByTime() = default;
  ~SortTrainByTime() = default;
  bool operator()(const AskData &, const AskData &) const;
};
class SortTrainByCost {
public:
  SortTrainByCost() = default;
  ~SortTrainByCost() = default;
  bool operator()(const AskData &, const AskData &) const;
};
class CompareTransferByCost {
public:
  CompareTransferByCost() = default;
  ~CompareTransferByCost() = default;
  bool operator()(const TransferData &, const TransferData &) const;
};
class CompareTransferByTime {
public:
  CompareTransferByTime() = default;
  ~CompareTransferByTime() = default;
  bool operator()(const TransferData &, const TransferData &) const;
};
void AddTrain(string &);
void ReleaseTrain(string &);
void DeleteTrain(string &);
void QueryTrain(string &);
void QueryTicket(string &);
void QueryTransfer(string &);
#endif
#include "exception.hpp"
#include "utility.hpp"
#include <cstring>
#include <string>
#ifndef TRAIN_HPP
#define TRAIN_HPP
using std::string;
const int smell = 114514191;
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
  bool IsReleased();
  int FindIndex(const char *);
  int AskPrice(int, int);
  Time AskOutTime(int index, int month, int day);
  bool IsSaleTime(int, int);
  Time AskLeaveTime(int, int, int);
  Time AskArriveTime(int, int, int);
  bool CheckAvailable(const Time &);
  int AskTime(int, int);
  friend void AddTrain(string &);
  friend void ReleaseTrain(string &);
  friend void DeleteTrain(string &);
  friend void QueryTrain(string &);
  friend void QueryTicket(string &);
  friend void QueryTransfer(string &);
};
class TrainDay {
private:
  int month = 0;
  int day = 0;
  int ticket[100];

public:
  TrainDay() = default;
  ~TrainDay() = default;
  TrainDay(int _m, int _d, int _s);
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
  bool operator()(AskData, AskData);
};
class SortTrainByCost {
public:
  bool operator()(AskData, AskData);
};
class CompareTransferByCost {
public:
  bool operator()(TransferData, TransferData);
};
class CompareTransferByTime {
public:
  bool operator()(TransferData, TransferData);
};
void AddTrain(string &);
void ReleaseTrain(string &);
void DeleteTrain(string &);
void QueryTrain(string &);
void QueryTicket(string &);
void QueryTransfer(string &);
#endif
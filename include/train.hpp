#include "utility.hpp"
#include <cstring>
#include <string>
#ifndef TRAIN_HPP
#define TRAIN_HPP
using std::string;
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
  TrainInfo() {
    memset(stations, '\0', sizeof(stations));
  }
  ~TrainInfo() = default;
  friend void AddTrain(string &);
  friend void ReleaseTrain(string &);
  friend void DeleteTrain(string &);
};
class BetterInTime {
public:
  BetterInTime();
  ~BetterInTime();
  bool operator()();
}
void AddTrain(string &);
void ReleaseTrain(string &);
void DeleteTrain(string &);
#endif
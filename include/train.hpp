#include "utility.hpp"
#include <string>
#ifndef TRAIN_HPP
#define TRAIN_HPP
using std::string;
class TrainInfo {
private:
  unsigned long long id_hash1 = 0;
  unsigned long long id_hash2 = 0;
  unsigned long long station_hash1[100];
  unsigned long long station_hash2[100];
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
  TrainInfo() = default;
  ~TrainInfo() = default;
  friend void AddTrain(string &);
  friend void ReleaseTrain(string &);
  friend void DeleteTrain(string &);
};
void AddTrain(string &);
void ReleaseTrain(string &);
void DeleteTrain(string &);
#endif
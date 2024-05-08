#ifndef UTILITY_HPP
#define UTILITY_HPP
#include "exception.hpp"
#include <iostream>
class Time {
private:
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;

public:
  ~Time() = default;
  Time() = default;
  Time(int, int, int, int);
  void Add(int);
  void Minus(int);
  void Print();
  int GetMonth();
  int GetDay();
  bool operator<(const Time&rhs) const;
  bool operator>(const Time&rhs) const;
  bool operator==(const Time &rhs) const;
};
std::string ProcessTxt(std::string &);
std::string ProcessMalValue(std::string &);
#endif
#include "exception.hpp"
#include <iostream>
#ifndef UTILITY_HPP
#define UTILITY_HPP
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
};
std::string ProcessTxt(std::string &);
std::string ProcessMalValue(std::string &);
#endif
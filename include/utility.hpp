#include<iostream>
#ifndef UTILITY_HPP
#define UTILITY_HPP
class time {
private:
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;
public:
  ~time() = default;
  time() = default;
  time(int, int, int, int);
  void Add(int);
};
std::string ProcessTxt(std::string &);
#endif
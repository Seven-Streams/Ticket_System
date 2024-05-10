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
  bool operator<(const Time &rhs) const;
  bool operator>(const Time &rhs) const;
  bool operator==(const Time &rhs) const;
};
namespace sjtu {

template <class T1, class T2> class pair {
public:
  T1 first;
  T2 second;
  constexpr pair() : first(), second() {}
  pair(const pair &other) = default;
  pair(pair &&other) = default;
  pair(const T1 &x, const T2 &y) : first(x), second(y) {}
  template <class U1, class U2> pair(U1 &&x, U2 &&y) : first(x), second(y) {}
  template <class U1, class U2>
  pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
  template <class U1, class U2>
  pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
};

} // namespace sjtu
std::string ProcessTxt(std::string &);
std::string ProcessMalValue(std::string &);
#endif
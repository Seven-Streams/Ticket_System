#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <fstream>
#include <iostream>
const unsigned long long exp1 = 13331, exp2 = 131;
const int minus_max = -2147483648;
const int maxn = 2147483647;
namespace SevenStream {
class exception {
  std::string type;

public:
  exception(const std::string &txt) { type = txt; }
  std::string what() { return type; }
};
} // namespace SevenStream
class Time {
private:
  int month = 0;
  int day = 0;
  int hour = 0;
  int minute = 0;

public:
  ~Time() = default;
  Time() = default;
  Time(const int &, const int &, const int &, const int &);
  void Add(const int &);
  void Minus(const int &);
  void Print() const;
  int GetMonth() const;
  int GetDay() const;
  friend int IntervalMinute(Time, Time);
  bool operator<(const Time &rhs) const;
  bool operator>(const Time &rhs) const;
  bool operator==(const Time &rhs) const;
};
int IntervalMinute(Time, Time);
std::string ProcessTxt(std::string &);
std::string ProcessMalValue(std::string &);
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
template <class T> class Less {
public:
  bool operator()(T a, T b) { return a < b; }
};
inline unsigned long long MyHash(const std::string &txt,
                                 const unsigned long long &exp) {
  unsigned long long ans = 0;
  for (int i = 0; i < txt.size(); i++) {
    ans *= exp;
    ans += txt[i];
  }
  return ans;
}
} // namespace sjtu
#endif
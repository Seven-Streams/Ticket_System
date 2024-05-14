#include "../include/utility.hpp"
#include <fstream>
using std::cout;
using std::string;
string ProcessTxt(string &txt) {
  while (txt[0] == ' ') {
    txt.erase(0, 1);
  }
  while (txt[txt.size() - 1] == ' ') {
    txt.erase(txt.size() - 1, 1);
  }
  string tmp = "";
  while ((!txt.empty()) && txt[0] != ' ') {
    tmp += txt[0];
    txt.erase(0, 1);
  }
  while ((!txt.empty()) && txt[0] == ' ') {
    txt.erase(0, 1);
  }
  return tmp;
}
Time::Time(int _mon, int _d, int _h, int _min) {
  month = _mon;
  day = _d;
  hour = _h;
  minute = _min;
}
void Time::Add(int duration_minutes) {
  minute += duration_minutes;
  int add_hour = minute / 60;
  minute %= 60;
  hour += add_hour;
  int add_day = hour / 24;
  hour %= 24;
  day += add_day;
  if ((month == 6) && (day > 30)) {
    month++;
    day -= 30;
    return;
  }
  if (day > 31) {
    month++;
    day -= 31;
    return;
  }
}
string ProcessMalValue(string &txt) {
  string tmp;
  if (txt[0] == '|') {
    txt.erase(0, 1);
  }
  while ((!txt.empty()) && txt[0] != '|') {
    tmp += txt[0];
    txt.erase(0, 1);
  }
  if (tmp.empty()) {
    throw(SevenStream::exception("Empty value."));
  }
  return tmp;
}

void Time::Minus(int duration_minutes) {
  int to_minus_hour = duration_minutes / 60;
  int to_minus_min = duration_minutes % 60;
  if (minute < to_minus_min) {
    to_minus_hour++;
    minute += 60;
  } // to ensure it's positive.
  minute -= to_minus_min;
  int to_minus_days = to_minus_hour / 24;
  to_minus_hour %= 24;
  if (hour < to_minus_hour) {
    hour += 24;
    to_minus_days++;
  }
  hour -= to_minus_hour;
  if (to_minus_days >= day) {
    if (month == 7) {
      day += 30;
      month--;
    } else {
      day += 31;
      month--;
    }
  }
  day -= to_minus_days;
  return;
}

void Time::Print() {
  if (month < 10) {
    cout << 0;
  }
  cout << month << '-';
  if (day < 10) {
    cout << 0;
  }
  cout << day << ' ';
  if (hour < 10) {
    cout << 0;
  }
  cout << hour << ':';
  if (minute < 10) {
    cout << 0;
  }
  cout << minute << ' ';
  return;
}
int Time::GetMonth() const { return month; }
int Time::GetDay()const  { return day; }
bool Time::operator<(const Time &rhs) const {
  if (month != rhs.month) {
    return (month < rhs.month);
  }
  if (day != rhs.day) {
    return (day < rhs.day);
  }
  if (hour != rhs.hour) {
    return (hour < rhs.hour);
  }
  return (minute < rhs.minute);
}
bool Time::operator>(const Time &rhs) const { return (rhs < (*this)); }
bool Time::operator==(const Time &rhs) const {
  return ((!((*this) < rhs)) && (!(rhs < (*this))));
}
int IntervalMinute(Time time1, Time time2) {
  int ans = 0;
  if (time1 == time2) {
    return 0;
  }
  if (time1 > time2) {
    std::swap(time1, time2);
  }
  while (time1 < time2) {
    time2.Minus(24 * 60);
    ans += 24 * 60;
  }
  if(time1 == time2) {
    return ans;
  }
  time2.Add(24 * 60);
  ans -= 24 * 60;
  while(time1 < time2) {
    time2.Minus(60);
    ans += 60;
  }
  if(time1 == time2) {
    return ans;
  }
  ans -= 60;
  time2.Add(60);
  while(time1 < time2) {
    time2.Minus(1);
    ans++;
  }
  return ans;
}
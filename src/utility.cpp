#include "../include/utility.hpp"
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
time::time(int a, int b, int c, int d) {
  month = a;
  day = b;
  hour = c;
  minute = d;
}
void time::Add(int duration_minutes) {
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
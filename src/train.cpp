#include "../include/train.hpp"
#include "../include/exception.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
#include <cstring>
sjtu::BPT<int> train_index("train_index");
sjtu::MemoryRiver<TrainInfo, 1> train_info;
sjtu::BPT<int> station_database("station");
sjtu::BPT<TrainDay> trains_day("train_day");
void AddTrain(std::string &command) {
  string ID, num_raw, seat_raw, stations, prices, start_time, travel_time,
      stop_time, sale_date, type;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-i") {
      if (ID != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      ID = ProcessTxt(command);
      CheckTrainID(ID.c_str());
    }
    if (op == "-n") {
      if (num_raw != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      num_raw = ProcessTxt(command);
      CheckStatNum(num_raw.c_str());
    }
    if (op == "-m") {
      if (seat_raw != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      seat_raw = ProcessTxt(command);
      CheckSeatNum(seat_raw.c_str());
    }
    if (op == "-s") {
      if (stations != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      stations = ProcessTxt(command);
    }
    if (op == "-p") {
      if (prices != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      prices = ProcessTxt(command);
    }
    if (op == "-x") {
      if (start_time != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      start_time = ProcessTxt(command);
      CheckStartTime(start_time.c_str());
    }
    if (op == "-t") {
      if (travel_time != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      travel_time = ProcessTxt(command);
    }
    if (op == "-o") {
      if (stop_time != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      stop_time = ProcessTxt(command);
    }
    if (op == "-d") {
      if (sale_date != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      sale_date = ProcessTxt(command);
    }
    if (op == "-y") {
      if (type != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      type = ProcessTxt(command);
      CheckType(type.c_str());
    }
  }
  if ((ID == "") || (num_raw == "") || (seat_raw == "") || (stations == "") ||
      (prices == "") || (start_time == "") || (travel_time == "") ||
      (stop_time == "") || (sale_date == "") || (type == "")) {
    throw(SevenStream::exception("Invalid input."));
  }
  TrainInfo res;
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(ID, exp1);
  hash2 = sjtu::MyHash(ID, exp2);
  auto exist_check = train_index.find(hash1, hash2, minus_max);
  if (!exist_check.empty()) {
    throw(SevenStream::exception("The train has been added."));
  }
  strcpy(res.ID, ID.c_str());
  int num = std::stoi(num_raw);
  res.station_number = num;
  int seat_num = std::stoi(seat_raw);
  res.seat_number = seat_num;
  for (int i = 1; i < num; i++) {
    string price_raw = ProcessMalValue(prices);
    CheckPrice(price_raw.c_str());
    int price = std::stoi(price_raw);
    res.price[i] = price;
  }
  for (int i = 0; i < num; i++) {
    string station = ProcessMalValue(stations);
    CheckStation(station.c_str());
    strcpy(res.stations[i], station.c_str());
  }
  CheckStartTime(start_time.c_str());
  int hour = (start_time[0] - '0') * 10 + (start_time[1] - '0');
  int minute = (start_time[3] - '0') * 10 + (start_time[4] - '0');
  res.start_hour = hour;
  res.start_minute = minute;
  for (int i = 1; i < num; i++) {
    string travel_time_raw = ProcessMalValue(travel_time);
    CheckInterTime(travel_time_raw.c_str());
    int time = std::stoi(travel_time_raw);
    res.travel[i] = time;
  }
  for (int i = 1; i < (num - 1); i++) {
    string stop_time_raw = ProcessMalValue(stop_time);
    CheckInterTime(stop_time_raw.c_str());
    int time = std::stoi(stop_time_raw);
    res.stop[i] = time;
  }
  string date = ProcessMalValue(sale_date);
  CheckDate(date.c_str());
  int month = (date[0] - '0') * 10 + (date[1] - '0');
  int day = (date[3] - '0') * 10 + (date[4] - '0');
  res.sale_month = month;
  res.sale_day = day;
  date = ProcessMalValue(sale_date);
  CheckDate(date.c_str());
  month = (date[0] - '0') * 10 + (date[1] - '0');
  day = (date[3] - '0') * 10 + (date[4] - '0');
  res.des_month = month;
  res.des_day = day;

  res.type = type[0];
  int current;
  train_info.get_info(current, 1);
  train_info.write(res, ++current);
  train_info.write_info(current, 1);
  train_index.Insert(hash1, hash2, current);
  for (int i = 0; i < num; i++) {
    unsigned long long station_hash1, station_hash2;
    string station(res.stations[i]);
    station_hash1 = sjtu::MyHash(station, exp1);
    station_hash2 = sjtu::MyHash(station, exp2);
    station_database.Insert(station_hash1, station_hash2, current);
  }
  return;
}
void ReleaseTrain(string &command) {
  string op;
  op = ProcessTxt(command);
  if (op != "-i") {
    throw(SevenStream::exception("Invalid input."));
  }
  string id = ProcessTxt(command);
  CheckTrainID(id.c_str());
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(hash1, hash2, minus_max);
  if (index_raw.empty()) {
    throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo to_release;
  train_info.read(to_release, index);
  to_release.released = true;
  for (int i = 0; i < to_release.station_number; i++) {
    string station(to_release.stations[i]);
    unsigned long long station_hash1, station_hash2;
    station_hash1 = sjtu::MyHash(station, exp1);
    station_hash2 = sjtu::MyHash(station, exp2);
    station_database.Insert(station_hash1, station_hash2, index);
  }
  
  train_info.write(to_release, index);
  return;
}
void DeleteTrain(string &command) {
  string op = ProcessTxt(command);
  if (op != "-i") {
    throw(SevenStream::exception("Invalid input."));
  }
  string id = ProcessTxt(command);
  CheckTrainID(id.c_str());
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(hash1, hash2, minus_max);
  if (index_raw.empty()) {
    throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo to_delete;
  train_info.read(to_delete, index);
  if (to_delete.released) {
    throw(SevenStream::exception("The train has been released."));
  }
  train_index.Erase(hash1, hash2, index);
  return;
}

void QueryTrain(string &command) {
  string op = ProcessTxt(command);
  string id, date;
  if (op == "-i") {
    id = ProcessTxt(command);
  } else {
    if (op == "-d") {
      date = ProcessTxt(command);
    } else {
      throw(SevenStream::exception("Invalid input."));
    }
  }
  op = ProcessTxt(command);
  if (op == "-i") {
    if (id != "") {
      throw(SevenStream::exception("Invalid input."));
    }
    id = ProcessTxt(command);
  } else {
    if (op == "-d") {
      if (date != "") {
        throw(SevenStream::exception("Invalid input."));
      }
      date = ProcessTxt(command);
    } else {
      throw(SevenStream::exception("Invalid input."));
    }
  }
  CheckDate(date.c_str());
  CheckTrainID(id.c_str());
  int month, day;
  month = 0;
  day = 0;
  month += date[0] - '0';
  month *= 10;
  month += date[1] - '0';
  day += date[3] - '0';
  day *= 10;
  day += date[4];
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(hash1, hash2, minus_max);
  if (index_raw.empty()) {
    throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo to_query;
  train_info.read(to_query, index);
  if (month < to_query.sale_month) {
    throw(SevenStream::exception("Invalid date."));
  }
  if (month > to_query.des_month) {
    throw(SevenStream::exception("Invalid date."));
  }
  if ((month == to_query.sale_month) && (day < to_query.sale_month)) {
    throw(SevenStream::exception("Invalid date."));
  }
  if ((month == to_query.des_month) && (day > to_query.sale_month)) {
    throw(SevenStream::exception("Invalid date."));
  }
  if (!to_query.released) {
    std::cout << to_query.ID << ' ' << to_query.type << '\n';
    std::cout << to_query.stations[0] << ' ' << "xx-xx xx:xx ->";
    Time time(month, day, to_query.start_hour, to_query.start_minute);
    time.Print();
    std::cout << 0 << ' ' << to_query.price << '\n';
    for (int i = 1; i < (to_query.station_number - 1); i++) {
      std::cout << to_query.stations[i] << ' ';
      time.Add(to_query.travel[i]);
      time.Print();
      std::cout << "-> ";
      time.Add(to_query.stop[i]);
      time.Print();
      std::cout << to_query.price[i] << ' ' << to_query.seat_number << '\n';
    }
    std::cout << to_query.stations[to_query.station_number - 1] << ' ';
    time.Add(to_query.travel[to_query.station_number - 1]);
    time.Print();
    std::cout << "-> xx-xx xx:xx ";
    std::cout << to_query.price[to_query.station_number - 1] << " x\n";
  } else {
    // Need to be finished here.
  }
  return;
}
TrainDay::TrainDay(int _m, int _d, int _s) {
  month = _m;
  day = _d;
  for (int &element : ticket) {
    element = _s;
  }
}
bool TrainDay::operator<(const TrainDay &rhs) const {
  if (month != rhs.month) {
    return month < rhs.month;
  }
  return (day < rhs.month);
}
bool TrainDay::operator>(const TrainDay &rhs) const {
  return rhs < (*this);
}
bool TrainDay::operator==(const TrainDay &rhs) const {
  return ((!(rhs < (*this))) && (!((*this) < rhs)));
}
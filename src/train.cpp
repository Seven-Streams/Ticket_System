#include "../include/train.hpp"
#include "../include/exception.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
#include <cstring>
#include <string>
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
  Time time(to_release.sale_month, to_release.sale_day, 1, 0);
  Time end_time(to_release.des_month, to_release.des_day, 1, 0);
  TrainDay to_release_day(to_release.sale_month, to_release.sale_day,
                          to_release.seat_number);
  for (auto i = time; i < end_time; i.Add(60 * 24)) {
    to_release_day.month = i.GetMonth();
    to_release_day.day = i.GetDay();
    trains_day.Insert(hash1, hash2, to_release_day);
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
    std::cout << 0 << ' ' << to_query.seat_number << '\n';
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
    std::cout << to_query.ID << ' ' << to_query.type << '\n';
    std::cout << to_query.stations[0] << ' ' << "xx-xx xx:xx ->";
    Time time(month, day, to_query.start_hour, to_query.start_minute);
    time.Print();
    TrainDay actual_train(month, day, 0);
    unsigned long long id_hash1, id_hash2;
    id_hash1 = sjtu::MyHash(id, exp1);
    id_hash2 = sjtu::MyHash(id, exp2);
    auto raw_actual_train = trains_day.find(id_hash1, id_hash2, actual_train);
    actual_train = raw_actual_train.front();
    std::cout << 0 << ' ' << actual_train.ticket[0] << '\n';
    for (int i = 1; i < (to_query.station_number - 1); i++) {
      std::cout << to_query.stations[i] << ' ';
      time.Add(to_query.travel[i]);
      time.Print();
      std::cout << "-> ";
      time.Add(to_query.stop[i]);
      time.Print();
      std::cout << to_query.price[i] << ' ' << actual_train.ticket[i] << '\n';
    }
    std::cout << to_query.stations[to_query.station_number - 1] << ' ';
    time.Add(to_query.travel[to_query.station_number - 1]);
    time.Print();
    std::cout << "-> xx-xx xx:xx ";
    std::cout << to_query.price[to_query.station_number - 1] << " x\n";
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
bool TrainDay::operator>(const TrainDay &rhs) const { return rhs < (*this); }
bool TrainDay::operator==(const TrainDay &rhs) const {
  return ((!(rhs < (*this))) && (!((*this) < rhs)));
}
bool TrainInfo::IsReleased() { return released; }
int TrainInfo::FindIndex(const char *str) {
  for (int i = 0; i < station_number; i++) {
    if (str == stations[i]) {
      return i;
    }
  }
  throw(SevenStream::exception("No such station."));
}
int TrainInfo::AskPrice(int start, int end) {
  int ans = 0;
  for (int i = (start + 1); i <= end; i++) {
    ans += price[i];
  }
  return ans;
}
Time TrainInfo::AskOutTime(int index, int month, int day) {
  Time start_time(month, day, start_hour, start_minute);
  Time time = start_time;
  for (int i = 1; i < index; i++) {
    time.Add(travel[i]);
    time.Add(stop[i]);
  }
  int final_month, final_day;
  final_month = time.GetMonth();
  final_day = time.GetDay();
  while ((final_month != month) || (final_day != day)) {
    time.Minus(24 * 60);
    final_month = time.GetMonth();
    final_day = time.GetDay();
    start_time.Minus(24 * 60);
  }
  return start_time;
}
bool TrainInfo::IsSaleTime(int _m, int _d) {
  if (_m < sale_month) {
    return false;
  }
  if ((_m == sale_month) && (_d < sale_day)) {
    return false;
  }
  if (_m > des_month) {
    return false;
  }
  if ((_m == des_month) && (_d > des_day)) {
    return false;
  }
  return true;
}
Time TrainInfo::AskLeaveTime(int index, int _m, int _d) {
  Time ans(_m, _d, start_hour, start_minute);
  for (int i = 1; i <= index; i++) {
    ans.Add(travel[i]);
    ans.Add(stop[i]);
  }
  return ans;
}
Time TrainInfo::AskArriveTime(int index, int _m, int _d) {
  Time ans(_m, _d, start_hour, start_minute);
  for (int i = 1; i < index; i++) {
    ans.Add(travel[i]);
    ans.Add(stop[i]);
  }
  ans.Add(travel[index]);
  return ans;
}
void QueryTicket(string &command) {
  string start, end, date;
  bool by_time = true;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-s") {
      start = ProcessTxt(command);
      CheckStation(start.c_str());
      continue;
    }
    if (op == "-t") {
      end = ProcessTxt(command);
      CheckStation(start.c_str());
      continue;
    }
    if (op == "-d") {
      date = ProcessTxt(command);
      CheckDate(date.c_str());
      continue;
    }
    if (op == "-p") {
      string res = ProcessTxt(command);
      if (command == "time") {
        continue;
      }
      if (command == "cost") {
        by_time = false;
      }
      throw(SevenStream::exception("Invalid priority type."));
    }
  }
  int month = date[0] - '0';
  month *= 10;
  month += date[1] - '0';
  int day = date[3] - '0';
  day *= 10;
  day += date[4] - '0';
  unsigned long long start_hash1, start_hash2;
  start_hash1 = sjtu::MyHash(start, exp1);
  start_hash2 = sjtu::MyHash(start, exp2);
  auto start_index_raw = station_database.find(start_hash1, start_hash2, -1);
  unsigned long long end_hash1, end_hash2;
  end_hash1 = sjtu::MyHash(end, exp1);
  end_hash2 = sjtu::MyHash(end, exp2);
  auto end_index_raw = station_database.find(end_hash1, end_hash2, -1);
  sjtu::priority_queue<int> start_indexs, end_indexs;
  for (auto it = start_index_raw.begin(); it != start_index_raw.end(); it++) {
    start_indexs.push(*it);
  }
  for (auto it = end_index_raw.begin(); it != end_index_raw.end(); it++) {
    end_indexs.push(*it);
  }
  sjtu::list<int> same_index;
  int start_index, end_index;
  while ((!start_indexs.empty()) && (!end_indexs.empty())) {
    start_index = start_indexs.top();
    end_index = end_indexs.top();
    if (start_index == end_index) {
      same_index.push_back(start_index);
      start_indexs.pop();
      end_indexs.pop();
    } else {
      if (start_index < end_index) {
        start_indexs.pop();
      } else {
        end_indexs.pop();
      }
    }
  } // to get the train_ID.
  sjtu::list<AskData> condidate_trains;
  for (auto it = same_index.begin(); it != same_index.end(); it++) {
    TrainInfo res;
    train_info.read(res, *it);
    int start_index, end_index;
    start_index = res.FindIndex(start.c_str());
    end_index = res.FindIndex(end.c_str());
    if (end_index > start_index) {
      AskData to_push;
      to_push.ID = res.ID;
      to_push.end_index = end_index;
      to_push.start_index = start_index;
      to_push.price = res.AskPrice(start_index, end_index);
      to_push.out_time = res.AskOutTime(start_index, month, day);
      to_push.start_time = res.AskLeaveTime(start_index, to_push.out_time.GetMonth(), to_push.out_time.GetDay());
      to_push.end_time = res.AskArriveTime(end_index, to_push.out_time.GetMonth(), to_push.out_time.GetDay());
      to_push.time = res.AskTime(start_index, end_index);
      condidate_trains.push_back(to_push);
    }
  }
  sjtu::list<AskData> confirmed_data;
  for (auto it = condidate_trains.begin(); it != condidate_trains.end(); it++) {
    unsigned long long hash1, hash2;
    hash1 = sjtu::MyHash(it->ID, exp1);
    hash2 = sjtu::MyHash(it->ID, exp2);
    TrainDay to_find((it->out_time).GetMonth(), (it->out_time).GetDay(), 0);
    auto trains = trains_day.find(hash1, hash2, to_find);
    if (trains.size()) {
      auto to_check = trains.front();
      if ((to_check.month == it->out_time.GetMonth()) &&
          (to_check.day == it->out_time.GetDay())) {
        int available = maxn;
        for (int i = it->start_index; i < it->end_index; i++) {
          available = std::min(available, to_check.ticket[i]);
        }
        it->seat = available;
        confirmed_data.push_back(*it);
      }
    }
  }
  sjtu::list<AskData> output_list;
  if(by_time) {
    sjtu::priority_queue<AskData, SortTrainByTime> output;
    for(auto it = confirmed_data.begin(); it != confirmed_data.end(); it++) {
      output.push(*it);
    }
    while(!output.empty()) {
      output_list.push_back(output.top());
      output.pop();
    }
  } else {
      sjtu::priority_queue<AskData, SortTrainByCost> output;
    for(auto it = confirmed_data.begin(); it != confirmed_data.end(); it++) {
      output.push(*it);
    }
    while(!output.empty()) {
      output_list.push_back(output.top());
      output.pop();
    } 
  }
  std::cout << output_list.size() << '\n';
  for(auto it = output_list.begin(); it != output_list.end(); it++) {
    std::cout << it->ID << ' ' << start << ' ';
    it->start_time.Print();
    std::cout << "-> " << end << ' ';
    it->end_time.Print();
    std::cout << it->price << ' ' << it->seat << '\n';
  }
  return;
}

int TrainInfo::AskTime(int start_index, int end_index) {
  int ans = 0;
  for (int i = start_index + 1; i < end_index; i++) {
    ans += travel[i];
    ans += stop[i];
  }
  ans += travel[end_index];
  return ans;
}
bool SortTrainByTime::operator()(AskData lhs, AskData rhs) {
  if (lhs.time != rhs.time) {
    return (lhs.time < rhs.time);
  }
  return (lhs.ID < rhs.ID);
}
bool SortTrainByCost::operator()(AskData lhs, AskData rhs) {
  if (lhs.price != rhs.price) {
    return (lhs.price < rhs.price);
  }
  return (lhs.ID < rhs.ID);
}
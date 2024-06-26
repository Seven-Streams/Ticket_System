#include "../include/train.hpp"
sjtu::BPT<int> train_index("train_index");
sjtu::MemoryRiver<TrainInfo, 1> train_info("train_database");
sjtu::BPT<int, 168, 6, 300> station_database("station");
sjtu::BPT<TrainDayIndex, 126, 4, 1700> trains_day_index("train_day_index");
sjtu::MemoryRiver<TrainDay, 1> train_day_info("train_day_info");
void AddTrain(std::string &command) {
  string ID, num_raw, seat_raw, stations, prices, start_time, travel_time,
      stop_time, sale_date, type;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-i") {
      if (ID != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      ID = ProcessTxt(command);
    }
    if (op == "-n") {
      if (num_raw != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      num_raw = ProcessTxt(command);
    }
    if (op == "-m") {
      if (seat_raw != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      seat_raw = ProcessTxt(command);
    }
    if (op == "-s") {
      if (stations != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      stations = ProcessTxt(command);
    }
    if (op == "-p") {
      if (prices != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      prices = ProcessTxt(command);
    }
    if (op == "-x") {
      if (start_time != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      start_time = ProcessTxt(command);
    }
    if (op == "-t") {
      if (travel_time != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      travel_time = ProcessTxt(command);
    }
    if (op == "-o") {
      if (stop_time != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      stop_time = ProcessTxt(command);
    }
    if (op == "-d") {
      if (sale_date != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      sale_date = ProcessTxt(command);
    }
    if (op == "-y") {
      if (type != "") {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid input."));
      }
      type = ProcessTxt(command);
    }
  }
  if ((ID == "") || (num_raw == "") || (seat_raw == "") || (stations == "") ||
      (prices == "") || (start_time == "") || (travel_time == "") ||
      (stop_time == "") || (sale_date == "") || (type == "")) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Invalid input."));
  }
  TrainInfo res;
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(ID, exp1);
  hash2 = sjtu::MyHash(ID, exp2);
  auto exist_check = train_index.find(hash1, hash2, minus_max);
  if (!exist_check.empty()) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The train has been added."));
  }
  strcpy(res.ID, ID.c_str());
  int num = std::stoi(num_raw);
  res.station_number = num;
  int seat_num = std::stoi(seat_raw);
  res.seat_number = seat_num;
  for (int i = 1; i < num; i++) {
    string price_raw = ProcessMalValue(prices);
    int price = std::stoi(price_raw);
    res.price[i] = price;
  }
  for (int i = 0; i < num; i++) {
    string station = ProcessMalValue(stations);
    strcpy(res.stations[i], station.c_str());
  }
  int hour = (start_time[0] - '0') * 10 + (start_time[1] - '0');
  int minute = (start_time[3] - '0') * 10 + (start_time[4] - '0');
  res.start_hour = hour;
  res.start_minute = minute;
  for (int i = 1; i < num; i++) {
    string travel_time_raw = ProcessMalValue(travel_time);
    int time = std::stoi(travel_time_raw);
    res.travel[i] = time;
  }
  for (int i = 1; i < (num - 1); i++) {
    string stop_time_raw = ProcessMalValue(stop_time);
    int time = std::stoi(stop_time_raw);
    res.stop[i] = time;
  }
  string date = ProcessMalValue(sale_date);
  int month = (date[0] - '0') * 10 + (date[1] - '0');
  int day = (date[3] - '0') * 10 + (date[4] - '0');
  res.sale_month = month;
  res.sale_day = day;
  date = ProcessMalValue(sale_date);
  month = (date[0] - '0') * 10 + (date[1] - '0');
  day = (date[3] - '0') * 10 + (date[4] - '0');
  res.des_month = month;
  res.des_day = day;

  res.type = type[0];
  int current;
  train_info.get_info(current, 1);
  current++;
  train_info.write_info(current, 1);
  train_info.write(res, current);
  train_index.Insert(hash1, hash2, current);
  std::cout << "0\n";
  return;
  ;
}
void ReleaseTrain(string &command) {
  string op;
  op = ProcessTxt(command);
  if (op != "-i") {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Invalid input."));
  }
  string id = ProcessTxt(command);
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(hash1, hash2, minus_max);
  if (index_raw.empty()) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo to_release;
  train_info.read(to_release, index);
  if (to_release.released) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The train has been released."));
  }
  to_release.released = true;
  for (int i = 0; i < to_release.station_number; i++) {
    string station(to_release.stations[i]);
    unsigned long long station_hash1, station_hash2;
    station_hash1 = sjtu::MyHash(station, exp1);
    station_hash2 = sjtu::MyHash(station, exp2);
    station_database.Insert(station_hash1, station_hash2, index);
  }
  Time time(to_release.sale_month, to_release.sale_day, 1, 0);
  Time end_time(to_release.des_month, to_release.des_day, 23, 59);
  TrainDay to_release_day(to_release.sale_month, to_release.sale_day,
                          to_release.seat_number);
  int current;
  train_day_info.get_info(current, 1);
  for (auto i = time; (i < end_time); i.Add(60 * 24)) {
    to_release_day.month = i.GetMonth();
    to_release_day.day = i.GetDay();
    current++;
    train_day_info.write(to_release_day, current);
    TrainDayIndex res;
    res.month = to_release_day.month;
    res.day = to_release_day.day;
    res.index = current;
    trains_day_index.Insert(hash1, hash2, res);
  }
  train_day_info.write_info(current, 1);
  train_info.write(to_release, index);
  std::cout << "0\n";
  return;
  ;
}
void DeleteTrain(string &command) {
  string op = ProcessTxt(command);
  if (op != "-i") {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Invalid input."));
  }
  string id = ProcessTxt(command);
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(hash1, hash2, minus_max);
  if (index_raw.empty()) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo to_delete;
  train_info.read(to_delete, index);
  if (to_delete.released) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The train has been released."));
  }
  train_index.Erase(hash1, hash2, index);
  std::cout << "0\n";
  return;
  ;
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
      std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid input."));
    }
  }
  op = ProcessTxt(command);
  if (op == "-i") {
    if (id != "") {
      std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid input."));
    }
    id = ProcessTxt(command);
  } else {
    if (op == "-d") {
      if (date != "") {
        std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid input."));
      }
      date = ProcessTxt(command);
    } else {
      std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid input."));
    }
  }
  int month, day;
  month = 0;
  day = 0;
  month += date[0] - '0';
  month *= 10;
  month += date[1] - '0';
  day += date[3] - '0';
  day *= 10;
  day += date[4] - '0';
  unsigned long long hash1 = sjtu::MyHash(id, exp1);
  unsigned long long hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(hash1, hash2, minus_max);
  if (index_raw.empty()) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("The train doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo to_query;
  train_info.read(to_query, index);
  if (month < to_query.sale_month) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid date1."));
  }
  if (month > to_query.des_month) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid date2."));
  }
  if ((month == to_query.sale_month) && (day < to_query.sale_day)) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid date3."));
  }
  if ((month == to_query.des_month) && (day > to_query.des_day)) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("Invalid date4."));
  }
  if (!to_query.released) {
    std::cout << to_query.ID << ' ' << to_query.type << '\n';
    std::cout << to_query.stations[0] << ' ' << "xx-xx xx:xx -> ";
    Time time(month, day, to_query.start_hour, to_query.start_minute);
    time.Print();
    std::cout << 0 << ' ' << to_query.seat_number << '\n';
    int price = 0;
    for (int i = 1; i < (to_query.station_number - 1); i++) {
      std::cout << to_query.stations[i] << ' ';
      time.Add(to_query.travel[i]);
      time.Print();
      std::cout << "-> ";
      time.Add(to_query.stop[i]);
      time.Print();
      price += to_query.price[i];
      std::cout << price << ' ' << to_query.seat_number << '\n';
    }
    std::cout << to_query.stations[to_query.station_number - 1] << ' ';
    time.Add(to_query.travel[to_query.station_number - 1]);
    time.Print();
    std::cout << "-> xx-xx xx:xx ";
    std::cout << price + to_query.price[to_query.station_number - 1] << " x\n";
  } else {
    std::cout << to_query.ID << ' ' << to_query.type << '\n';
    std::cout << to_query.stations[0] << ' ' << "xx-xx xx:xx -> ";
    Time time(month, day, to_query.start_hour, to_query.start_minute);
    time.Print();
    TrainDay actual_train(month, day, 0);
    TrainDayIndex to_find;
    to_find.month = month;
    to_find.day = day;
    unsigned long long id_hash1, id_hash2;
    id_hash1 = sjtu::MyHash(id, exp1);
    id_hash2 = sjtu::MyHash(id, exp2);
    auto bigger_train = to_find;
    bigger_train.day++;
    auto raw_actual_train =
        trains_day_index.find2(id_hash1, id_hash2, to_find, bigger_train);
    to_find = raw_actual_train.front();
    train_day_info.read(actual_train, to_find.index);
    std::cout << 0 << ' ' << actual_train.ticket[0] << '\n';
    int price = 0;
    for (int i = 1; i < (to_query.station_number - 1); i++) {
      std::cout << to_query.stations[i] << ' ';
      time.Add(to_query.travel[i]);
      time.Print();
      std::cout << "-> ";
      time.Add(to_query.stop[i]);
      time.Print();
      price += to_query.price[i];
      std::cout << price << ' ' << actual_train.ticket[i] << '\n';
    }
    std::cout << to_query.stations[to_query.station_number - 1] << ' ';
    time.Add(to_query.travel[to_query.station_number - 1]);
    time.Print();
    std::cout << "-> xx-xx xx:xx ";
    std::cout << price + to_query.price[to_query.station_number - 1] << " x\n";
  }
  return;
}
TrainDay::TrainDay(const int &_m, const int &_d, const int &_s) {
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
  return (day < rhs.day);
}
bool TrainDay::operator>(const TrainDay &rhs) const { return rhs < (*this); }
bool TrainDay::operator==(const TrainDay &rhs) const {
  return ((!(rhs < (*this))) && (!((*this) < rhs)));
}
bool TrainInfo::IsReleased() const { return released; }
int TrainInfo::FindIndex(const char *str) const {
  for (int i = 0; i < station_number; i++) {
    if (strcmp(stations[i], str) == 0) {
      return i;
    }
  }
  throw(SevenStream::exception("No such station!"));
}
int TrainInfo::FindIndex2(const char *str) const {
  for (int i = 0; i < station_number; i++) {
    if (strcmp(stations[i], str) == 0) {
      return i;
    }
  }
  return -1;
}
int TrainInfo::AskPrice(const int &start, const int &end) const {
  int ans = 0;
  for (int i = (start + 1); i <= end; i++) {
    ans += price[i];
  }
  return ans;
}
Time TrainInfo::AskOutTime(const int &index, const int &month,
                           const int &day) const {
  Time start_time(month, day, start_hour, start_minute);
  Time time = start_time;
  for (int i = 1; i <= index; i++) {
    time.Add(travel[i]);
    time.Add(stop[i]);
  }
  int final_month, final_day;
  final_month = time.GetMonth();
  final_day = time.GetDay();
  while ((final_month != month) || (final_day > day)) {
    time.Minus(24 * 60);
    final_month = time.GetMonth();
    final_day = time.GetDay();
    start_time.Minus(24 * 60);
  }
  return start_time;
}
bool TrainInfo::IsSaleTime(const int &_m, const int &_d) const {
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
Time TrainInfo::AskLeaveTime(const int &index, const int &_m,
                             const int &_d) const {
  Time ans(_m, _d, start_hour, start_minute);
  for (int i = 1; i <= index; i++) {
    ans.Add(travel[i]);
    ans.Add(stop[i]);
  }
  return ans;
}
Time TrainInfo::AskArriveTime(const int &index, const int &_m,
                              const int &_d) const {
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
      continue;
    }
    if (op == "-t") {
      end = ProcessTxt(command);
      continue;
    }
    if (op == "-d") {
      date = ProcessTxt(command);
      continue;
    }
    if (op == "-p") {
      string res = ProcessTxt(command);
      if (res == "time") {
        continue;
      }
      if (res == "cost") {
        by_time = false;
        continue;
      }
      std::cout
          << "-1"
          << '\n'; // throw(SevenStream::exception("Invalid priority type."));
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
  sjtu::map<int, bool> start_indexs;
  for (auto it = start_index_raw.begin(); it != start_index_raw.end(); it++) {
    start_indexs[*it] = true;
  }
  sjtu::list<int> same_index;
  for (auto it = end_index_raw.begin(); it != end_index_raw.end(); it++) {
    if (start_indexs.count(*it)) {
      same_index.push_back(*it);
    }
  }
  sjtu::list<AskData> condidate_trains;
  if (same_index.size()) {
    int cnt = 0;
    for (auto it = same_index.begin(); it != same_index.end(); it++) {
      TrainInfo res;
      train_info.read(res, *it);
      int start_index, end_index;
      start_index = res.FindIndex2(start.c_str());
      end_index = res.FindIndex2(end.c_str());
      if (end_index > start_index && start_index >= 0 && end_index >= 0) {
        AskData to_push;
        to_push.ID = res.ID;
        to_push.end_index = end_index;
        to_push.start_index = start_index;
        to_push.price = res.AskPrice(start_index, end_index);
        to_push.out_time = res.AskOutTime(start_index, month, day);
        to_push.start_time =
            res.AskLeaveTime(start_index, to_push.out_time.GetMonth(),
                             to_push.out_time.GetDay());
        to_push.end_time = res.AskArriveTime(
            end_index, to_push.out_time.GetMonth(), to_push.out_time.GetDay());
        to_push.time = res.AskTime(start_index, end_index);
        condidate_trains.push_back(to_push);
      }
    }
  }
  sjtu::list<AskData> confirmed_data;
  for (auto it = condidate_trains.begin(); it != condidate_trains.end(); it++) {
    unsigned long long hash1, hash2;
    hash1 = sjtu::MyHash(it->ID, exp1);
    hash2 = sjtu::MyHash(it->ID, exp2);
    TrainDay to_find((it->out_time).GetMonth(), (it->out_time).GetDay(), 0);
    TrainDayIndex to_find_index;
    to_find_index.month = to_find.month;
    to_find_index.day = to_find.day;
    auto bigger_find = to_find_index;
    bigger_find.day++;
    auto trains =
        trains_day_index.find2(hash1, hash2, to_find_index, bigger_find);
    if (trains.size()) {
      auto to_check_index = trains.front();
      TrainDay to_check;
      train_day_info.read(to_check, to_check_index.index);
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
  if (by_time) {
    sjtu::priority_queue<AskData, SortTrainByTime> output;
    for (auto it = confirmed_data.begin(); it != confirmed_data.end(); it++) {
      output.push(*it);
    }
    while (!output.empty()) {
      output_list.push_back(output.top());
      output.pop();
    }
  } else {
    sjtu::priority_queue<AskData, SortTrainByCost> output;
    for (auto it = confirmed_data.begin(); it != confirmed_data.end(); it++) {
      output.push(*it);
    }
    while (!output.empty()) {
      output_list.push_back(output.top());
      output.pop();
    }
  }
  std::cout << output_list.size() << '\n';
  for (auto it = output_list.begin(); it != output_list.end(); it++) {
    std::cout << it->ID << ' ' << start << ' ';
    it->start_time.Print();
    std::cout << "-> " << end << ' ';
    it->end_time.Print();
    std::cout << it->price << ' ' << it->seat << '\n';
  }
  return;
}

int TrainInfo::AskTime(const int &start_index, const int &end_index) const {
  int ans = 0;
  for (int i = start_index + 1; i < end_index; i++) {
    ans += travel[i];
    ans += stop[i];
  }
  ans += travel[end_index];
  return ans;
}
bool SortTrainByTime::operator()(const AskData &lhs, const AskData &rhs) const {
  if (lhs.time != rhs.time) {
    return (lhs.time < rhs.time);
  }
  return (lhs.ID < rhs.ID);
}
bool SortTrainByCost::operator()(const AskData &lhs, const AskData &rhs) const {
  if (lhs.price != rhs.price) {
    return (lhs.price < rhs.price);
  }
  return (lhs.ID < rhs.ID);
}
void QueryTransfer(string &command) {
  TransferData ans;
  string start, end, date;
  bool by_time = true;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-s") {
      start = ProcessTxt(command);
      continue;
    }
    if (op == "-t") {
      end = ProcessTxt(command);
      continue;
    }
    if (op == "-d") {
      date = ProcessTxt(command);
      continue;
    }
    if (op == "-p") {
      string res = ProcessTxt(command);
      if (res == "time") {
        continue;
      }
      if (res == "cost") {
        by_time = false;
        continue;
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
  sjtu::map<int, bool> end_indexs;
  for (auto it = end_index_raw.begin(); it != end_index_raw.end(); it++) {
    end_indexs[*it] = true;
  }
  sjtu::list<TransferData> transfers;
  for (auto it = start_index_raw.begin(); it != start_index_raw.end(); it++) {
    TrainInfo res;
    train_info.read(res, *it);
    int start_index = res.FindIndex(start.c_str());
    Time out_time = res.AskOutTime(start_index, month, day);
    if (!res.CheckAvailable(out_time)) {
      continue;
    }
    unsigned long long id_hash1, id_hash2;
    id_hash1 = sjtu::MyHash(res.ID, exp1);
    id_hash2 = sjtu::MyHash(res.ID, exp2);
    TrainDay actual_train;
    actual_train.month = out_time.GetMonth();
    actual_train.day = out_time.GetDay();
    TrainDayIndex to_find;
    to_find.month = actual_train.month;
    to_find.day = actual_train.day;
    auto bigger = to_find;
    bigger.day++;
    auto day_trains_raw =
        trains_day_index.find2(id_hash1, id_hash2, to_find, bigger);
    if ((day_trains_raw.empty())) {
      continue;
    }
    if ((day_trains_raw.front().day != actual_train.day) ||
        (day_trains_raw.front().month != day_trains_raw.front().month)) {
      continue;
    }
    train_day_info.read(actual_train, day_trains_raw.front().index);
    AskData first_train;
    first_train.ID = res.ID;
    first_train.out_time = out_time;
    first_train.start_time =
        res.AskLeaveTime(start_index, out_time.GetMonth(), out_time.GetDay());
    first_train.seat = 2e7;
    for (int i = (start_index + 1); i < res.station_number; i++) {
      string inter_station = res.stations[i];
      if (inter_station == end) {
        continue;
      }
      unsigned long long station_hash1, station_hash2;
      station_hash1 = sjtu::MyHash(inter_station, exp1);
      station_hash2 = sjtu::MyHash(inter_station, exp2);
      first_train.end_time =
          res.AskArriveTime(i, out_time.GetMonth(), out_time.GetDay());
      first_train.time = res.AskTime(start_index, i);
      first_train.price = res.AskPrice(start_index, i);
      first_train.seat = std::min(actual_train.ticket[i - 1], first_train.seat);
      auto possible_trains =
          station_database.find(station_hash1, station_hash2, -1);
      for (auto it2 = possible_trains.begin(); it2 != possible_trains.end();
           it2++) {
        if (!end_indexs.count(*it2)) {
          continue;
        } // Can't reach the target.
        if ((*it) == (*it2)) {
          continue;
        }
        TrainInfo second_train_info;
        train_info.read(second_train_info, *it2);
        int start_index2, end_index2;
        start_index2 = second_train_info.FindIndex(inter_station.c_str());
        end_index2 = second_train_info.FindIndex(end.c_str());
        if (end_index2 < start_index2) {
          continue;
        } // The direction is reversed.
        Time test_out_time;
        test_out_time = second_train_info.AskOutTime(
            start_index2, first_train.end_time.GetMonth(),
            first_train.end_time.GetDay());
        Time test_leave_time;
        test_leave_time = second_train_info.AskLeaveTime(
            start_index2, test_out_time.GetMonth(), test_out_time.GetDay());
        while (test_leave_time < first_train.end_time) {
          test_out_time.Add(24 * 60);
          test_leave_time = second_train_info.AskLeaveTime(
              start_index2, test_out_time.GetMonth(), test_out_time.GetDay());
        }
        unsigned long long id2_hash1, id2_hash2;
        id2_hash1 = sjtu::MyHash(second_train_info.ID, exp1);
        id2_hash2 = sjtu::MyHash(second_train_info.ID, exp2);
        TrainDay second_day_train;
        second_day_train.month = test_out_time.GetMonth();
        second_day_train.day = test_out_time.GetDay();
        TrainDayIndex second_index;
        second_index.month = second_day_train.month;
        second_index.day = second_day_train.day;
        auto second_day_trains =
            trains_day_index.find(id2_hash1, id2_hash2, second_index);
        if (second_day_trains.empty()) {
          continue;
        }
        train_day_info.read(second_day_train, second_day_trains.front().index);
        AskData second_train;
        second_train.ID = second_train_info.ID;
        second_train.start_index = start_index2;
        second_train.end_index = end_index2;
        second_train.time = second_train_info.AskTime(start_index2, end_index2);
        second_train.price =
            second_train_info.AskPrice(start_index2, end_index2);
        second_train.out_time = second_train_info.AskLeaveTime(
            0, second_day_train.month, second_day_train.day);
        second_train.start_time = second_train_info.AskLeaveTime(
            start_index2, second_day_train.month, second_day_train.day);
        second_train.end_time = second_train_info.AskArriveTime(
            end_index2, second_day_train.month, second_day_train.day);
        second_train.seat = 2e7;
        for (int k = start_index2; k < end_index2; k++) {
          second_train.seat =
              std::min(second_train.seat, second_day_train.ticket[k]);
        }
        TransferData condidate_answer;
        condidate_answer.line1 = first_train;
        condidate_answer.line2 = second_train;
        condidate_answer.transfer = inter_station;
        if (by_time) {
          CompareTransferByTime tool;
          if (tool(condidate_answer, ans) || (ans.transfer == "")) {
            ans = condidate_answer;
          }
        } else {
          CompareTransferByCost tool;
          if (tool(condidate_answer, ans) || (ans.transfer == "")) {
            ans = condidate_answer;
          }
        }
      }
    }
  }
  if (ans.transfer == "") {
    std::cout << 0 << '\n';
    return;
  }
  std::cout << ans.line1.ID << ' ' << start << ' ';
  ans.line1.start_time.Print();
  std::cout << "-> " << ans.transfer << ' ';
  ans.line1.end_time.Print();
  std::cout << ans.line1.price << ' ' << ans.line1.seat << '\n';
  std::cout << ans.line2.ID << ' ' << ans.transfer << ' ';
  ans.line2.start_time.Print();
  std::cout << "-> " << end << ' ';
  ans.line2.end_time.Print();
  std::cout << ans.line2.price << ' ' << ans.line2.seat << '\n';
  return;
}

bool TrainInfo::CheckAvailable(const Time &out_time) const {
  if (out_time.GetMonth() < sale_month) {
    return false;
  }
  if (out_time.GetMonth() > des_month) {
    return false;
  }
  if ((out_time.GetMonth() == sale_month) && (out_time.GetDay() < sale_day)) {
    return false;
  }
  if ((out_time.GetMonth() == des_month) && (out_time.GetDay() > des_day)) {
    return false;
  }
  return true;
}
bool CompareTransferByCost::operator()(const TransferData &lhs,
                                       const TransferData &rhs) const {
  int price_1 = (lhs.line1.price + lhs.line2.price);
  int price_2 = (rhs.line1.price + rhs.line2.price);
  if (price_1 != price_2) {
    return price_1 < price_2;
  }
  int time_1 = IntervalMinute(lhs.line1.start_time, lhs.line2.end_time);
  int time_2 = IntervalMinute(rhs.line1.start_time, rhs.line2.end_time);
  if (time_1 != time_2) {
    return (time_1 < time_2);
  }
  if (lhs.line1.ID != rhs.line1.ID) {
    return (lhs.line1.ID < rhs.line1.ID);
  }
  return (lhs.line2.ID < rhs.line2.ID);
}
bool CompareTransferByTime::operator()(const TransferData &lhs,
                                       const TransferData &rhs) const {
  int time_1 = IntervalMinute(lhs.line1.start_time, lhs.line2.end_time);
  int time_2 = IntervalMinute(rhs.line1.start_time, rhs.line2.end_time);
  if (time_1 != time_2) {
    return (time_1 < time_2);
  }
  int price_1 = (lhs.line1.price + lhs.line2.price);
  int price_2 = (rhs.line1.price + rhs.line2.price);
  if (price_1 != price_2) {
    return price_1 < price_2;
  }
  if (lhs.line1.ID != rhs.line1.ID) {
    return (lhs.line1.ID < rhs.line1.ID);
  }
  return (lhs.line2.ID < rhs.line2.ID);
}
bool TrainDayIndex::operator<(const TrainDayIndex &other) const {
  if (month != other.month) {
    return month < other.month;
  }
  return day < other.day;
}
bool TrainDayIndex::operator>(const TrainDayIndex &other) const {
  return (other < *this);
}
bool TrainDayIndex::operator==(const TrainDayIndex &other) const {
  return (!(*this < other)) && (!(other < *this));
}
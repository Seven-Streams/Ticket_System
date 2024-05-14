#include "../include/order.hpp"
#include <cstring>
#include <string>
using std::string;
sjtu::BPT<OrderByUser> order_user("order_user");
sjtu::BPT<OrderByTrain> queue_list("queue");
// Consider that if a user ordered first, the index will be smaller.
extern sjtu::list<HashOfAccount> account_logged;
extern sjtu::BPT<int> train_index;
extern sjtu::MemoryRiver<TrainInfo, 1> train_info;
extern sjtu::BPT<TrainDay> trains_day;
void QueryOrder(string &command) {
  string op = ProcessTxt(command);
  if (op != "-u") {
    throw(SevenStream::exception("The user doesn't log in."));
  }
  string user_name = ProcessTxt(command);
  bool logged = false;
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if ((*it) == user_name) {
      logged = true;
      break;
    }
  }
  if (!logged) {
    throw(SevenStream::exception("The account doesn't log in."));
  }
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user_name, exp1);
  hash2 = sjtu::MyHash(user_name, exp2);
  OrderByUser min_element;
  min_element.stamp = maxn;
  auto orders = order_user.find(hash1, hash2, min_element);
  for (auto it = orders.begin(); it != orders.end(); it++) {
    (*it).Print();
  }
  return;
}

bool OrderByTrain::operator<(const OrderByTrain &rhs) const {
  if (start_month != rhs.start_month) {
    return rhs.start_month > start_month;
  }
  if (start_day != rhs.start_day) {
    return rhs.start_day > start_day;
  }
  return stamp < rhs.stamp;
}
bool OrderByTrain::operator>(const OrderByTrain &rhs) const {
  return rhs < *this;
}
bool OrderByTrain::operator==(const OrderByTrain &rhs) const {
  return (!(*this < rhs)) && (!(rhs < *this));
}
bool OrderByUser::operator<(const OrderByUser &rhs) const {
  return stamp > rhs.stamp;
}
bool OrderByUser::operator>(const OrderByUser &rhs) const {
  return stamp < rhs.stamp;
}
bool OrderByUser::operator==(const OrderByUser &rhs) const {
  return stamp == rhs.stamp;
}

void OrderByUser::Print() {
  switch (status) {
  case (1): {
    std::cout << "[success] ";
    break;
  }
  case (2): {
    std::cout << "[pending] ";
    break;
  }
  case (3): {
    std::cout << "[refunded] ";
    break;
  }
  default: {
    throw(SevenStream::exception("Invalid status."));
  }
  }
  std::cout << Train_ID << ' ';
  std::cout << start_station << ' ';
  start_time.Print();
  std::cout << "->";
  std::cout << end_station << ' ';
  end_time.Print();
  std::cout << price << ' ';
  std::cout << number << '\n';
}

void Buy(std::string &command, int stamp) {
  bool queue = false;
  string user, id, date, number_raw, start_station, end_station;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-u") {
      user = ProcessTxt(command);
      CheckUsername(user.c_str());
      continue;
    }
    if (op == "-i") {
      id = ProcessTxt(command);
      CheckTrainID(id.c_str());
      continue;
    }
    if (op == "-d") {
      date = ProcessTxt(command);
      CheckDate(date.c_str());
      continue;
    }
    if (op == "-n") {
      number_raw = ProcessTxt(command);
      CheckNumber(number_raw.c_str());
      continue;
    }
    if (op == "-f") {
      start_station = ProcessTxt(command);
      CheckStation(start_station.c_str());
      continue;
    }
    if (op == "-t") {
      end_station = ProcessTxt(command);
      CheckStation(end_station.c_str());
      continue;
    }
    if (op == "-q") {
      string res = ProcessTxt(command);
      if ((res != "true") && (res != "false")) {
        throw(SevenStream::exception("Invalid queue_status."));
      }
      if (res == "true") {
        queue = true;
      }
    }
  }
  HashOfAccount hash_user(user);
  bool logged = false;
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if (hash_user == (*it)) {
      logged = true;
      break;
    }
  }
  if (!logged) {
    throw(SevenStream::exception("Not log in."));
  }
  unsigned long long id_hash1, id_hash2;
  id_hash1 = sjtu::MyHash(id, exp1);
  id_hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(id_hash1, id_hash2, minus_max);
  int index = index_raw.front();
  TrainInfo train_total;
  train_info.read(train_total, index);
  if (!train_total.IsReleased()) {
    throw(SevenStream::exception("The train haven't been released."));
  }
  int start_index, end_index;
  start_index = train_total.FindIndex(start_station.c_str());
  end_index = train_total.FindIndex(end_station.c_str());
  if (end_index <= start_index) {
    std::cout << start_index << ' ' << end_index << std::endl;
    throw(SevenStream::exception("Invalid station."));
  }
  int number = std::stoi(number_raw);
  int price = train_total.AskPrice(start_index, end_index);
  int ask_month = date[0] - '0';
  ask_month *= 10;
  ask_month += date[1] - '0';
  int ask_day = date[3] - '0';
  ask_day *= 10;
  ask_day += date[4] - '0';
  Time out_time = train_total.AskOutTime(start_index, ask_month, ask_day);
  int out_month = out_time.GetMonth();
  int out_day = out_time.GetDay();
  if (!train_total.IsSaleTime(out_month, out_day)) {
    throw(SevenStream::exception("Not in sale time."));
  }
  TrainDay acutual_train(out_month, out_day, 0);
  auto find = trains_day.find(id_hash1, id_hash2, acutual_train);
  acutual_train = find.front();
  int available = 1e7;
  for (int i = start_index; i < end_index; i++) {
    available = std::min(available, acutual_train.ticket[i]);
  }
  if ((available < number) && (!queue)) {
    throw(SevenStream::exception("Not enough tickets."));
  }
  OrderByUser order_by_user;
  strcpy(order_by_user.end_station, end_station.c_str());
  strcpy(order_by_user.Train_ID, id.c_str());
  strcpy(order_by_user.start_station, start_station.c_str());
  order_by_user.start_index = start_index;
  order_by_user.end_index = end_index;
  order_by_user.out_month = out_month;
  order_by_user.out_day = out_day;
  order_by_user.number = number;
  order_by_user.stamp = stamp;
  order_by_user.price = price;
  order_by_user.start_time =
      train_total.AskLeaveTime(start_index, out_month, out_day);
  order_by_user.end_time =
      train_total.AskArriveTime(end_index, out_month, out_day);
  unsigned long long user_hash1, user_hash2;
  user_hash1 = sjtu::MyHash(user, exp1);
  user_hash2 = sjtu::MyHash(user, exp2);
  if (available < number) {
    order_by_user.status = 2;
    OrderByTrain to_queue;
    to_queue.user_hash1 = user_hash1;
    to_queue.user_hash2 = user_hash2;
    to_queue.end_station = end_index;
    to_queue.start_station = start_index;
    to_queue.start_month = out_month;
    to_queue.start_day = out_day;
    to_queue.number = number;
    to_queue.stamp = stamp;
    queue_list.Insert(id_hash1, id_hash2, to_queue);
    order_user.Insert(user_hash1, user_hash2, order_by_user);
    std::cout << "queue\n";
  } else {
    order_by_user.status = 1;
    order_user.Insert(user_hash1, user_hash2, order_by_user);
    for (int i = start_index; i < end_index; i++) {
      acutual_train.ticket[i] -= number;
    }
    trains_day.Erase(id_hash1, id_hash2, acutual_train);
    trains_day.Insert(id_hash1, id_hash2, acutual_train);
    int total_price = number * price;
    std::cout << total_price << '\n';
  }
  return;
}
void Refund(std::string &command) {
  int number = 1;
  string user;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-u") {
      user = ProcessTxt(command);
    }
    if (op == "-n") {
      string res = ProcessTxt(command);
      number = std::stoi(res);
    }
  }
  bool logged = false;
  HashOfAccount to_check(user);
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if ((*it) == to_check) {
      logged = true;
      break;
    }
  }
  if (!logged) {
    throw(SevenStream::exception("The account doesn't login."));
  }
  unsigned long long user_hash1, user_hash2;
  user_hash1 = sjtu::MyHash(user, exp1);
  user_hash2 = sjtu::MyHash(user, exp2);
  OrderByUser nothing;
  nothing.stamp = maxn;
  auto orders = order_user.find(user_hash1, user_hash2, nothing);
  int cnt = 1;
  auto it = orders.begin();
  while (cnt < number) {
    it++;
  }
  auto to_refund = *it;
  to_refund.status = 3;
  order_user.Erase(user_hash1, user_hash2, to_refund);
  order_user.Insert(user_hash1, user_hash2, to_refund);
  unsigned long long id_hash1, id_hash2;
  id_hash1 = sjtu::MyHash(to_refund.Train_ID, exp1);
  id_hash2 = sjtu::MyHash(to_refund.Train_ID, exp2);
  TrainDay empty_train(to_refund.out_month, to_refund.out_day, 0);
  auto train_actual_raw = trains_day.find(id_hash1, id_hash2, empty_train);
  auto train_actual = train_actual_raw.front();
  for (int i = to_refund.start_index; i < to_refund.end_index; i++) {
    train_actual.ticket[i] += to_refund.number;
  }
  OrderByTrain nothing_order;
  nothing_order.start_month = to_refund.out_month;
  nothing_order.start_day = to_refund.out_day;
  nothing_order.stamp = minus_max;
  auto queues = queue_list.find(id_hash1, id_hash2, nothing_order);
  for (auto it = queues.begin(); it != queues.end(); it++) {
    bool OK = true;
    for (int j = it->start_station; j < it->end_station; j++) {
      if (train_actual.ticket[j] < it->number) {
        OK = false;
        break;
      }
    }
    if (OK) {
      for (int j = it->start_station; j < it->end_station; j++) {
        train_actual.ticket[j] -= it->number;
      }
      queue_list.Erase(id_hash1, id_hash2, *it);
      OrderByUser to_change;
      to_change.stamp = it->stamp;
      auto to_find = order_user.find(it->user_hash1, it->user_hash2, to_change);
      to_change = to_find.front();
      to_change.status = 1;
      order_user.Erase(it->user_hash1, it->user_hash2, to_change);
      order_user.Insert(it->user_hash1, it->user_hash2, to_change);
    }
  }
  return;
}
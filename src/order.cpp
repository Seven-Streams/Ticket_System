#include "../include/order.hpp"

using std::string;
sjtu::BPT<OrderByUser, 19, 20> order_user("order_user");
sjtu::BPT<OrderByTrain, 62, 12> queue_list("queue");
// Consider that if a user ordered first, the index will be smaller.
extern sjtu::map<HashOfAccount, bool, sjtu::Less<HashOfAccount>> account_logged;
extern sjtu::BPT<int> train_index;
extern sjtu::MemoryRiver<TrainInfo, 1> train_info;
extern sjtu::MemoryRiver<TrainDay, 1> train_day_info;
extern sjtu::BPT<TrainDayIndex, 126, 4, 1700> trains_day_index;
void QueryOrder(string &command) {
  string op = ProcessTxt(command);
  if (op != "-u") {
    std::cout << "-1\n";return;//throw(SevenStream::exception("The user doesn't log in."));
  }
  string user_name = ProcessTxt(command);
  bool logged = false;
  HashOfAccount user_hash(user_name);
  if (!account_logged.count(user_hash)) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("The account doesn't log in."));
  }
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user_name, exp1);
  hash2 = sjtu::MyHash(user_name, exp2);
  OrderByUser min_element;
  min_element.stamp = maxn;
  auto orders = order_user.find(hash1, hash2, min_element);
  std::cout << orders.size() << '\n';
  if (orders.size()) {
    for (auto it = orders.begin(); it != orders.end(); it++) {
      (*it).Print();
    }
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
  return (stamp < rhs.stamp);
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

void OrderByUser::Print() const {
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
  std::cout << "-> ";
  std::cout << end_station << ' ';
  end_time.Print();
  std::cout << price << ' ';
  std::cout << number << '\n';
}

void Buy(std::string &command, const int stamp) {
  bool queue = false;
  string user, id, date, number_raw, start_station, end_station;
  while (command != "") {
    string op = ProcessTxt(command);
    if (op == "-u") {
      user = ProcessTxt(command);
      continue;
    }
    if (op == "-i") {
      id = ProcessTxt(command);
      continue;
    }
    if (op == "-d") {
      date = ProcessTxt(command);
      continue;
    }
    if (op == "-n") {
      number_raw = ProcessTxt(command);
      continue;
    }
    if (op == "-f") {
      start_station = ProcessTxt(command);
      continue;
    }
    if (op == "-t") {
      end_station = ProcessTxt(command);
      continue;
    }
    if (op == "-q") {
      string res = ProcessTxt(command);
      if ((res != "true") && (res != "false")) {
        std::cout << "-1\n";
        return; // throw(SevenStream::exception("Invalid queue_status."));
      }
      if (res == "true") {
        queue = true;
      }
    }
  }
  HashOfAccount hash_user(user);
  if (!account_logged.count(hash_user)) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Not log in."));
  }
  unsigned long long id_hash1, id_hash2;
  id_hash1 = sjtu::MyHash(id, exp1);
  id_hash2 = sjtu::MyHash(id, exp2);
  auto index_raw = train_index.find(id_hash1, id_hash2, minus_max);
  if (index_raw.empty()) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The Train Doesn't exist."));
  }
  int index = index_raw.front();
  TrainInfo train_total;
  train_info.read(train_total, index);
  if (!train_total.IsReleased()) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("The train haven't been
            // released."));
  }
  int start_index, end_index;
  start_index = train_total.FindIndex2(start_station.c_str());
  end_index = train_total.FindIndex2(end_station.c_str());
  if (end_index <= start_index || (start_index < 0) || (end_index < 0)) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Invalid station."));
  }
  int number = std::stoi(number_raw);
  if (number > train_total.seat_number) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("There aren't enough seats!"));
  }
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
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Not in sale time."));
  }
  TrainDay acutual_train(out_month, out_day, 0);
  TrainDayIndex train_index;
  train_index.month = acutual_train.month;
  train_index.day = acutual_train.day;
  auto bigger_train = train_index;
  bigger_train.day++;
  auto find =
      trains_day_index.find2(id_hash1, id_hash2, train_index, bigger_train);
  if (find.empty()) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("No available train."));
  }
  train_day_info.read(acutual_train, find.front().index);
  int available = 1e7;
  for (int i = start_index; i < end_index; i++) {
    available = std::min(available, acutual_train.ticket[i]);
  }
  if ((available < number) && (!queue)) {
    std::cout << "-1\n";
    return; // throw(SevenStream::exception("Not enough tickets."));
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
    TrainDayIndex to_find;
    to_find.month = acutual_train.month;
    to_find.day = acutual_train.day;
    auto to_find_raw = trains_day_index.find(id_hash1, id_hash2, to_find);
    train_day_info.write(acutual_train, to_find_raw.front().index);
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
  if (!account_logged.count(to_check)) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("The account doesn't login."));
  }
  unsigned long long user_hash1, user_hash2;
  user_hash1 = sjtu::MyHash(user, exp1);
  user_hash2 = sjtu::MyHash(user, exp2);
  OrderByUser nothing;
  nothing.stamp = maxn;
  auto orders = order_user.find(user_hash1, user_hash2, nothing);
  int cnt = 1;
  if (number > orders.size()) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("There aren't enough orders."));
  }
  auto it = orders.begin();
  while (cnt < number) {
    it++;
    cnt++;
  }
  auto to_refund = *it;
  if (to_refund.status == 3) {
    std::cout << "-1\n";return;//throw(SevenStream::exception("Have been refunded."));
  }
  if (to_refund.status == 2) {
    to_refund.status = 3;
    order_user.Replace(user_hash1, user_hash2, to_refund);
    OrderByTrain to_remove;
    to_remove.stamp = to_refund.stamp;
    to_remove.start_month = to_refund.out_month;
    to_remove.start_day = to_refund.out_day;
    unsigned long long hash1, hash2;
    hash1 = sjtu::MyHash(to_refund.Train_ID, exp1);
    hash2 = sjtu::MyHash(to_refund.Train_ID, exp2);
    queue_list.Erase(hash1, hash2, to_remove);
    std::cout << "0\n";return;//
  }
  to_refund.status = 3;
  order_user.Replace(user_hash1, user_hash2, to_refund);
  unsigned long long id_hash1, id_hash2;
  id_hash1 = sjtu::MyHash(to_refund.Train_ID, exp1);
  id_hash2 = sjtu::MyHash(to_refund.Train_ID, exp2);
  TrainDay empty_train(to_refund.out_month, to_refund.out_day, 0);
  TrainDayIndex train_index;
  train_index.month = empty_train.month;
  train_index.day = empty_train.day;
  auto bigger_train = train_index;
  bigger_train.day++;
  auto train_actual_raw =
      trains_day_index.find2(id_hash1, id_hash2, train_index, bigger_train);
  TrainDay train_actual;
  train_day_info.read(train_actual, train_actual_raw.front().index);
  for (int i = to_refund.start_index; i < to_refund.end_index; i++) {
    train_actual.ticket[i] += to_refund.number;
  }
  OrderByTrain nothing_order;
  nothing_order.start_month = to_refund.out_month;
  nothing_order.start_day = to_refund.out_day;
  nothing_order.stamp = minus_max;
  OrderByTrain biggest = nothing_order;
  biggest.stamp = maxn;
  auto queues = queue_list.find2(id_hash1, id_hash2, nothing_order, biggest);
  for (auto it = queues.begin(); it != queues.end(); it++) {
    if ((it->start_day != to_refund.out_day) ||
        (it->start_month != to_refund.out_month)) {
      break;
    }
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
      OrderByUser bigger = to_change;
      bigger.stamp++;
      to_change.stamp = it->stamp;
      auto to_find =
          order_user.find2(it->user_hash1, it->user_hash2, to_change, bigger);
      to_change = to_find.front();
      to_change.status = 1;
      order_user.Replace(it->user_hash1, it->user_hash2, to_change);
    }
  }
  TrainDayIndex to_change;
  to_change.month = train_actual.month;
  to_change.day = train_actual.day;
  auto index = trains_day_index.find(id_hash1, id_hash2, to_change);
  train_day_info.write(train_actual, index.front().index);
  std::cout << "0\n";return;
}
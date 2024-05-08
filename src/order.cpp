#include "../include/order.hpp"
using std::string;
sjtu::BPT<OrderByUser> order_user("order_user");
sjtu::BPT<OrderByTrain> order_train("order_train");
// Consider that if a user ordered first, the index will be smaller.
extern sjtu::list<HashOfAccount> account_logged;
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
  for(auto it = orders.begin(); it != orders.end(); it++) {
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
  switch(status) {
    case(1): {
      std::cout << "[success] ";
      break;
    }
    case(2): {
      std::cout << "[pending] ";
      break;
    }
    case(3): {
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
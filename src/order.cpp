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
  string orders = order_user.find(hash1, hash2);
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
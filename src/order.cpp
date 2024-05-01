#include "../include/order.hpp"
using std::string;
sjtu::MemoryRiver<Order, 1> order_info("order_info");
sjtu::BPT<int> order_id("order_id");
sjtu::BPT<int> order_user("order_user");
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
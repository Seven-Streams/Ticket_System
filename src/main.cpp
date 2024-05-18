#include "../include/account.hpp"
#include "../include/order.hpp"
#include "../include/train.hpp"
#include "../include/utility.hpp"
using std::cin;
using std::cout;
using std::string;
class Account;
extern sjtu::MemoryRiver<Account, 1> account_content;
extern sjtu::map<HashOfAccount, int, sjtu::Less<HashOfAccount>> account_logged;
extern sjtu::BPT<int> account_index;
extern sjtu::BPT<OrderByUser, 19, 20> order_user;
extern sjtu::BPT<OrderByTrain, 62, 12> queue_list;
extern sjtu::BPT<int> train_index;
extern sjtu::MemoryRiver<TrainInfo, 1> train_info;
extern sjtu::BPT<int, 168, 6, 300> station_database;
extern sjtu::BPT<TrainDayIndex, 126, 4, 1700> trains_day_index;
extern sjtu::MemoryRiver<TrainDay, 1> train_day_info;
void Clean() {
  account_content.clear();
  account_logged.clear();
  account_index.Clear();
  order_user.Clear();
  queue_list.Clear();
  train_index.Clear();
  train_info.clear();
  station_database.Clear();
  trains_day_index.Clear();
  train_day_info.clear();
  return;
}
int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);
  string command;
  bool has_account;
  int total;
  account_content.get_info(total, 1);
  if (total) {
    has_account = true;
  } else {
    has_account = false;
  }
  while (!cin.eof()) {
    string stamp;
    getline(cin, command);
    stamp = ProcessTxt(command);
    string type;
    type = ProcessTxt(command);
    if (type == "query_profile") {
      cout << stamp << ' ';
      std::string ans = QueryAccount(command);
      cout << ans << '\n';
      continue;
    }
    if (type == "query_ticket") {
      cout << stamp << ' ';
      QueryTicket(command);
      continue;
    }
    if (type == "buy_ticket") {
      cout << stamp << ' ';
      string num_raw = stamp;
      num_raw.erase(0, 1);
      num_raw.pop_back();
      int number = std::stoi(num_raw);
      Buy(command, number);
      continue;
    }
    if (type == "query_transfer") {
      cout << stamp << ' ';
      QueryTransfer(command);
      continue;
    }
    if (type == "login") {
      cout << stamp << ' ';
      Login(command);
      continue;
    }
    if (type == "logout") {
      cout << stamp << ' ';
      Logout(command);
      continue;
    }
    if (type == "modify_profile") {
      cout << stamp << ' ';
      std::string ans = ModifyAccount(command);
      cout << ans << '\n';
      continue;
    }
    if (type == "exit") {
      cout << stamp << " bye\n";
      return 0;
    }
    if (type == "query_order") {
      cout << stamp << ' ';
      QueryOrder(command);
      continue;
    }
    if (type == "add_user") {
      cout << stamp << ' ';
      if (has_account) {
        AddAccount(command);
      } else {
        AddFirstAccount(command);
        has_account = true;
      }
      continue;
    }
    if (type == "add_train") {
      cout << stamp << ' ';
      AddTrain(command);
      continue;
    }
    if (type == "delete_train") {
      cout << stamp << ' ';
      DeleteTrain(command);
      continue;
    }
    if (type == "release_train") {
      cout << stamp << ' ';
      ReleaseTrain(command);
      continue;
    }
      if (type == "query_train") {
        cout << stamp << ' ';
        QueryTrain(command);
        continue;
      }
      if (type == "refund_ticket") {
        cout << stamp << ' ';
        Refund(command);
        continue;
      }
      if (type == "clean") {
        cout << stamp << ' ';
        Clean();
        cout << "0\n";
        continue;
      }
  }
  return 0;
}
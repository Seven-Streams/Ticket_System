#include "../include/account.hpp"
#include "../include/exception.hpp"
#include "../include/order.hpp"
#include "../include/store.hpp"
#include "../include/train.hpp"
#include "../include/utility.hpp"
#include "../include/valid.hpp"
#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::string;
class Account;
extern sjtu::MemoryRiver<Account, 1> account_content;
extern sjtu::list<HashOfAccount> account_logged;
extern sjtu::BPT<int, 70, 20> account_index;
extern sjtu::BPT<OrderByUser> order_user;
extern sjtu::BPT<OrderByTrain> queue_list;
extern sjtu::BPT<int> train_index;
extern sjtu::MemoryRiver<TrainInfo, 1> train_info;
extern sjtu::BPT<int> station_database;
extern sjtu::BPT<TrainDay> trains_day;
void Clean() {
  account_content.clear();
  account_logged.clear();
  account_index.Clear();
  order_user.Clear();
  queue_list.Clear();
  train_index.Clear();
  train_info.clear();
  station_database.Clear();
  trains_day.Clear();
  return;
}
int main() {
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
    try {
      getline(cin, command);
      stamp = ProcessTxt(command);
      string type;
      type = ProcessTxt(command);
      if (type == "add_user") {
        cout << stamp << ' ';
        if (has_account) {
          AddAccount(command);
        } else {
          AddFirstAccount(command);
          has_account = true;
        }
        cout << "0" << std::endl;
        continue;
      }
      if (type == "login") {
        cout << stamp << ' ';
        Login(command);
        cout << "0" << std::endl;
        continue;
      }
      if (type == "logout") {
        cout << stamp << ' ';
        Logout(command);
        cout << "0" << std::endl;
        continue;
      }
      if (type == "query_profile") {
        cout << stamp << ' ';
        std::string ans = QueryAccount(command);
        cout << ans << std::endl;
        continue;
      }
      if (type == "modify_profile") {
        cout << stamp << ' ';
        std::string ans = ModifyAccount(command);
        cout << ans << std::endl;
        continue;
      }
      if (type == "exit") {
        cout << stamp << " bye\n";
        return 0;
      }
      if (type == "add_train") {
        cout << stamp << ' ';
        AddTrain(command);
        cout << "0\n";
        continue;
      }
      if (type == "delete_train") {
        cout << stamp << ' ';
        DeleteTrain(command);
        cout << "0\n";
        continue;
      }
      if (type == "release_train") {
        cout << stamp << ' ';
        ReleaseTrain(command);
        cout << "0\n";
        continue;
      }
      if (type == "query_train") {
        cout << stamp << ' ';
        QueryTrain(command);
        continue;
      }
      if (type == "query_ticket") {
        cout << stamp << ' ';
        QueryTicket(command);
        continue;
      }
      if (type == "query_transfer") {
        cout << stamp << ' ';
        QueryTransfer(command);
        continue;
      }
      if (type == "buy_ticket") {
        cout << stamp << ' ';
        string num_raw = stamp;
        num_raw.erase(0, 1);
        num_raw.erase(num_raw.size() - 1, 1);
        int number = std::stoi(num_raw);
        Buy(command, number);
        continue;
      }
      if (type == "query_order") {
        cout << stamp << ' ';
        QueryOrder(command);
        continue;
      }
      if (type == "refund_ticket") {
        cout << stamp << ' ';
        Refund(command);
        cout << "0\n";
        continue;
      }
      if (type == "clean") {
        cout << stamp << ' ';
        Clean();
        cout << "0\n";
        continue;
      }
    } catch (SevenStream::exception &e) {
      cout << e.what() << std::endl;
      // cout <<  "-1" << std::endl;
    }
  }
  return 0;
}
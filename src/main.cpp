#include "../include/account.hpp"
#include "../include/exception.hpp"
#include "../include/utility.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
#include "../include/train.hpp"
#include <iostream>
using std::cin;
using std::cout;
using std::string;
class Account;
extern sjtu::MemoryRiver<Account, 1> account_content;
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
      cin >> command;
      stamp = ProcessTxt(command);
      string type;
      type = ProcessTxt(command);
      if (type == "add_user") {
        if (has_account) {
          AddAccount(command);
        } else {
          AddFirstAccount(command);
          has_account = true;
        }
        cout << stamp << " 0\n";
        continue;
      }
      if (type == "login") {
        Login(command);
        cout << stamp << " 0\n";
        continue;
      }
      if (type == "logout") {
        Logout(command);
        cout << stamp << " 0\n";
        continue;
      }
      if (type == "query_profile") {
        std::string ans = QueryAccount(command);
        cout << stamp << ' ' << ans << '\n';
        continue;
      }
      if (type == "modify_profile") {
        std::string ans = ModifyAccount(command);
        cout << stamp << ' ' << ans << '\n';
        continue;
      }
      if (type == "exit") {
        cout << stamp << " bye\n";
        return 0;
      }
      if (type == "add_train") {
        AddTrain(command);
        cout << stamp << "0\n";
        continue;
      }
      if(type == "delete_train") {
        DeleteTrain(command);
        cout << stamp << "0\n";
        continue;
      }
      if(type == "release_train") {
        ReleaseTrain(command);
        cout << stamp << "0\n";
        continue;
      }
    } catch (SevenStream::exception &e) {
      cout << stamp << " -1\n";
    }
  }
}
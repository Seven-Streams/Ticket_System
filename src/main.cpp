#include "../include/exception.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
#include "../include/parser.hpp"
#include "../include/account.hpp"
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
  if(total) {
    has_account = true;
  } else {
    has_account = false;
  }
  while(!cin.eof()) {
    try{
      cin >> command;
      string stamp;
      stamp = ProcessTxt(command);
      string type;
      type = ProcessTxt(command);
      if(type == "add_user") {
        if(has_account) {
          AddAccount(command);
        } else {
          AddFirstAccount(command);
          has_account = true;
        }
        cout << 0 << '\n';
        continue;
      }
    }catch(SevenStream::exception& e) {
      // cout << e.what() << '\n';
    }
  }
}
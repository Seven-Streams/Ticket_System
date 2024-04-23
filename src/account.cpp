#include "../include/account.hpp"
#include "../include/valid.hpp"
#include "../include/parser.hpp"
#include <cstring>
using std::string;
class Account {
private:
  char username[21];
  char password[31];
  char name[21];
  char mail[31];
  int privilege;
  void ModifyPassword(char* _password) {
    strcpy(password, _password);
    return;
  }
  void ModifyName(char *_name) {
    strcpy(name, _name);
    return;
  }
  void ModifyMail(char *_mail) {
    strcpy(mail, _mail);
    return;
  }
  void ModifyPrivilege(int pr) {
    privilege = pr;
    return;
  }
public:
  Account() = default;
  Account(char* _user, char* _password, char *_name, char *_mail,
          int _privilege) {
    strcpy(username, _user);
    strcpy(password, _password);
    strcpy(name, _name);
    strcpy(mail, _mail);
    privilege = _privilege;
  }
  ~Account() = default;
};
sjtu::BPT<int, 70, 20> account_index("account_index");
sjtu::MemoryRiver<Account, 1> account_content("account_content");
Account GetAccount(int pos) {
  Account res;
  account_content.read(res, pos);
  return res;
}
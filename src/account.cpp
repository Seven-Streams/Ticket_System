#include "../include/account.hpp"

using std::string;
class HashOfAccount {
private:
  unsigned long long hash1, hash2;

public:
  HashOfAccount() = delete;
  ~HashOfAccount() = default;
  HashOfAccount(string name) {
    hash1 = sjtu::MyHash(name, exp1);
    hash2 = sjtu::MyHash(name, exp2);
  }
  bool operator==(const HashOfAccount &other) {
    return (hash1 == other.hash1) && (hash2 == other.hash2);
  }
};
class Account {
private:
  char username[21];
  char password[31];
  char name[21];
  char mail[31];
  int privilege;
  void ModifyPassword(char *_password) {
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
  Account(const char *_user, const char *_password, const char *_name,
          const char *_mail, int _privilege) {
    strcpy(username, _user);
    strcpy(password, _password);
    strcpy(name, _name);
    strcpy(mail, _mail);
    privilege = _privilege;
  }
  friend void AddAccount(std::string);
  ~Account() = default;
};
sjtu::list<HashOfAccount> account_logged;
sjtu::BPT<int, 70, 20> account_index("account_index");
sjtu::MemoryRiver<Account, 1> account_content("account_content");
Account GetAccount(int pos) {
  Account res;
  account_content.read(res, pos);
  return res;
}
void AddAccount(std::string command) {
  string current_user = ProcessTxt(command);
  string user_name = ProcessTxt(command);
  string password = ProcessTxt(command);
  string name = ProcessTxt(command);
  string mail = ProcessTxt(command);
  string pri_row = ProcessTxt(command);
  CheckUsername(current_user.c_str());
  CheckUsername(user_name.c_str());
  CheckPassword(password.c_str());
  Checkname(name.c_str());
  CheckMail(mail.c_str());
  CheckPrivilege(pri_row.c_str());
  int pr = std::stoi(pri_row);
  // finish the first part of check.
  HashOfAccount to_check(current_user);
  bool flag = false;
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if (*it == to_check) {
      flag = true;
      break;
    }
  }
  if (!flag) {
    throw(SevenStream::exception("Not login."));
  }
  unsigned long long hash1_of_new, hash2_of_new;
  hash1_of_new = sjtu::MyHash(user_name, exp1);
  hash2_of_new = sjtu::MyHash(user_name, exp2);
  if (account_index.find(hash1_of_new, hash2_of_new) != -1) {
    throw(SevenStream::exception("The account has been created."));
  }
  unsigned long long hash1_of_current, hash2_of_current;
  hash1_of_current = sjtu::MyHash(current_user, exp1);
  hash2_of_current = sjtu::MyHash(current_user, exp2);
  int index = account_index.find(hash1_of_current, hash2_of_current);
  Account current_account = GetAccount(index);
  if (current_account.privilege <= pr) {
    throw(SevenStream::exception("Privilege is not available."));
  }
  Account to_add(user_name.c_str(), password.c_str(), name.c_str(),
                 mail.c_str(), pr);
  int total;
  account_content.get_info(total, 1);
  total++;
  account_content.write(to_add, total);
  account_content.write_info(total, 1);
  account_index.Insert(hash1_of_new, hash2_of_new, total);
  return;
}
void AddFirstAccount(std::string command) {
  string user_name = ProcessTxt(command);
  string password = ProcessTxt(command);
  string name = ProcessTxt(command);
  string mail = ProcessTxt(command);
  CheckUsername(user_name.c_str());
  CheckPassword(password.c_str());
  Checkname(name.c_str());
  CheckMail(mail.c_str());
  Account to_add(user_name.c_str(), password.c_str(), name.c_str(), mail.c_str(), 10);
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user_name, exp1);
  hash2 = sjtu::MyHash(user_name, exp2);
  account_content.write(to_add, 1);
  int total = 1;
  account_content.write_info(total, 1);
  account_index.Insert(hash1, hash2, 1);
  return;
}
void Logout(std::string user) {
  CheckUsername(user.c_str());
  HashOfAccount to_remove(user);
  for(auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if(*it == to_remove) {
      account_logged.erase(it);
      return;
    }
  }
  throw(SevenStream::exception("This account doesn't login."));
  return; 
}
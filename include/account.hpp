#include "BPT.hpp"
#include "map.hpp"
#include "memoryriver.hpp"
#include "utility.hpp"
#include <cstring>
#include <string>
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
using std::string;
class AccountIndex {
private:
  int index = 0;
  int privilege = 0;

public:
  AccountIndex() = default;
  ~AccountIndex() = default;
  bool operator<(const AccountIndex &other) const;
  bool operator>(const AccountIndex &other) const;
  bool operator==(const AccountIndex &other) const;
  friend void AddAccount(string &);
  friend void AddFirstAccount(string &);
  friend void Login(string &);
  friend void Buy(std::string &, const int);
  friend void Refund(std::string &);
  friend string QueryAccount(string &);
  friend string ModifyAccount(string &);
};
class HashOfAccount {
private:
  unsigned long long hash1, hash2;

public:
  HashOfAccount() = delete;
  ~HashOfAccount() = default;
  HashOfAccount(string);
  bool operator==(const HashOfAccount &) const;
  bool operator<(const HashOfAccount &) const;
  bool operator>(const HashOfAccount &) const;
};
class Account {
private:
  char username[21] = "";
  char password[31] = "";
  char name[21] = "";
  char mail[31] = "";
  int privilege = 0;
  void ModifyPassword(const char *);
  void ModifyName(const char *);
  void ModifyMail(const char *);
  void ModifyPrivilege(const int);

public:
  Account() = default;
  Account(const char *_u, const char *_p, const char *_n, const char *_m, const int pr);
  ~Account() = default;
  friend void AddAccount(string &);
  friend void Login(string &);
  friend string QueryAccount(string &);
  friend string ModifyAccount(string &);
};
void AddAccount(string &);//输入合法命令，即可以创建账号。
void Login(string &);//输入合法命令进行登录。
void Logout(string &);//输入合法命令进行登出。
std::string QueryAccount(string &);//查询账号。如果权限合法，会返回一个带有查询对象的字符串。
std::string ModifyAccount(string &);//修改账号。如果权限合法，会返回一个带有修改对象的字符串。
void AddFirstAccount(string &);//如果没有账户，会创建第一个账户。
#endif
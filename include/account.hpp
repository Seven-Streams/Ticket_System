#include "BPT.hpp"
#include "map.hpp"
#include "memoryriver.hpp"
#include "utility.hpp"
#include <cstring>
#include <string>
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
using std::string;
struct AccountIndex {
  int index = 0;
  int privilege = 0;
  bool operator<(const AccountIndex &other) const;
  bool operator>(const AccountIndex &other) const;
  bool operator==(const AccountIndex &other) const;
};
class HashOfAccount {
private:
  unsigned long long hash1, hash2;

public:
  HashOfAccount() = delete;
  ~HashOfAccount() = default;
  HashOfAccount(string);
  bool operator==(const HashOfAccount &);
  bool operator<(const HashOfAccount &);
  bool operator>(const HashOfAccount &);
};
class Account {
private:
  char username[21] = "";
  char password[31] = "";
  char name[21] = "";
  char mail[31] = "";
  int privilege = 0;
  void ModifyPassword(char *);
  void ModifyName(char *);
  void ModifyMail(char *);
  void ModifyPrivilege(int);

public:
  Account() = default;
  Account(const char *, const char *, const char *, const char *, int);
  ~Account() = default;
  friend void AddAccount(string);
  friend void Login(string);
  friend string QueryAccount(string);
  friend string ModifyAccount(string);
};
void AddAccount(std::string);
void Login(std::string);
void Logout(std::string);
std::string QueryAccount(std::string);
std::string ModifyAccount(std::string);
void AddFirstAccount(std::string);
#endif
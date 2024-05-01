#include "exception.hpp"
#include "store.hpp"
#include "utility.hpp"
#include "valid.hpp"
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
using std::string;
class HashOfAccount {
private:
  unsigned long long hash1, hash2;

public:
  HashOfAccount() = delete;
  ~HashOfAccount() = default;
  HashOfAccount(string);
  bool operator==(const HashOfAccount &);
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
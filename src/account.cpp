#include "../include/account.hpp"
#include <cstring>
#include <string>

using std::string;
sjtu::list<HashOfAccount> account_logged;
sjtu::BPT<int, 70, 20> account_index("account_index");
sjtu::MemoryRiver<Account, 1> account_content("account_content");
HashOfAccount::HashOfAccount(string name) {
  hash1 = sjtu::MyHash(name, exp1);
  hash2 = sjtu::MyHash(name, exp2);
}
bool HashOfAccount::operator==(const HashOfAccount &other) {
  return (hash1 == other.hash1) && (hash2 == other.hash2);
}

void Account::ModifyPassword(char *_password) {
  strcpy(password, _password);
  return;
}
void Account::ModifyName(char *_name) {
  strcpy(name, _name);
  return;
}
void Account::ModifyMail(char *_mail) {
  strcpy(mail, _mail);
  return;
}
void Account::ModifyPrivilege(int pr) {
  privilege = pr;
  return;
}

Account::Account(const char *_user, const char *_password, const char *_name,
                 const char *_mail, int _privilege) {
  strcpy(username, _user);
  strcpy(password, _password);
  strcpy(name, _name);
  strcpy(mail, _mail);
  privilege = _privilege;
}
Account GetAccount(int pos) {
  Account res;
  account_content.read(res, pos);
  return res;
}
void AddAccount(string command) {
  string current_user;
  string user_name;
  string password;
  string name;
  string mail;
  string pri_row;
  bool c, u, p, n, m, g;
  c = u = p = n = m = g = false;
  while (command != "") {
    string res = ProcessTxt(command);
    if (res[0] != '-') {
      throw(SevenStream::exception("Incorrect input."));
    }
    switch (res[1]) {
    case 'c': {
      if (c == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      c = true;
      current_user = ProcessTxt(command);
      break;
    }
    case 'u': {
      if (u == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      u = true;
      user_name = ProcessTxt(command);
      break;
    }
    case 'p': {
      if (p == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      p = true;
      password = ProcessTxt(command);
      break;
    }
    case 'n': {
      if (n == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      n = true;
      name = ProcessTxt(command);
      break;
    }
    case 'm': {
      if (m == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      m = true;
      mail = ProcessTxt(command);
      break;
    }
    case 'g': {
      if (g == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      g = true;
      pri_row = ProcessTxt(command);
      break;
    }
    default: {
      throw(SevenStream::exception("Incorrect input."));
    }
    }
  }
  if ((!c) || (!u) || (!p) || (!n) || (!m) || (!g)) {
    throw(SevenStream::exception("Incorrect input."));
  }
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
  if (account_index.find(hash1_of_new, hash2_of_new) != "") {
    throw(SevenStream::exception("The account has been created."));
  }
  unsigned long long hash1_of_current, hash2_of_current;
  hash1_of_current = sjtu::MyHash(current_user, exp1);
  hash2_of_current = sjtu::MyHash(current_user, exp2);
  int index = std::stoi(account_index.find(hash1_of_current, hash2_of_current));
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
void AddFirstAccount(string command) {
  string user_name;
  string password;
  string name;
  string mail;
  bool u, p, n, m;
  u = p = n = m = false;
  while (command != "") {
    string res = ProcessTxt(command);
    if (res[0] != '-') {
      throw(SevenStream::exception("Incorrect input."));
    }
    switch (res[1]) {
    case 'u': {
      if (u == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      u = true;
      user_name = ProcessTxt(command);
      break;
    }
    case 'p': {
      if (p == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      p = true;
      password = ProcessTxt(command);
      break;
    }
    case 'n': {
      if (n == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      n = true;
      name = ProcessTxt(command);
      break;
    }
    case 'm': {
      if (m == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      m = true;
      mail = ProcessTxt(command);
      break;
    }
    default: {
      throw(SevenStream::exception("Incorrect input."));
    }
    }
  }
  if ((!u) || (!p) || (!n) || (!m)) {
    throw(SevenStream::exception("Incorrect input."));
  }
  CheckUsername(user_name.c_str());
  CheckPassword(password.c_str());
  Checkname(name.c_str());
  CheckMail(mail.c_str());
  Account to_add(user_name.c_str(), password.c_str(), name.c_str(),
                 mail.c_str(), 10);
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user_name, exp1);
  hash2 = sjtu::MyHash(user_name, exp2);
  account_content.write(to_add, 1);
  int total = 1;
  account_content.write_info(total, 1);
  account_index.Insert(hash1, hash2, 1);
  return;
}
void Logout(string command) {
  string key = ProcessTxt(command);
  if (key != "-u") {
    throw(SevenStream::exception("Incorrect input."));
  }
  string user = ProcessTxt(command);
  CheckUsername(user.c_str());
  HashOfAccount to_remove(user);
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if (*it == to_remove) {
      account_logged.erase(it);
      return;
    }
  }
  throw(SevenStream::exception("This account doesn't login."));
  return;
}
void Login(string command) {
  string user;
  string password;
  bool u, p;
  u = p = false;
  while (command != "") {
    string res = ProcessTxt(command);
    if (res[0] != '-') {
      throw(SevenStream::exception("Incorrect input."));
    }
    switch (res[1]) {
    case 'u': {
      if (u == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      u = true;
      user = ProcessTxt(command);
      break;
    }
    case 'p': {
      if (p == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      p = true;
      password = ProcessTxt(command);
      break;
    }
    default: {
      throw(SevenStream::exception("Incorrect input."));
    }
    }
  }
  if ((!u) || (!p)) {
    throw(SevenStream::exception("Incorrect input."));
  }
  CheckUsername(user.c_str());
  CheckPassword(password.c_str());
  HashOfAccount hash_of_login(user);
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if ((*it) == hash_of_login) {
      throw(SevenStream::exception("The account has logged in."));
    }
  }
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user, exp1);
  hash2 = sjtu::MyHash(user, exp2);
  string index_raw = account_index.find(hash1, hash2);
  if (index_raw == "") {
    throw(SevenStream::exception("This account doesn't exist."));
  }
  int index = std::stoi(index_raw);
  Account to_login = GetAccount(index);
  if (to_login.password != password) {
    throw(SevenStream::exception("Wrong Password."));
  }
  account_logged.push_back(hash_of_login);
  return;
}
string QueryAccount(string command) {
  string current;
  string to_query;
  bool c, u;
  c = u = false;
  while (command != "") {
    string res = ProcessTxt(command);
    if (res[0] != '-') {
      throw(SevenStream::exception("Incorrect input."));
    }
    switch (res[1]) {
    case 'c': {
      if (c == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      c = true;
      current = ProcessTxt(command);
      break;
    }
    case 'u': {
      if (u == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      u = true;
      to_query = ProcessTxt(command);
      break;
    }
    default: {
      throw(SevenStream::exception("Incorrect input."));
    }
    }
  }
  if ((!c) || (!u)) {
    throw(SevenStream::exception("Incorrect input."));
  }
  CheckUsername(current.c_str());
  CheckUsername(to_query.c_str());
  HashOfAccount hash_current(current);
  bool logged = false;
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if ((*it) == hash_current) {
      logged = true;
      break;
    }
  }
  if (!logged) {
    throw(SevenStream::exception("Not login."));
  }
  unsigned long long hash1_current, hash2_current;
  hash1_current = sjtu::MyHash(current, exp1);
  hash2_current = sjtu::MyHash(current, exp2);
  int current_index = std::stoi(account_index.find(hash1_current, hash2_current));
  Account current_account = GetAccount(current_index);
  unsigned long long hash1_query, hash2_query;
  hash1_query = sjtu::MyHash(to_query, exp1);
  hash2_query = sjtu::MyHash(to_query, exp2);
  string query_raw = account_index.find(hash1_query, hash2_query);
  if(query_raw != "") {
    throw(SevenStream::exception("The query account doesn't exist."));
  }
  int query_index = std::stoi(query_raw);
  Account query_account = GetAccount(query_index);
  if (query_account.privilege >= current_account.privilege) {
    throw(SevenStream::exception("Not enough privilege."));
  }
  string query_ans;
  query_ans += query_account.username;
  query_ans += ' ';
  query_ans += query_account.name;
  query_ans += ' ';
  query_ans += query_account.mail;
  query_ans += ' ';
  query_ans += std::to_string(query_account.privilege);
  return query_ans;
}
string ModifyAccount(string command) {
  string current;
  string to_query;
  string password;
  string name;
  string mail;
  string pri_row;
  bool c, u, p, n, m, g;
  c = u = p = n = m = g = false;
  while (command != "") {
    string res = ProcessTxt(command);
    if (res[0] != '-') {
      throw(SevenStream::exception("Incorrect input."));
    }
    switch (res[1]) {
    case 'c': {
      if (c == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      c = true;
      current = ProcessTxt(command);
      break;
    }
    case 'u': {
      if (u == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      u = true;
      to_query = ProcessTxt(command);
      break;
    }
    case 'p': {
      if (p == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      p = true;
      password = ProcessTxt(command);
      break;
    }
    case 'n': {
      if (n == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      n = true;
      name = ProcessTxt(command);
      break;
    }
    case 'm': {
      if (m == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      m = true;
      mail = ProcessTxt(command);
      break;
    }
    case 'g': {
      if (g == true) {
        throw(SevenStream::exception("Incorrect input."));
      }
      g = true;
      pri_row = ProcessTxt(command);
      break;
    }
    default: {
      throw(SevenStream::exception("Incorrect input."));
    }
    }
  }
  CheckUsername(current.c_str());
  CheckUsername(to_query.c_str());
  if ((!c) || (!u)) {
    throw(SevenStream::exception("Incorrect input."));
  }
  HashOfAccount hash_current(current);
  bool logged = false;
  for (auto it = account_logged.begin(); it != account_logged.end(); it++) {
    if ((*it) == hash_current) {
      logged = true;
      break;
    }
  }
  if (!logged) {
    throw(SevenStream::exception("Not login."));
  }
  unsigned long long hash1_current, hash2_current;
  hash1_current = sjtu::MyHash(current, exp1);
  hash2_current = sjtu::MyHash(current, exp2);
  int current_index = std::stoi(account_index.find(hash1_current, hash2_current));
  Account current_account = GetAccount(current_index);
  unsigned long long hash1_query, hash2_query;
  hash1_query = sjtu::MyHash(to_query, exp1);
  hash2_query = sjtu::MyHash(to_query, exp2);
  string query_raw = account_index.find(hash1_query, hash2_query);
  if(query_raw == "") {
    throw("The query account doesn't exist.");
  }
  int query_index = std::stoi(query_raw);
  Account query_account = GetAccount(query_index);
  if (query_account.privilege >= current_account.privilege) {
    throw(SevenStream::exception("Not enough privilege."));
  }
  if (password != "") {
    CheckPassword(password.c_str());
    strcpy(query_account.password, password.c_str());
  }
  if (name != "") {
    Checkname(name.c_str());
    strcpy(query_account.name, name.c_str());
  }
  if (mail != "") {
    CheckMail(mail.c_str());
    strcpy(query_account.mail, mail.c_str());
  }
  if (pri_row != "") {
    CheckPrivilege(pri_row.c_str());
    int pr = std::stoi(pri_row);
    query_account.privilege = pr;
  }
  account_content.write(query_account, query_index);
  string query_ans;
  query_ans += query_account.username;
  query_ans += ' ';
  query_ans += query_account.name;
  query_ans += ' ';
  query_ans += query_account.mail;
  query_ans += ' ';
  query_ans += std::to_string(query_account.privilege);
  return query_ans;
}
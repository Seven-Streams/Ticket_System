#include "../include/account.hpp"

using std::string;
sjtu::map<HashOfAccount, int, sjtu::Less<HashOfAccount>> account_logged;
sjtu::BPT<AccountIndex, 126, 4> account_index("account_index");
sjtu::MemoryRiver<Account, 1> account_content("account_content");
HashOfAccount::HashOfAccount(string name) {
  hash1 = sjtu::MyHash(name, exp1);
  hash2 = sjtu::MyHash(name, exp2);
}
bool HashOfAccount::operator==(const HashOfAccount &other) const{
  return (hash1 == other.hash1) && (hash2 == other.hash2);
}

void Account::ModifyPassword(const char *_password) {
  strcpy(password, _password);
  return;
}
void Account::ModifyName(const char *_name) {
  strcpy(name, _name);
  return;
}
void Account::ModifyMail(const char *_mail) {
  strcpy(mail, _mail);
  return;
}
void Account::ModifyPrivilege(const int pr) {
  privilege = pr;
  return;
}

Account::Account(const char *_user, const char *_password, const char *_name,
                 const char *_mail, const int _privilege) {
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
void AddAccount(string& command) {
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
  int pr = std::stoi(pri_row);
  // finish the first part of check.
  HashOfAccount to_check(current_user);
  if (!account_logged.count(to_check)) {
    throw(SevenStream::exception("Not login."));
  }
  unsigned long long hash1_of_new, hash2_of_new;
  hash1_of_new = sjtu::MyHash(user_name, exp1);
  hash2_of_new = sjtu::MyHash(user_name, exp2);
  AccountIndex mini;
  mini.index = minus_max;
  auto check = account_index.find(hash1_of_new, hash2_of_new, mini);
  if (!account_index.find(hash1_of_new, hash2_of_new, mini).empty()) {
    throw(SevenStream::exception("The account has been created."));
  }
  unsigned long long hash1_of_current, hash2_of_current;
  int current_privilege = account_logged[to_check];
  if (current_privilege <= pr) {
    throw(SevenStream::exception("Privilege is not available."));
  }
  Account to_add(user_name.c_str(), password.c_str(), name.c_str(),
                 mail.c_str(), pr);
  int total;
  account_content.get_info(total, 1);
  total++;
  account_content.write(to_add, total);
  account_content.write_info(total, 1);
  AccountIndex res;
  res.index = total;
  res.privilege = pr;
  account_index.Insert(hash1_of_new, hash2_of_new, res);
  return;
}
void AddFirstAccount(string& command) {
  string user_name;
  string password;
  string name;
  string mail;
  bool u, p, n, m;
  u = p = n = m = false;
  while (command != "") {
    string res = ProcessTxt(command);
    if (res[0] != '-') {
      throw(SevenStream::exception("Incorrect input1."));
    }
    switch (res[1]) {
    case 'c': {
      ProcessTxt(command);
      break;
    }
    case 'g': {
      ProcessTxt(command);
      break;
    }
    case 'u': {
      if (u == true) {
        throw(SevenStream::exception("Incorrect input2."));
      }
      u = true;
      user_name = ProcessTxt(command);
      break;
    }
    case 'p': {
      if (p == true) {
        throw(SevenStream::exception("Incorrect input3."));
      }
      p = true;
      password = ProcessTxt(command);
      break;
    }
    case 'n': {
      if (n == true) {
        throw(SevenStream::exception("Incorrect input4."));
      }
      n = true;
      name = ProcessTxt(command);
      break;
    }
    case 'm': {
      if (m == true) {
        throw(SevenStream::exception("Incorrect input5."));
      }
      m = true;
      mail = ProcessTxt(command);
      break;
    }
    default: {
      throw(SevenStream::exception("Incorrect input6."));
    }
    }
  }
  if ((!u) || (!p) || (!n) || (!m)) {
    throw(SevenStream::exception("Incorrect input7."));
  }
  Account to_add(user_name.c_str(), password.c_str(), name.c_str(),
                 mail.c_str(), 10);
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user_name, exp1);
  hash2 = sjtu::MyHash(user_name, exp2);
  account_content.write(to_add, 1);
  int total = 1;
  account_content.write_info(total, 1);
  AccountIndex res;
  res.index = 1;
  res.privilege = 10;
  account_index.Insert(hash1, hash2, res);
  return;
}
void Logout(string& command) {
  string key = ProcessTxt(command);
  if (key != "-u") {
    throw(SevenStream::exception("Incorrect input."));
  }
  string user = ProcessTxt(command);
  HashOfAccount to_remove(user);
  auto res = account_logged.find(to_remove);
  if (res != account_logged.end()) {
    account_logged.erase(res);
  } else {
    throw(SevenStream::exception("This account doesn't login."));
  }
  return;
}
void Login(string& command) {
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
  HashOfAccount hash_of_login(user);

  if (account_logged.count(hash_of_login)) {
    throw(SevenStream::exception("The account has logged in."));
  }
  unsigned long long hash1, hash2;
  hash1 = sjtu::MyHash(user, exp1);
  hash2 = sjtu::MyHash(user, exp2);
  AccountIndex mini;
  mini.index = minus_max;
  auto index_raw = account_index.find(hash1, hash2, mini);
  if (index_raw.empty()) {
    throw(SevenStream::exception("This account doesn't exist."));
  }
  auto index = index_raw.front();
  Account to_login = GetAccount(index.index);
  if (to_login.password != password) {
    throw(SevenStream::exception("Wrong Password."));
  }
  account_logged[hash_of_login] = to_login.privilege;
  return;
}
string QueryAccount(string& command) {
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
  HashOfAccount hash_current(current);
  bool logged = false;
  if (!account_logged.count(hash_current)) {
    throw(SevenStream::exception("Not login."));
  }
  unsigned long long hash1_current, hash2_current;
  hash1_current = sjtu::MyHash(current, exp1);
  hash2_current = sjtu::MyHash(current, exp2);
  AccountIndex mini;
  mini.index = minus_max;
  int current_privilege = account_logged[hash_current];
  unsigned long long hash1_query, hash2_query;
  hash1_query = sjtu::MyHash(to_query, exp1);
  hash2_query = sjtu::MyHash(to_query, exp2);
  auto query_raw = account_index.find(hash1_query, hash2_query, mini);
  if (query_raw.empty()) {
    throw(SevenStream::exception("The query account doesn't exist."));
  }
  auto query_index = query_raw.front();
  if ((current != to_query) &&
      (query_index.privilege >= current_privilege)) {
    throw(SevenStream::exception("Not enough privilege!"));
  }
  Account query_account = GetAccount(query_index.index);
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
string ModifyAccount(string& command) {
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
  if ((!c) || (!u)) {
    throw(SevenStream::exception("Incorrect input."));
  }
  HashOfAccount hash_current(current);
  if (!account_logged.count(hash_current)) {
    throw(SevenStream::exception("Not login."));
  }
  unsigned long long hash1_current, hash2_current;
  hash1_current = sjtu::MyHash(current, exp1);
  hash2_current = sjtu::MyHash(current, exp2);
  AccountIndex mini;
  mini.index = minus_max;
  int current_privilege = account_logged[hash_current];
  if (pri_row != "") {
    int pr = std::stoi(pri_row);
    if (pr >= current_privilege) {
      throw(SevenStream::exception("Not Enough privilege."));
    }
  }
  unsigned long long hash1_query, hash2_query;
  hash1_query = sjtu::MyHash(to_query, exp1);
  hash2_query = sjtu::MyHash(to_query, exp2);
  auto query_raw = account_index.find(hash1_query, hash2_query, mini);
  if (query_raw.empty()) {
    throw(SevenStream::exception("The query account doesn't exist."));
  }
  auto query_index = query_raw.front();
  if ((current != to_query) &&
      (query_index.privilege >= current_privilege)) {
    throw(SevenStream::exception("Not enough privilege!"));
  }
  Account query_account = GetAccount(query_index.index);
  if (password != "") {
    strcpy(query_account.password, password.c_str());
  }
  if (name != "") {
    strcpy(query_account.name, name.c_str());
  }
  if (mail != "") {
    strcpy(query_account.mail, mail.c_str());
  }
  if (pri_row != "") {
    int pr = std::stoi(pri_row);
    query_account.privilege = pr;
    query_index.privilege = pr;
    account_index.Insert(hash1_query, hash2_query, query_index);
  }
  account_content.write(query_account, query_index.index);
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
bool HashOfAccount::operator<(const HashOfAccount &rhs) const{
  if (hash1 != rhs.hash1) {
    return (hash1 < rhs.hash1);
  }
  return (hash2 < rhs.hash2);
}
bool HashOfAccount::operator>(const HashOfAccount &rhs) const{
  if (hash1 != rhs.hash1) {
    return (hash1 > rhs.hash1);
  }
  return (hash2 > rhs.hash2);
}
bool AccountIndex::operator<(const AccountIndex &other) const {
  return index < other.index;
}
bool AccountIndex::operator>(const AccountIndex &other) const {
  return index > other.index;
}
bool AccountIndex::operator==(const AccountIndex &other) const {
  return index == other.index;
}
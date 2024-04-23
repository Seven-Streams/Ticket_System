#include "store.hpp"
#include "valid.hpp"
#include "parser.hpp"
#include "exception.hpp"
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
void AddAccount(std::string);
void Login(std::string);
void Logout(std::string);
std::string QueryAccount(std::string);
std::string ModifyAccount(std::string);
void AddFirstAccount(std::string);
#endif
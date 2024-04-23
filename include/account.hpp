#include "store.hpp"
#include "valid.hpp"
#include "parser.hpp"
#include "exception.hpp"
#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
void AddAccount(std::string);
void Login(std::string);
void LogOut(std::string);
void QueryAccount(std::string);
void ModifyAccount(std::string);
#endif
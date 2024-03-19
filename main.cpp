#include "exception.hpp"
#include "store.hpp"
#include "valid.hpp"
#include "parser.hpp"
#include <iostream>
using std::cin;
using std::cout;
using std::string;
int main() {
  string command;
  while(!cin.eof()) {
    try{
      cin >> command;
      string type;
      type = ProcessTxt(command);
      
    }catch(SevenStream::exception& e) {
      // cout << e.what() << '\n';
    }
  }
}
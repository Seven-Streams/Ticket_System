#include "../include/exception.hpp"
#include "../include/store.hpp"
#include "../include/valid.hpp"
#include "../include/parser.hpp"
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
      if(type == "") {
        
      }
    }catch(SevenStream::exception& e) {
      // cout << e.what() << '\n';
    }
  }
}
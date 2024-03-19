#include "valid.hpp"
#include "exception.hpp"
void CheckUsername(char * txt) {
  int len = strlen(txt);
  if(len > 20 || (!len)) {
    throw(SevenStream::InvalidInput());
  }
  if((txt[0] < 'a') || txt[0] > 'z') {
    if((txt[0] < 'A') || txt[0] > 'Z') {
      throw(SevenStream::InvalidInput());
    }
  }
  for (int i = 0; i < len; ++i) {
    if (txt[i] < '0' || txt[i] > '9') {
      if (txt[i] < 'a' || txt[i] > 'z') {
        if (txt[i] < 'A' || txt[i] > 'Z') {
          if (txt[i] != '_') {
            throw(SevenStream::InvalidInput());
          }
        }
      }
    }
  }
  return;
}
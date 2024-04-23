#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <string>
namespace SevenStream {
class exception {
  std::string type;

public:
  exception(std::string txt) { type = txt; }
  std::string what() { return type; }
};
} // namespace SevenStream
#endif
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
class InvalidUsername : public exception {
public:
  InvalidUsername() : exception("Invalid username."){};
};
class InvalidPassword : public exception {
public:
  InvalidPassword() : exception("Invalid password."){};
};
class InvalidName : public exception {
public:
  InvalidName() : exception("Invalid name."){};
};
class InvalidMail : public exception {
public:
  InvalidMail() : exception("Invalid Mail."){};
};
class InvalidPrivilege : public exception {
public:
  InvalidPrivilege() : exception("Invalid Privilege."){};
};
class InvalidTrainID : public exception {
public:
  InvalidTrainID() : exception("Invalid trainID."){};
};
class InvalidStaNum : public exception{
  public:
  InvalidStaNum():exception("Invalid station number."){};
};
class InvalidStationName : public exception{
  public:
  InvalidStationName():exception("Invalid station name."){};
};
class InvalidPrice :public exception{
  public:
InvalidPrice():exception("Invalid price."){};
};
class InvalidTime :public exception{
  public:
  InvalidTime():exception("Invalid time."){};
}
} // namespace SevenStream
#endif
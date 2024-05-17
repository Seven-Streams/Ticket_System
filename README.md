# Seven Streams' Ticket System🚋
这里是玉川的数据结构大作业。

本次作业要求实现一个类似于 [12306](https://www.12306.cn/) 的火车票订票系统，该系统向用户提供购票业务相关功能，包括车票查询、购票、订单操作等，以及向管理员提供后台管理功能。

如果大家看得起我写的这一坨，~并且从我这里学到什么，那是我的荣幸。😀~(学不到什么，慢得一坨）

~(如果能够点个star就更好了XD)😝~

但还是请不要**直接抄**我的🐴。
## 代码文件结构
- 📁`src/`
  - 📄`main.cpp`
  - 📄`account.cpp`
  - 📄`order.cpp`
  - 📄`train.cpp`
  - 📄`utility.cpp`
- 📁`include/`
  - 📄`account.hpp`
  - 📄`BPT.hpp`
  - 📄`list.hpp`
  - 📄`map.hpp`
  - 📄`memoryriver.hpp`
  - 📄`order.hpp`
  - 📄`priority_queue.hpp`
  - 📄`train.hpp`
  - 📄`utility.hpp`
  - 📄`vector.hpp`
- 📄`README.md`
- 📄`.gitignore`
- 📄`CMakeLists.txt`

## 代码文件概述

在BPT.hpp, list.hpp, map.hpp, priority_queue.hpp, vector.hpp中，分别提供了一个B+树类，链表类，映射类，优先队列类，线性表类。

其中，list, map, priority_queue, vector的行为与Cpp标准库相近。不保证迭代器的行为与C++一致。

BPT类要求以两个ULL哈希值为输入。支持缓存。缓存由list和map组合而成。BPT允许可写入文件的并支持大于，小于，等于运算符的类型。可以调节每个块中的元素个数，可以调节块中冗余量大小。BPT类支持文件回收。BPT支持插入和删除，支持按键值查找大于某个值的所有元素，支持按键值查找大于某个值，并小于某个值的元素。支持一键清空。查找的返回对象是一个list.

在memoryriver.hpp中，提供了一个与外存交互的工具。可以存储可写入数据，并且可存储参数。

account.hpp中，提供了与账号服务的相关功能。

utility.hpp中，提供了文本处理功能，exception，pair，和一个时间类。

train.hpp中，提供了对火车、火车票务的查询和维护功能。

order.hpp中，提供了购票、退票、订单查询功能。

## 存储文件结构

account_index:记录了账号的存储索引和权限。

account_content:对账号的全部内容进行存储。

order_user:对用户下达的所有订单进行存储。

queue:对候补订单进行存储。

train_index:记录了车次的存储索引。

train_database:记录了车辆的所有信息。

station:记录了经过某站点的所有列车。

train_day_index:记录了经过某站点的不同发车时间的所有列车的存储索引。

train_day_info:存储了该车次仍旧具有的余票量。

## 类和函数说明

### Account.hpp

```cpp
void AddAccount(string &);//输入合法命令，即可以创建账号。
void Login(string &);//输入合法命令进行登录。
void Logout(string &);//输入合法命令进行登出。
std::string QueryAccount(string &);//查询账号。如果权限合法，会返回一个带有查询对象的字符串。
std::string ModifyAccount(string &);//修改账号。如果权限合法，会返回一个带有修改对象的字符串。
void AddFirstAccount(string &);//如果没有账户，会创建第一个账户。

class AccountIndex;//账号的存储索引和权限。
class HashOfAccount;//账号的hash值。
class Account;//账号的所有信息。
```

### BPT.hpp

```cpp
  template <class Value = int, int size = 168, int redundancy = 6, int cachesize = 400>
  //size指示块的大小上限。redundancy用于指示冗余量，从而保证块大小。cachesize用于指定缓存上限。
class BPT;//B+树存储类
BPT(std::string);//构造函数，指定文件名。
//在下面四个函数中，第一个和参数和第二个参数表示两个有顺序的hash值。
//这四个函数都是BPT类的成员函数。
void insert(unsigned long long, unsigned long long, Value);//插入
void erase(unsigned long long, unsigned long long, Value);//删除
sjtu::list<Value> find(unsigned long long, unsigned long long, Value);//查找哈希值相同，并且不小于该Value的所有元素。小的元素在链表头。
sjtu::list<Value> find2(unsigned long long, unsigned long long, Value, Value);//查找哈希值相同，不小于第一个Value，并且小于第二个Value的元素。小的元素在链表头。
```

### memoryriver.hpp

```cpp
template <class W, int info_len = 3> class MemoryRiver;
MemoryRiver(const std::string);//构造函数，指定文件名。
void ChangeName(std::string);//用于更名。
void initialise();//用于执行初始化操作。
  // 读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n);

  // 将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n);

  // 在文件合适位置写入或读入类对象t，并返回写入的位置索引index
  // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  // 位置索引index可以取为对象写入的起始位置,1base，size表示一次性读入多少个W。
  void write(W &t, int which_node, int size = 1);
  void read(W &t, int which_node, int size = 1);
  void clear();//清空所有元素。
```

### order.hpp

```cpp
class OrderByTrain;//存储候补订单信息。
class OrderByUser;//存储用户订单信息。
//用户订单具有一个public函数。
void Print() const;//这会按照要求打印订单信息。
void QueryOrder(std::string &);//查询订单情况并打印。
void Buy(std::string &, const int);//购票。第二个参数是时间戳。
void Refund(std::string &);//退票
```

### train.hpp

```cpp
class TrainDayIndex;//记录了每车次每天的票务情况的存储索引。
class AskData;//记录某日车次待查询情况。
class TransferData;//记录换乘方案。
class TrainInfo;//记录车次的所有信息。
//TrainInfo类具有较多的成员函数。下列函数均是。
bool IsReleased() const;//是否已发布。
int FindIndex(const char *) const;//查找站名对应的index。
int AskPrice(const int &, const int &) const;//询问两个站点之间的费用。
Time AskOutTime(const int &index, const int &month, const int &day) const;
//询问某日到达某站的列车的发车时间。
bool IsSaleTime(const int &, const int &) const;//询问是否在发售时间内。
Time AskLeaveTime(const int &index, const int &, const int &) const;
//询问某日发车的列车何时离开某站。
Time AskArriveTime(const int &index, const int &, const int &) const;
//询问某日到达的列车何时到达某站。
bool CheckAvailable(const Time &) const;
//检查时间时候在发售时间内。
int AskTime(const int &, const int &) const;
//询问两个站点间所要花费的时间。
class TrainDay;//记录某日某班车的票务情况。
//以下是四个排序仿函数。
class SortTrainByCost;
class CompareTransferByCost;
class SortTrainByTime;
class CompareTransferByTime;

void AddTrain(string &);//添加列车。
void ReleaseTrain(string &);//发布列车。
void DeleteTrain(string &);//删除列车。
void QueryTrain(string &);//查询列车。
void QueryTicket(string &);//查询票务。
void QueryTransfer(string &);//查询换乘方案。
```

### utility.hpp
```cpp
class exception;//异常处理类。
exception(std::string);//构造函数。
class Time;//时间类。下列为其成员函数。
void Add(const int &);//在当前基础上加上多少分钟。
void Minus(const int &);//在当前基础上减去多少分钟。
void Print() const;//打印时间。
int GetMonth() const;//返回月份。
int GetDay() const;//返回日期。

int IntervalMinute(Time, Time);//询问两个时间之间间隔的时间数。
std::string ProcessTxt(std::string &);//将字符串在空格处截断。
std::string ProcessMalValue(std::string &);//将字符串在'|'处截断。
template <class T1, class T2> class pair;//引用自ACM_STLite库。模拟pair的行为。
template <class T> class Less;//模拟std::less<T>
inline unsigned long long MyHash(const std::string &,
const unsigned long long &);//文本hash值计算。
```

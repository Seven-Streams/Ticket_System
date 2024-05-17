# Seven Streams' Ticket System

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

## 程序功能要求
本次作业要求实现一个类似于 [12306](https://www.12306.cn/) 的火车票订票系统，该系统向用户提供购票业务相关功能，包括车票查询、购票、订单操作等，以及向管理员提供后台管理功能。

## 代码文件结构
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
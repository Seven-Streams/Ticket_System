#include "utility.hpp"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#ifndef River_HPP
#define River_HPP

namespace sjtu {

template <class W, int info_len = 3> class MemoryRiver { // 应当采取3个参数。
  // 一个存储目前的元素个数，一个存储目前的根节点，一个存储当前的块应该写入到哪里。
private:
  std::fstream file;
  std::string file_name; // 文件名。
  int config[info_len];
  int sizeofT = sizeof(W); // W的大小。

public:
  MemoryRiver() {}

  MemoryRiver(const std::string &file_name) : file_name(file_name) {
    initialise();
    return;
  }
  ~MemoryRiver() {
    file.open(file_name + "_config");
    for (int i = 0; i < info_len; i++) {
      file.seekp(i * sizeof(int), std::fstream::beg);
      file.write(reinterpret_cast<char *>(&config[i]), sizeof(int));
    }
    file.close();
  }
  void ChangeName(std::string res) {
    file_name = res;
    initialise();
  }

  void
  initialise() { // 如果文件不存在，则会进行初始化操作，建立文件并初始化参数。
    std::filesystem::path test(file_name);
    if (std::filesystem::exists(test)) {
      file.open(file_name + "_config");
      for (int i = 0; i < info_len; i++) {
        file.seekg(i * sizeof(int));
        file.read(reinterpret_cast<char *>(&config[i]), sizeof(int));
      }
      file.close();
      return;
    }
    file.open(file_name, std::fstream::out);
    file.close();
    file.open(file_name + "_config", std::fstream::out);
    int tmp = 0;
    for (int i = 0; i < info_len; ++i)
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
    for (int i = 0; i < info_len; i++) {
      config[i] = 0;
    }
  }

  // 读出第n个int的值赋给tmp，1_base
  void get_info(int &tmp, int n) {
    tmp = config[n - 1];
    return;
  }

  // 将tmp写入第n个int的位置，1_base
  void write_info(int tmp, int n) {
    config[n - 1] = tmp;
    return;
  }

  // 在文件合适位置写入类对象t，并返回写入的位置索引index
  // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
  // 位置索引index可以取为对象写入的起始位置,1base
  void write(W &t, int which_node, int size = 1) {
    int place = (which_node - 1) * sizeofT;
    file.open(file_name);
    file.seekp(place);
    file.write(reinterpret_cast<char *>(&t), sizeofT * size);
    file.close();
    return;
  }
  void read(W &t, int which_node, int size = 1) {
    int place = (which_node - 1) * sizeofT;
    file.open(file_name);
    file.seekg(place);
    file.read(reinterpret_cast<char *>(&t), sizeofT * size);
    file.close();
    return;
  }
  void clear() {
    file.open(file_name, std::ios::out);
    file.close();
    for (int i = 0; i < info_len; i++) {
      config[i] = 0;
    }
    return;
  }
};


}

#endif
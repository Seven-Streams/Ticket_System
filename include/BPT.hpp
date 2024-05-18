#include "list.hpp"
#include "map.hpp"
#include "memoryriver.hpp"
#include "vector.hpp"
#ifndef BPT_HPP
#define BPT_HPP
namespace sjtu {
template <class Value = int, int size = 168, int redundancy = 6,
          int cachesize = 200>
class BPT {
private:
  struct MyData {
    unsigned long long hash1 = 0;
    unsigned long long hash2 = 0;
    Value value;
    int son = 0;
    bool operator>(const MyData &other) {
      if (hash1 != other.hash1) {
        return hash1 > other.hash1;
      }
      if (hash2 != other.hash2) {
        return hash2 > other.hash2;
      }
      return value > other.value;
    }
    bool operator<(const MyData &other) {
      if (hash1 != other.hash1) {
        return hash1 < other.hash1;
      }
      if (hash2 != other.hash2) {
        return hash2 < other.hash2;
      }
      return value < other.value;
    }
    bool operator==(const MyData &other) {
      if (hash1 != other.hash1) {
        return false;
      }
      if (hash2 != other.hash2) {
        return false;
      }
      return value == other.value;
    }
    bool operator!=(const MyData &other) { return !(*this == other); }
  };
  struct Node {
    int empty[redundancy];
    MyData datas[size + 1];
    int left_sibling = 0;
    int right_sibling = 0;
    int now_size = 0;
    int pos = 0;
  } nothing;
  sjtu::vector<int> recycle;
  int B_total = 0;
  int B_root = 0;
  int B_current = 0;
  MemoryRiver<Node, 3> mydatabase;
  MemoryRiver<int, 1> myrecycle;
  sjtu::list<Node> mycache;
  sjtu::map<int, typename sjtu::list<Node>::iterator> mycache_map;
  void ReadwithCache(Node &res, int pos) {
    if (mycache_map.count(pos)) {
      res = *(mycache_map[pos]);
      mycache.erase(mycache_map[pos]);
      mycache.push_front(res);
      mycache_map[pos] = mycache.begin();
      return;
    }
    mydatabase.read(res, pos);
    mycache.push_front(res);
    mycache_map[pos] = mycache.begin();
    if (mycache.size() > cachesize) {
      Node the_back = mycache.back();
      mycache_map.erase(mycache_map.find(the_back.pos));
      mydatabase.write(the_back, the_back.pos);
      mycache.pop_back();
    }
    // Node res;
    // mydatabase.read(res, pos);
    return;
  }
  void WritewithCache(Node to_write, int nothing) {
    if (mycache_map.count(to_write.pos)) {
      mycache.erase(mycache_map[to_write.pos]);
      mycache.push_front(to_write);
      mycache_map[to_write.pos] = mycache.begin();
      return;
    }
    mycache.push_front(to_write);
    mycache_map[to_write.pos] = mycache.begin();
    if (mycache.size() > cachesize) {
      Node the_back = mycache.back();
      mycache_map.erase(mycache_map.find(the_back.pos));
      mydatabase.write(the_back, the_back.pos);
      mycache.pop_back();
    }
    // mydatabase.write(to_write, to_write.pos);
    return;
  }
  void NodeInsert(const MyData &to_insert, const int &pos, const int &last_node,
                  const Node &last_parent) {
    Node res;
    ReadwithCache(res, pos);
    int find = 0;
    for (find = 0; find < res.now_size; find++) {
      if (res.datas[find] > to_insert) {
        break;
      }
    }
    if (res.datas[0].son == 0) {
      if (find != res.now_size) {
        std::memmove(&res.datas[find + 1], &res.datas[find],
                     (res.now_size - find) * sizeof(MyData));
      }
      res.datas[find] = to_insert;
      res.now_size++;
      WritewithCache(res, pos);
    } else {
      if (find == (res.now_size)) {
        find--;
        auto x = to_insert;
        x.son = res.datas[find].son;
        res.datas[find] = x;
        WritewithCache(res, pos);
      }
      NodeInsert(to_insert, res.datas[find].son, res.pos, res);
    }
    if (res.now_size >= size) {
      Split(res.pos, last_node, last_parent);
    }
    return;
  }
  void Split(const int &pos, const int &last_node, Node parent) {
    Node res;
    Node new_node;
    ReadwithCache(res, pos);
    int now_size = res.now_size;
    int half = now_size >> 1;
    std::memmove(&new_node.datas[0], &res.datas[0], half * sizeof(MyData));
    std::memmove(&res.datas[0], &res.datas[half],
                 (now_size - half) * sizeof(MyData));
    res.now_size = (now_size - half);
    new_node.now_size = half;
    new_node.left_sibling = res.left_sibling;
    new_node.right_sibling = res.pos;
    if (recycle.empty()) {
      B_current++;
      new_node.pos = B_current; // 至此，所有新节点已经准备完毕。
    } else {
      new_node.pos = recycle.back();
      recycle.pop_back();
    }
    res.left_sibling = new_node.pos;
    Node res1;
    ReadwithCache(res1, new_node.left_sibling);
    res1.right_sibling = new_node.pos;
    WritewithCache(res1, res1.pos);
    MyData index;
    index = new_node.datas[half - 1];
    index.son = new_node.pos;
    if (last_node) {
      OnlyInsert(last_node, index, parent);
    } else {
      int current;
      if (recycle.empty()) {
        current = B_current;
        current++;
        B_current = current;
      } else {
        current = recycle.back();
        recycle.pop_back();
      }
      Node new_alloc;
      new_alloc.now_size = 2;
      new_alloc.left_sibling = 0;
      new_alloc.right_sibling = 0;
      new_alloc.pos = current;
      new_alloc.datas[1] = res.datas[now_size - half - 1];
      new_alloc.datas[1].son = res.pos;
      new_alloc.datas[0] = index;
      WritewithCache(new_alloc, current);
      B_root = current;
    }
    WritewithCache(res, res.pos);
    WritewithCache(new_node, new_node.pos);
    return;
  }
  void OnlyInsert(const int &pos, const MyData &to_insert, const Node &parent) {
    Node res = parent;
    int find = 0;
    for (find = 0; find < res.now_size; find++) {
      if (res.datas[find] > to_insert) {
        break;
      }
    }
    std::memmove(&res.datas[find + 1], &res.datas[find],
                 (res.now_size - find) * sizeof(MyData));
    res.datas[find] = to_insert;
    res.now_size++;
    WritewithCache(res, pos);
    return;
  }
  bool NodeErase(const int &pos, const int &last_pos, MyData to_delete,
                 const int &where, const int &how_many) {
    // pos表示当前节点号、父亲节点号、待删除内容，这个节点是父亲节点的多少号元素、父亲节点有多少个元素。
    Node res;
    ReadwithCache(res, pos);
    int found = 0;
    auto last_one = res.datas[res.now_size - 1];
    for (int i = 0; i < res.now_size; i++) {
      if ((res.datas[i] > to_delete) || (res.datas[i] == to_delete)) {
        if (res.datas[i].son == 0) {
          if (res.datas[i] != to_delete) {
            return false; // didn't find~
          } else {
            if (res.now_size == 1) { // 说明删空了。
              recycle.push_back(res.pos);
              if (res.left_sibling != 0) {
                Node left;
                ReadwithCache(left, res.left_sibling);
                left.right_sibling = res.right_sibling;
                WritewithCache(left, res.left_sibling);
              }
              if (res.right_sibling != 0) {
                Node right;
                ReadwithCache(right, res.right_sibling);
                right.left_sibling = res.left_sibling;
                WritewithCache(right, res.right_sibling);
              }
              auto x = res.datas[0];
              if (last_pos) {
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == x) {
                    if (i != (parent.now_size - 1)) {
                      std::memmove(&parent.datas[i], &parent.datas[i + 1],
                                   (parent.now_size - i - 1) * sizeof(MyData));
                    }
                    parent.now_size--;
                    WritewithCache(parent, last_pos);
                  }
                }
              }
              res.now_size--;
              WritewithCache(res, pos);
              return true;
            }
            if (i != (res.now_size - 1)) {
              std::memmove(&res.datas[i], &res.datas[i + 1],
                           (res.now_size - i - 1) * sizeof(MyData));
            }
            res.now_size--;
            if (last_pos == 0) {
              WritewithCache(res, pos);
              return true;
            }
            if (res.now_size < ((size >> 2) - 1)) {
              if ((where == 0) &&
                  (how_many == 1)) { // 说明这个节点没有办法进行调整。
                if (i == res.now_size) {
                  Node parent;
                  ReadwithCache(parent, last_pos);
                  auto to_change = res.datas[res.now_size];
                  auto to_update = res.datas[res.now_size - 1];
                  for (int i = 0; i < parent.now_size; i++) {
                    if (to_change == parent.datas[i]) {
                      to_update.son = parent.datas[i].son;
                      parent.datas[i] = to_update;
                      WritewithCache(parent, last_pos);
                      WritewithCache(res, pos);
                      return true;
                    }
                  }
                }
                WritewithCache(res, pos);
                return true;
              }
              if (where != (how_many - 1)) {
                int right;
                right = res.right_sibling;
                Node right_s;
                ReadwithCache(right_s, res.right_sibling); // 读入右儿子。
                if (right_s.now_size >= (size >> 1)) {
                  auto to_update = res.datas[res.now_size];
                  res.datas[res.now_size] = right_s.datas[0];
                  res.now_size++;
                  WritewithCache(res, pos);
                  std::memmove(&right_s.datas[0], &right_s.datas[1],
                               (right_s.now_size - 1) * sizeof(MyData));
                  right_s.now_size--;
                  WritewithCache(right_s, right_s.pos);
                  Node parent;
                  ReadwithCache(parent, last_pos);
                  for (int i = 0; i < parent.now_size; i++) {
                    if (parent.datas[i] == to_update) {
                      auto to_change = res.datas[res.now_size - 1];
                      to_change.son = parent.datas[i].son;
                      parent.datas[i] = to_change;
                      WritewithCache(parent, last_pos);
                      return true; // 借块操作完成。右兄弟节点、本身、父节点均得到更新。
                    }
                  }
                } else { // 说明旁边的节点数目数目已经足够少。
                  recycle.push_back(right);
                  auto to_change = res.datas[res.now_size];
                  std::memmove(&res.datas[res.now_size], &right_s.datas[0],
                               right_s.now_size * sizeof(MyData));
                  res.now_size += right_s.now_size;
                  Node parent;
                  ReadwithCache(parent, last_pos);
                  for (int i = 0; i < parent.now_size; i++) {
                    if (parent.datas[i] == to_change) {
                      if (i != (parent.now_size - 1)) {
                        std::memmove(&parent.datas[i], &parent.datas[i + 1],
                                     (parent.now_size - i - 1) *
                                         sizeof(MyData));
                      }
                      parent.now_size--;
                      break;
                    }
                  }
                  auto to_update = res.datas[res.now_size - 1];
                  for (int i = 0; i < parent.now_size; i++) {
                    if (parent.datas[i] == to_update) {
                      parent.datas[i].son = pos;
                      break;
                    }
                  }
                  WritewithCache(parent, last_pos);
                  res.right_sibling = right_s.right_sibling;
                  if (right_s.right_sibling != 0) {
                    Node double_right;
                    ReadwithCache(double_right, right_s.right_sibling);
                    double_right.left_sibling = pos;
                    WritewithCache(double_right, right_s.right_sibling);
                  }
                  WritewithCache(res, pos);
                  return true;
                }
              } else {
                if (i == res.now_size) {
                  auto to_erase = res.datas[res.now_size];
                  auto to_update = res.datas[res.now_size - 1];
                  Node parent;
                  ReadwithCache(parent, last_pos);
                  for (int i = 0; i < parent.now_size; i++) {
                    if (parent.datas[i] == to_erase) {
                      to_update.son = parent.datas[i].son;
                      parent.datas[i] = to_update;
                      WritewithCache(parent, last_pos);
                    }
                  }
                }
                int left;
                left = res.left_sibling;
                Node left_s;
                ReadwithCache(left_s, res.left_sibling); // 读入右儿子。
                if (left_s.now_size >= (size >> 1)) {
                  std::memmove(&res.datas[1], &res.datas[0],
                               res.now_size * sizeof(MyData));
                  res.datas[0] = left_s.datas[left_s.now_size - 1];
                  auto to_change = left_s.datas[left_s.now_size - 1];
                  auto to_update = left_s.datas[left_s.now_size - 2];
                  left_s.now_size--;
                  res.now_size++;
                  WritewithCache(res, pos);
                  WritewithCache(left_s, left);
                  Node parent;
                  ReadwithCache(parent, last_pos);
                  for (int i = 0; i < parent.now_size; i++) {
                    if (parent.datas[i] == to_change) {
                      to_update.son = parent.datas[i].son;
                      parent.datas[i] = to_update;
                      WritewithCache(parent, last_pos);
                      return true;
                    }
                  }
                } else { // 说明旁边的节点数目数目已经足够少。
                  recycle.push_back(left);
                  // std::cout << "CHECK IT" << to_delete.value <<  std::endl;
                  std::memmove(&res.datas[left_s.now_size], &res.datas[0],
                               res.now_size * sizeof(MyData));
                  std::memmove(&res.datas[0], &left_s.datas[0],
                               left_s.now_size * sizeof(MyData));
                  res.now_size += left_s.now_size;
                  res.left_sibling = left_s.left_sibling;
                  WritewithCache(res, pos);
                  if (left_s.left_sibling != 0) {
                    Node double_left;
                    ReadwithCache(double_left, left_s.left_sibling);
                    double_left.right_sibling = pos;
                    WritewithCache(double_left, left_s.left_sibling);
                  }
                  auto to_change = res.datas[left_s.now_size - 1];
                  Node parent;
                  ReadwithCache(parent, last_pos);
                  for (int i = 0; i < parent.now_size; i++) {
                    if (parent.datas[i] == to_change) {
                      if (i != (parent.now_size - 1)) {
                        std::memmove(&parent.datas[i], &parent.datas[i + 1],
                                     (parent.now_size - i - 1) *
                                         sizeof(MyData));
                      }
                      parent.now_size--;
                      WritewithCache(parent, last_pos);
                      break;
                    }
                  }
                  WritewithCache(res, pos);
                  return true;
                }
              }
            } else {
              if (i == res.now_size) {
                auto to_erase = res.datas[res.now_size];
                auto to_update = res.datas[res.now_size - 1];
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_erase) {
                    to_update.son = parent.datas[i].son;
                    parent.datas[i] = to_update;
                    WritewithCache(parent, last_pos);
                    break;
                  }
                }
              }
              WritewithCache(res, pos);
              return true;
            }
          }
        } else {
          bool ans =
              NodeErase(res.datas[i].son, pos, to_delete, i, res.now_size);
          if (ans == false) {
            return false;
          }
          if (last_pos == 0) {
            return true;
          }
          ReadwithCache(res, pos);
          if (res.now_size == 0) { // 说明删空了。
            recycle.push_back(res.pos);
            if (res.left_sibling != 0) {
              Node left;
              ReadwithCache(left, res.left_sibling);
              left.right_sibling = res.right_sibling;
              WritewithCache(left, res.left_sibling);
            }
            if (res.right_sibling != 0) {
              Node right;
              ReadwithCache(right, res.right_sibling);
              right.left_sibling = res.left_sibling;
              WritewithCache(right, res.right_sibling);
            }
            auto x = res.datas[0];
            if (last_pos) {
              Node parent;
              ReadwithCache(parent, last_pos);
              for (int i = 0; i < parent.now_size; i++) {
                if (parent.datas[i] == x) {
                  if (i != (parent.now_size - 1)) {
                    std::memmove(&parent.datas[i], &parent.datas[i + 1],
                                 (parent.now_size - i - 1) * sizeof(MyData));
                  }
                  parent.now_size--;
                  WritewithCache(parent, last_pos);
                  break;
                }
              }
            }
            WritewithCache(res, pos);
            return true;
          }
          if (res.datas[res.now_size - 1] != last_one) { // 应当向上修改。
            Node parent;
            ReadwithCache(parent, last_pos);
            for (int i = 0; i < parent.now_size; i++) {
              if (parent.datas[i] == last_one) {
                auto to_update = res.datas[res.now_size - 1];
                to_update.son = parent.datas[i].son;
                parent.datas[i] = to_update;
                WritewithCache(parent, last_pos);
                break;
              }
            }
          }
          if (res.now_size < ((size >> 1) - 1)) {
            if ((where == 0) &&
                (how_many == 1)) { // 说明这个节点没有办法进行调整。
              if (i == res.now_size) {
                Node parent;
                ReadwithCache(parent, last_pos);
                auto to_change = res.datas[res.now_size];
                auto to_update = res.datas[res.now_size - 1];
                for (int i = 0; i < parent.now_size; i++) {
                  if (to_change == parent.datas[i]) {
                    to_update.son = parent.datas[i].son;
                    parent.datas[i] = to_update;
                    WritewithCache(parent, last_pos);
                    WritewithCache(res, pos);
                    return true;
                  }
                }
              }
              WritewithCache(res, pos);
              return true;
            }
            if (where != (how_many - 1)) {
              int right;
              right = res.right_sibling;
              Node right_s;
              ReadwithCache(right_s, res.right_sibling); // 读入右儿子。
              if (right_s.now_size >= (size >> 1)) {
                auto to_update = res.datas[res.now_size];
                res.datas[res.now_size] = right_s.datas[0];
                res.now_size++;
                WritewithCache(res, pos);
                std::memmove(&right_s.datas[0], &right_s.datas[1],
                             (right_s.now_size - 1) * sizeof(MyData));
                right_s.now_size--;
                WritewithCache(right_s, right_s.pos);
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_update) {
                    auto to_change = res.datas[res.now_size - 1];
                    to_change.son = parent.datas[i].son;
                    parent.datas[i] = to_change;
                    WritewithCache(parent, last_pos);
                    return true; // 借块操作完成。右兄弟节点、本身、父节点均得到更新。
                  }
                }
              } else { // 说明旁边的节点数目数目已经足够少。
                recycle.push_back(right);
                auto to_change = res.datas[res.now_size];
                std::memmove(&res.datas[res.now_size], &right_s.datas[0],
                             right_s.now_size * sizeof(MyData));
                res.now_size += right_s.now_size;
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_change) {
                    if (i != (parent.now_size - 1)) {
                      std::memmove(&parent.datas[i], &parent.datas[i + 1],
                                   (parent.now_size - i - 1) * sizeof(MyData));
                    }
                    parent.now_size--;
                    break;
                  }
                }
                auto to_update = res.datas[res.now_size - 1];
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_update) {
                    parent.datas[i].son = pos;
                    break;
                  }
                }
                WritewithCache(parent, last_pos);
                res.right_sibling = right_s.right_sibling;
                if (right_s.right_sibling != 0) {
                  Node double_right;
                  ReadwithCache(double_right, right_s.right_sibling);
                  double_right.left_sibling = pos;
                  WritewithCache(double_right, right_s.right_sibling);
                }
                WritewithCache(res, pos);
                return true;
              }
            } else {
              if (i == res.now_size) {
                auto to_erase = res.datas[res.now_size];
                auto to_update = res.datas[res.now_size - 1];
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_erase) {
                    to_update.son = parent.datas[i].son;
                    parent.datas[i] = to_update;
                    WritewithCache(parent, last_pos);
                  }
                }
              }
              int left;
              left = res.left_sibling;
              Node left_s;
              ReadwithCache(left_s, res.left_sibling); // 读入右儿子。
              if (left_s.now_size >= (size >> 1)) {
                std::memmove(&res.datas[1], &res.datas[0],
                             res.now_size * sizeof(MyData));
                res.datas[0] = left_s.datas[left_s.now_size - 1];
                auto to_change = left_s.datas[left_s.now_size - 1];
                auto to_update = left_s.datas[left_s.now_size - 2];
                left_s.now_size--;
                res.now_size++;
                WritewithCache(res, pos);
                WritewithCache(left_s, left);
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_change) {
                    to_update.son = parent.datas[i].son;
                    parent.datas[i] = to_update;
                    WritewithCache(parent, last_pos);
                    return true;
                  }
                }
              } else { // 说明旁边的节点数目数目已经足够少。
                std::memmove(&res.datas[left_s.now_size], &res.datas[0],
                             res.now_size * sizeof(MyData));
                std::memmove(&res.datas[0], &left_s.datas[0],
                             left_s.now_size * sizeof(MyData));
                res.now_size += left_s.now_size;
                res.left_sibling = left_s.left_sibling;
                WritewithCache(res, pos);
                if (left_s.left_sibling != 0) {
                  Node double_left;
                  ReadwithCache(double_left, left_s.left_sibling);
                  double_left.right_sibling = pos;
                  WritewithCache(double_left, left_s.left_sibling);
                }
                auto to_change = res.datas[left_s.now_size - 1];
                Node parent;
                ReadwithCache(parent, last_pos);
                for (int i = 0; i < parent.now_size; i++) {
                  if (parent.datas[i] == to_change) {
                    if (i != (parent.now_size - 1)) {
                      std::memmove(&parent.datas[i], &parent.datas[i + 1],
                                   (parent.now_size - i - 1) * sizeof(MyData));
                    }
                    parent.now_size--;
                    WritewithCache(parent, last_pos);
                    break;
                  }
                }
                recycle.push_back(left);
                WritewithCache(res, pos);
                return true;
              }
            }
          } else {
            if (i == res.now_size) {
              auto to_erase = res.datas[res.now_size];
              auto to_update = res.datas[res.now_size - 1];
              Node parent;
              ReadwithCache(parent, last_pos);
              for (int i = 0; i < parent.now_size; i++) {
                if (parent.datas[i] == to_erase) {
                  to_update.son = parent.datas[i].son;
                  parent.datas[i] = to_update;
                  WritewithCache(parent, last_pos);
                  break;
                }
              }
            }
            WritewithCache(res, pos);
            return true;
          }
        }
      }
    }
    return false;
  }

public:
  void Clear() {
    recycle.clear();
    B_total = 0;
    B_root = 0;
    B_current = 0;
    mydatabase.clear();
    myrecycle.clear();
    mycache.clear();
  }
  BPT() = delete;
  BPT(std::string name) {
    // int check = sizeof(nothing);
    // std::cout << check << std::endl;
    mydatabase.ChangeName(name);
    mydatabase.get_info(B_total, 1);
    mydatabase.get_info(B_root, 2);
    mydatabase.get_info(B_current, 3);
    std::string recyle_name = name + "_recycle";
    myrecycle.ChangeName(recyle_name);
    int recycle_total;
    myrecycle.get_info(recycle_total, 1);
    for (int i = 1; i <= recycle_total; i++) {
      int res;
      myrecycle.read(res, i);
      recycle.push_back(res);
    }
  }
  ~BPT() {
    mydatabase.write_info(B_total, 1);
    mydatabase.write_info(B_root, 2);
    mydatabase.write_info(B_current, 3);
    myrecycle.write_info(recycle.size(), 1);
    for (int i = 0; i < recycle.size(); i++) {
      myrecycle.write(recycle[i], i + 1);
    }
    for (auto it = mycache.begin(); it != mycache.end(); it++) {
      mydatabase.write(*it, it->pos);
    }
    return;
  }
  void Insert(const unsigned long long &hash1, unsigned long long hash2,
              const Value &value) {
    int total = B_total;
    if (total == 0) {
      Node res1;
      recycle.clear();
      res1.datas[0].hash1 = hash1;
      res1.datas[0].hash2 = hash2;
      res1.datas[0].value = value;
      res1.now_size = 1;
      res1.pos = 1;
      WritewithCache(res1, 1);
      B_root = 1;
      B_total = 1;
      B_current = 1;
    } else {
      int root;
      root = B_root;
      MyData res;
      res.hash1 = hash1;
      res.hash2 = hash2;
      res.value = value;
      NodeInsert(res, root, 0, nothing);
      total++;
      B_total = total;
    }
    return;
  }
  void Replace(const unsigned long long &hash_1,
               const unsigned long long &hash_2, const Value &to_replace) {
    if (B_total == 0) {
      return;
    }
    Node res;
    MyData data;
    data.hash1 = hash_1;
    data.hash2 = hash_2;
    data.value = to_replace;
    ReadwithCache(res, B_root);
    while (res.datas[0].son != 0) {
      for (int i = 0; i < res.now_size; i++) {
        if ((data < res.datas[i]) || (data == res.datas[i])) {
          ReadwithCache(res, res.datas[i].son);
          break;
        }
        if (i == (res.now_size - 1)) {
          return;
        }
      }
    }
    int found = 0;
    for (found = 0; found < res.now_size; found++) {
      if ((hash_1 == res.datas[found].hash1) &&
          (hash_2 == res.datas[found].hash2)) {
        break;
      }
    }
    if (found == res.now_size) {
      return;
    }
    while ((hash_1 == res.datas[found].hash1) &&
           (hash_2 == res.datas[found].hash2)) {
      if (res.datas[found].value == to_replace) {
        res.datas[found].value = to_replace;
        WritewithCache(res, res.pos);
        return;
      }
      found++;
      if (found == res.now_size) {
        if (res.right_sibling == 0) {
          return;
        }
        ReadwithCache(res, res.right_sibling);
        found = 0;
      }
    }
    return;
  }
  sjtu::list<Value> find(const unsigned long long &hash_1,
                         const unsigned long long &hash_2, const Value &min) {
    sjtu::list<Value> answers;
    if (B_total == 0) {
      return answers;
    }
    Node res;
    MyData to_find;
    to_find.hash1 = hash_1;
    to_find.hash2 = hash_2;
    to_find.value = min;
    ReadwithCache(res, B_root);
    while (res.datas[0].son != 0) {
      for (int i = 0; i < res.now_size; i++) {
        if ((to_find < res.datas[i]) || (to_find == res.datas[i])) {
          ReadwithCache(res, res.datas[i].son);
          break;
        }
        if (i == (res.now_size - 1)) {
          return answers;
        }
      }
    }
    int found = 0;
    for (found = 0; found < res.now_size; found++) {
      if ((hash_1 == res.datas[found].hash1) &&
          (hash_2 == res.datas[found].hash2)) {
        break;
      }
    }
    if (found == res.now_size) {
      return answers;
    }
    while ((hash_1 == res.datas[found].hash1) &&
           (hash_2 == res.datas[found].hash2)) {
      if ((res.datas[found].value > min) || (res.datas[found].value == min)) {
        answers.push_back(res.datas[found].value);
      }
      found++;
      if (found == res.now_size) {
        if (res.right_sibling == 0) {
          return answers;
        }
        ReadwithCache(res, res.right_sibling);
        found = 0;
      }
    }
    return answers;
  }
  sjtu::list<Value> find2(const unsigned long long &hash_1,
                          const unsigned long long &hash_2, const Value &min,
                          const Value &max) {
    sjtu::list<Value> answers;
    if (B_total == 0) {
      return answers;
    }
    Node res;
    MyData to_find;
    to_find.hash1 = hash_1;
    to_find.hash2 = hash_2;
    to_find.value = min;
    ReadwithCache(res, B_root);
    while (res.datas[0].son != 0) {
      for (int i = 0; i < res.now_size; i++) {
        if ((to_find < res.datas[i]) || (to_find == res.datas[i])) {
          ReadwithCache(res, res.datas[i].son);
          break;
        }
        if (i == (res.now_size - 1)) {
          return answers;
        }
      }
    }
    int found = 0;
    for (found = 0; found < res.now_size; found++) {
      if ((hash_1 == res.datas[found].hash1) &&
          (hash_2 == res.datas[found].hash2)) {
        break;
      }
    }
    if (found == res.now_size) {
      return answers;
    }
    while ((hash_1 == res.datas[found].hash1) &&
           (hash_2 == res.datas[found].hash2)) {
      if ((res.datas[found].value > max) || (res.datas[found].value == max)) {
        return answers;
      }
      if (((res.datas[found].value > min) || (res.datas[found].value == min))) {
        answers.push_back(res.datas[found].value);
      }
      found++;
      if (found == res.now_size) {
        if (res.right_sibling == 0) {
          return answers;
        }
        ReadwithCache(res, res.right_sibling);
        found = 0;
      }
    }
    return answers;
  }
  void Erase(const unsigned long long &hash_1, const unsigned long long &hash_2,
             const Value &value) {
    MyData to_delete;
    to_delete.hash1 = hash_1;
    to_delete.hash2 = hash_2;
    to_delete.value = value;
    if (NodeErase(B_root, 0, to_delete, 0, 0) != false) {
      B_total--;
      Node to_check;
      ReadwithCache(to_check, B_root);
      if (to_check.now_size == 1 && (B_total != 1)) {
        recycle.push_back(B_root);
        B_root = to_check.datas[0].son;
      }
    }
    return;
  }
  int CheckSize() {
    return sizeof(nothing);
  }
};
} // namespace sjtu
#endif
#include "utility.hpp"
#ifndef PQ_HPP
#define PQ_HPP
namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template <typename T, class Compare = Less<T>> class priority_queue {
  class Node;

private:
  Compare tool;
  Node *root = nullptr;
  size_t total = 0;
  void Clear() {
    if (root == nullptr) {
      return;
    }
    root->Clear();
    delete root;
    return;
  }
  Node *NodeMerge(Node *x, Node *y) {
    if (x == nullptr) {
      return y;
    }
    if (y == nullptr) {
      return x;
    } // The other child is empty.
    Node *tmp = nullptr;
    Node *x_ls, *x_rs, *y_ls, *y_rs;
    x_ls = x->left_son;
    x_rs = x->right_son;
    y_ls = y->left_son;
    y_rs = y->right_son;
    int x_depth = x->depth;
    int y_depth = y->depth;
    try {
      if (tool((*(x->value)), (*(y->value)))) {
        x->right_son = NodeMerge(x->right_son, y);
        if (x->right_son == nullptr || x->left_son == nullptr) {
          x->depth = 1;
          if (x->left_son == nullptr) {
            x->left_son = x->right_son;
            x->right_son = nullptr;
          }
        } else {
          x->depth = (x->right_son->depth) > (x->left_son->depth)
                         ? x->left_son->depth + 1
                         : x->right_son->depth + 1;
          if (x->left_son->depth < x->right_son->depth) {
            tmp = x->left_son;
            x->left_son = x->right_son;
            x->right_son = tmp;
          }
        }
        return x;
      } else {
        y->right_son = NodeMerge(y->right_son, x);
        if (y->right_son == nullptr || y->left_son == nullptr) {
          y->depth = 1;
          if (y->left_son == nullptr) {
            y->left_son = y->right_son;
            y->right_son = nullptr;
          }
        } else {
          y->depth = (y->right_son->depth) > (y->left_son->depth)
                         ? y->left_son->depth + 1
                         : y->right_son->depth + 1;
          if (y->left_son->depth < y->right_son->depth) {
            tmp = y->left_son;
            y->left_son = y->right_son;
            y->right_son = tmp;
          }
        }
        return y;
      } // This part is to merge the larger ls and the rs of the relative root.
    } catch (...) {
      x->left_son = x_ls;
      x->right_son = x_rs;
      y->left_son = y_ls;
      y->right_son = y_rs;
      x->depth = x_depth;
      y->depth = y_depth;
      throw(1);
      return nullptr;
    }
  }
  class Node {
  private:
    Node *left_son = nullptr;
    Node *right_son = nullptr;
    T *value = nullptr;
    int depth = 1;

  public:
    friend class priority_queue;
    Node() {
      left_son = nullptr;
      right_son = nullptr;
      value = nullptr;
      depth = 1;
    }
    Node(const Node &other) = delete;
    Node(const T &input) {
      left_son = nullptr;
      right_son = nullptr;
      value = new T(input);
      depth = 1;
    }
    ~Node() { delete value; }
    const T &GetValue() { return *value; }
    void Copy(Node *other) {
      this->value = new T(*(other->value));
      this->depth = other->depth;
      if (other->left_son != nullptr) {
        this->left_son = new Node();
        this->left_son->Copy(other->left_son);
      }
      if (other->right_son != nullptr) {
        this->right_son = new Node();
        this->right_son->Copy(other->right_son);
      }
      return;
    }
    void Clear() {
      if (left_son != nullptr) {
        left_son->Clear();
        delete left_son;
      }
      if (right_son != nullptr) {
        right_son->Clear();
        delete right_son;
      }
      return;
    } // This function is to clear all the child nodes.
  };

public:
  /**
   * TODO constructors
   */
  priority_queue() {
    total = 0;
    root = nullptr;
  }
  priority_queue(const priority_queue &other) {
    root = new Node();
    root->Copy(other.root);
    this->total = other.total;
  }
  /**
   * TODO deconstructor
   */
  ~priority_queue() { Clear(); }
  /**
   * TODO Assignment operator
   */
  priority_queue &operator=(const priority_queue &other) {
    if (other.root == root) {
      return *this;
    }
    Clear();
    root = new Node();
    total = other.total;
    root->Copy(other.root);
    return *this;
  }
  /**
   * get the top of the queue.
   * @return a reference of the top element.
   * throw container_is_empty if empty() returns true;
   */
  const T &top() const {
    if (root == nullptr) {
      throw(1);
    }
    return root->GetValue();
  }
  /**
   * TODO
   * push new element to the priority queue.
   */
  void push(const T &e) {
    Node *to_push = new Node(e);
    try {
      root = NodeMerge(to_push, root);
      total++;
    } catch (...) {
      delete to_push;
      throw(1);
    }
    return;
  }
  /**
   * TODO
   * delete the top element.
   * throw container_is_empty if empty() returns true;
   */
  void pop() {
    if (root == nullptr) {
      throw(1);
    }
    Node *tmp = root;
    try {
      total--;
      root = NodeMerge(root->left_son, root->right_son);
    } catch (...) {
      delete root;
      throw(1);
    }
    delete tmp;
    return;
  }
  /**
   * return the number of the elements.
   */
  size_t size() const { return total; }
  /**
   * check if the container has at least an element.
   * @return true if it is empty, false if it has at least an element.
   */
  bool empty() const { return (total == 0); }
  /**
   * merge two priority_queues with at least O(logn) complexity.
   * clear the other priority_queue.
   */
  void merge(priority_queue &other) {
    if (other.root == root) {
      return;
    }
    Node *tmp = root;
    int total_old = total;
    try {
      root = NodeMerge(root, other.root);
      other.root = nullptr;
      other.total = 0;
      return;
    } catch (...) {
      root = tmp;
      total = total_old;
      throw(1);
    }
  }
};
} // namespace sjtu
#endif
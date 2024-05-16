#include "utility.hpp"
#ifndef MAP_HPP
#define MAP_HPP
namespace sjtu {
struct my_true_type {
  bool value = true;
};
struct my_false_type {
  bool value = false;
};
template <typename T> struct my_type_traits {
  using iterator_assignable = typename T::iterator_assignable;
  bool operator()() {
    iterator_assignable t;
    return t.value;
  }
};
template <class Key, class T, class Compare = Less<Key>> class map {
  class Node;

private:
  Node *root = nullptr;
  int total = 0;
  class Node {
  public:
    typedef pair<const Key, T> value_type;

  private:
    value_type *data = nullptr;
    Node *left_son = nullptr;
    Node *right_son = nullptr;
    Node *parent = nullptr;
    int level = 1;

  public:
    Node() = default;
    Node(Key k, T v, Node *p = nullptr) {
      data = new value_type(k, v);
      left_son = nullptr;
      right_son = nullptr;
      parent = p;
      level = 1;
    }
    Node(const Node &other) = delete;
    ~Node() { delete data; }
    inline Node *Split() {
      if (right_son == nullptr) {
        return this;
      }
      if (right_son->right_son == nullptr) {
        return this;
      }
      if (right_son->level != level || right_son->right_son->level != level) {
        return this;
      }
      Node *tmp = this->right_son;
      tmp->parent = parent;
      right_son = tmp->left_son;
      if (right_son != nullptr) {
        right_son->parent = this;
      }
      tmp->left_son = this;
      parent = tmp;
      (tmp->level)++;
      return tmp;
    }
    inline Node *Skew() {
      if (left_son == nullptr) {
        return this;
      }
      if (left_son->level != level) {
        return this;
      }
      Node *tmp = this->left_son;
      left_son = left_son->right_son;
      if (left_son != nullptr) {
        left_son->parent = this;
      }
      tmp->parent = parent;
      tmp->right_son = this;
      parent = tmp;
      return tmp;
    }
    Node *FindNode(const Key &k) {
      Compare tool;
      if ((!tool((data->first), k)) && (!tool(k, data->first))) {
        return this;
      }
      if (tool(k, data->first)) {
        if (left_son == nullptr) {
          return nullptr;
        }
        return left_son->FindNode(k);
      } else {
        if (right_son == nullptr) {
          return nullptr;
        }
        return right_son->FindNode(k);
      }
    }
    Node *InsertNode(const Key &k, const T &v, bool &tag) {
      Compare tool;
      if (tool(k, data->first)) {
        if (left_son == nullptr) {
          left_son = new Node(k, v, this);
          return left_son;
        } else {
          Node *tmp = nullptr;
          tmp = left_son->InsertNode(k, v, tag);
          if (left_son != nullptr) {
            left_son = left_son->Skew();
            left_son = left_son->Split();
          }
          return tmp;
        }
      } else {
        if (!tool(data->first, k)) {
          tag = false;
          return this;
        }
        if (right_son == nullptr) {
          right_son = new Node(k, v, this);
          return right_son;
        } else {
          Node *tmp = nullptr;
          tmp = right_son->InsertNode(k, v, tag);
          right_son = right_son->Skew();
          right_son = right_son->Split();
          return tmp;
        }
      }
    }
    void Copy(const Node *other, Node *p) {
      data = new value_type(other->data->first, other->data->second);
      level = other->level;
      parent = p;
      if (other->left_son != nullptr) {
        left_son = new Node();
        left_son->Copy(other->left_son, this);
      } else {
        left_son = nullptr;
      }
      if (other->right_son != nullptr) {
        right_son = new Node();
        right_son->Copy(other->right_son, this);
      } else {
        right_son = nullptr;
      }
      return;
    }
    void Destroy() {
      if (left_son != nullptr) {
        left_son->Destroy();
        delete left_son;
      }
      if (right_son != nullptr) {
        right_son->Destroy();
        delete right_son;
      }
      return;
    }
    friend class map<Key, T, Compare>;
  };
  void RemoveNode(const Key &k, Node *&now) {
    Compare tool;
    if (now == nullptr) {
      throw(1);
    }
    if ((!(tool(k, (now->data->first))) && (!(tool((now->data->first), k))))) {
      if (now->left_son != nullptr && now->right_son != nullptr) {
        Node *tmp = now->right_son;
        while (tmp->left_son != nullptr) {
          tmp = tmp->left_son;
        }
        bool is_son = false;
        if (now->right_son == tmp) {
          is_son = true;
        }
        if (tmp->parent->left_son == tmp) {
          tmp->parent->left_son =
              new Node((tmp->data->first), (tmp->data->second), tmp->parent);
          auto new_node = tmp->parent->left_son;
          new_node->left_son = tmp->left_son;
          new_node->right_son = tmp->right_son;
          new_node->level = tmp->level;
        } else {
          tmp->parent->right_son =
              new Node((tmp->data->first), (tmp->data->second), tmp->parent);
          auto new_node = tmp->parent->right_son;
          new_node->left_son = tmp->left_son;
          new_node->right_son = tmp->right_son;
          new_node->level = tmp->level;
        }
        RemoveNode((tmp->data->first), now->right_son);
        if (!is_son) {
          tmp->right_son = now->right_son;
        }
        tmp->left_son = now->left_son;
        tmp->parent = now->parent;
        if (now->left_son != nullptr) {
          tmp->left_son->parent = tmp;
        }
        if (now->right_son != nullptr) {
          tmp->right_son->parent = tmp;
        }
        delete now;
        now = tmp;
      } else {
        Node *tmp = now;
        if (now->left_son == nullptr && now->right_son == nullptr) {
          if (now->parent == nullptr) {
            delete now;
            now = nullptr;
            return;
          } else {
            if (now->parent->left_son == now) {
              now->parent->left_son = nullptr;
              delete tmp;
              now = nullptr;
            } else {
              now->parent->right_son = nullptr;
              delete tmp;
              now = nullptr;
            }
            return;
          }
        }
        auto res = now;
        if (now->left_son == nullptr) {
          now->right_son->parent = now->parent;
          now = now->right_son;
          delete res;
        } else {
          now->left_son->parent = now->parent;
          now = now->left_son;
          delete res;
        }
        return;
      }
    } else {
      if (tool(k, (now->data->first))) {
        RemoveNode(k, now->left_son);
      } else {
        RemoveNode(k, now->right_son);
      }
    }
    if (now->left_son != nullptr) {
      now->left_son = now->left_son->Skew();
      now->left_son = now->left_son->Split();
    }
    if (now->right_son != nullptr) {
      now->right_son = now->right_son->Skew();
      now->right_son = now->right_son->Split();
    }
    if (now->left_son == nullptr || now->right_son == nullptr) {
      now->level = 1;
    } else {
      now->level = std::min(now->left_son->level, now->right_son->level) + 1;
    }
    if (now->right_son != nullptr && now->right_son->level > now->level) {
      now->level++;
    }
    return;
  }

public:
  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<const Key, T> value_type;
  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  class const_iterator;
  class iterator {
  private:
    Node *ptr;
    map<Key, T, Compare> *which;
    using iterator_assignable = my_true_type;

  public:
    friend struct my_type_traits<iterator>;
    friend class map;
    iterator() {
      ptr = nullptr;
      which = nullptr;
    }
    iterator(const iterator &other) {
      ptr = other.ptr;
      which = other.which;
    }
    iterator(Node *v, map<Key, T, Compare> *m) {
      ptr = v;
      which = m;
    }
    ~iterator() = default;
    /**
     * TODO iter++
     */
    iterator operator++(int x) {
      auto res = *this;
      Node *tmp = ptr;
      if (ptr == nullptr) {
        throw(2);
      } // end.
      if (tmp->right_son != nullptr) {
        tmp = tmp->right_son;
        while (tmp->left_son != nullptr) {
          tmp = tmp->left_son;
        }
        ptr = tmp;
        return res;
      } else {
        if (tmp->parent == nullptr) {
          ptr = nullptr;
          return res;
        } // to the end.
        while (tmp->parent->right_son == tmp) {
          Node *last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->right_son == last) {
            ptr = nullptr;
            return res;
          }
        }
        ptr = tmp->parent;
        if (ptr != nullptr && ptr->right_son != nullptr) {
        }
        return res;
      }
    }
    /**
     * TODO ++iter
     */
    iterator &operator++() {
      Node *tmp = ptr;
      if (ptr == nullptr) {
        throw(2);
      } // end.
      if (tmp->right_son != nullptr) {
        tmp = tmp->right_son;
        while (tmp->left_son != nullptr) {
          tmp = tmp->left_son;
        }
        ptr = tmp;
        return *this;
      } else {
        if (tmp->parent == nullptr) {
          ptr = nullptr;
          return *this;
        } // to the end.
        Node *last = nullptr;
        while (tmp->parent->right_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->right_son == last) {
            ptr = nullptr;
            return *this;
          }
        }
        ptr = tmp->parent;
        return *this;
      }
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      auto res = *this;
      Node *tmp = ptr;
      if (tmp == nullptr) {
        tmp = which->root;
        if (tmp == nullptr) {
          throw(2);
        }
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return res;
      } // end.
      if (tmp->left_son != nullptr) {
        tmp = tmp->left_son;
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return res;
      } else {
        if (tmp->parent == nullptr) {
          throw(2);
        }
        while (tmp->parent->left_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->left_son == last) {
            throw(2);
          }
        }
        ptr = tmp->parent;
        return res;
      }
    }
    /**
     * TODO --iter
     */
    iterator &operator--() {
      Node *tmp = ptr;
      if (tmp == nullptr) {
        tmp = which->root;
        if (tmp == nullptr) {
          throw(2);
        }
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return *this;
      } // end.
      if (tmp->left_son != nullptr) {
        tmp = tmp->left_son;
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return *this;
      } else {
        if (tmp->parent == nullptr) {
          throw(2);
        }
        while (tmp->parent->left_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->left_son == last) {
            throw(2);
          }
        }
        ptr = tmp->parent;
        return *this;
      }
    }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory).
     */
    value_type &operator*() const { return *(ptr->data); }
    iterator &operator=(const iterator &other) {
      if (this->ptr == other.ptr && this->which == other.which) {
        return *this;
      } else {
        this->ptr = other.ptr;
        this->which = other.which;
        return *this;
      }
    }
    bool operator==(const iterator &rhs) const {
      return rhs.ptr == ptr && rhs.which == which;
    }
    bool operator==(const const_iterator &rhs) const {
      return rhs.ptr == ptr && rhs.which == which;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return rhs.ptr != ptr || rhs.which != which;
    }
    bool operator!=(const const_iterator &rhs) const {
      return rhs.ptr != ptr || rhs.which != which;
    }

    /**
     * for the support of it->first.
     * See
     * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
     * for help.
     */
    value_type *operator->() const noexcept { return ptr->data; }
  };
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
  private:
    using iterator_assignable = my_false_type;
    Node *ptr;
    map<Key, T, Compare> *which;

  public:
    friend struct my_type_traits<const_iterator>;
    friend class map;
    const_iterator() {
      ptr = nullptr;
      which = nullptr;
    }
    const_iterator(const const_iterator &other) {
      ptr = other.ptr;
      which = other.which;
    }
    const_iterator(const iterator &other) {
      ptr = other.ptr;
      which = other.which;
    }
    const_iterator(Node *v, map<Key, T, Compare> *m) {
      ptr = v;
      which = m;
    }
    ~const_iterator() = default;
    const value_type &operator*() const { return *(ptr->data); }
    const value_type *operator->() const noexcept { return ptr->data; }
    const_iterator operator++(int x) {
      auto res = *this;
      Node *tmp = ptr;
      if (ptr == nullptr) {
        throw(2);
      } // end.
      if (tmp->right_son != nullptr) {
        tmp = tmp->right_son;
        while (tmp->left_son != nullptr) {
          tmp = tmp->left_son;
        }
        ptr = tmp;
        return res;
      } else {
        if (tmp->parent == nullptr) {
          ptr = nullptr;
          return res;
        } // to the end.
        Node *last = nullptr;
        while (tmp->parent->right_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->right_son == last) {
            ptr = nullptr;
            return res;
          }
        }
        ptr = tmp->parent;
        return res;
      }
    }
    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      Node *tmp = ptr;
      if (ptr == nullptr) {
        throw(2);
      } // end.
      if (tmp->right_son != nullptr) {
        tmp = tmp->right_son;
        while (tmp->left_son != nullptr) {
          tmp = tmp->left_son;
        }
        ptr = tmp;
        return *this;
      } else {
        if (tmp->parent == nullptr) {
          ptr = nullptr;
          return *this;
        } // to the end.
        Node *last = nullptr;
        while (tmp->parent->right_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->right_son == last) {
            ptr = nullptr;
            return *this;
          }
        }
        ptr = tmp->parent;
        return *this;
      }
    }
    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      auto res = *this;
      Node *tmp = ptr;
      if (tmp == nullptr) {
        tmp = which->root;
        if (tmp == nullptr) {
          throw(2);
        }
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return res;
      } // end.
      if (tmp->left_son != nullptr) {
        tmp = tmp->left_son;
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return res;
      } else {
        if (tmp->parent == nullptr) {
          throw(2);
        }
        while (tmp->parent->left_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->left_son == last) {
            throw(2);
          }
        }
        ptr = tmp->parent;
        return res;
      }
    }
    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      Node *tmp = ptr;
      if (tmp == nullptr) {
        tmp = which->root;
        if (tmp == nullptr) {
          throw(2);
        }
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return *this;
      } // end.
      if (tmp->left_son != nullptr) {
        tmp = tmp->left_son;
        while (tmp->right_son != nullptr) {
          tmp = tmp->right_son;
        }
        ptr = tmp;
        return *this;
      } else {
        if (tmp->parent == nullptr) {
          throw(2);
        }
        while (tmp->parent->left_son == tmp) {
          auto last = tmp;
          tmp = tmp->parent;
          if (tmp->parent == nullptr && tmp->left_son == last) {
            throw(2);
          }
        }
        ptr = tmp->parent;
        return *this;
      }
    }
    const_iterator &operator=(const const_iterator &other) {
      if (this->ptr == other.ptr && this->which == other.which) {
        return *this;
      } else {
        this->ptr = other.ptr;
        this->which = other.which;
        return *this;
      }
    }
    bool operator==(const iterator &rhs) const {
      return rhs.ptr == ptr && rhs.which == which;
    }
    bool operator==(const const_iterator &rhs) const {
      return rhs.ptr == ptr && rhs.which == which;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return rhs.ptr != ptr || rhs.which != which;
    }
    bool operator!=(const const_iterator &rhs) const {
      return rhs.ptr != ptr || rhs.which != which;
    }
  };
  /**
   * TODO two constructors
   */
  map() = default;
  map(const map &other) {
    total = other.total;
    if (other.root == nullptr) {
      root = nullptr;
    } else {
      root = new Node();
      root->Copy(other.root, nullptr);
    }
  }
  /**
   * TODO assignment operator
   */
  map &operator=(const map &other) {
    if (other.root == root) {
      return *this;
    }
    clear();
    if (other.root == nullptr) {
      root = nullptr;
      total = 0;
      return *this;
    }
    root = new Node();
    root->Copy(other.root, nullptr);
    total = other.total;
    return *this;
  }
  /**
   * TODO Destructors
   */
  ~map() { clear(); }
  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key equivalent
   * to key. If no such element exists, an exception of type
   * `index_out_of_bound'
   */
  T &at(const Key &key) {
    if (root == nullptr) {
      throw(3);
    }
    Node *tmp;
    tmp = root->FindNode(key);
    if (tmp == nullptr) {
      throw(3);
    }
    return tmp->data->second;
  }
  const T &at(const Key &key) const {
    if (root == nullptr) {
      throw(3);
    }
    Node *tmp;
    tmp = root->FindNode(key);
    if (tmp == nullptr) {
      throw(3);
    }
    return tmp->data->second;
  }
  /**
   * TODO
   * access specified element
   * Returns a reference to the value that is mapped to a key equivalent to key,
   *   performing an insertion if such key does not already exist.
   */
  T &operator[](const Key &key) {
    if (root == nullptr) {
      root = new Node(key, T(), nullptr);
      total++;
      return root->data->second;
    }
    bool res = true;
    auto tmp = root->InsertNode(key, T(), res);
    if (res) {
      total++;
    }
    return tmp->data->second;
  }
  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const {
    if (root == nullptr) {
      throw(3);
    }
    Node *tmp;
    tmp = root->FindNode(key);
    if (tmp == nullptr) {
      throw(3);
    }
    return (tmp->data->second);
  }
  /**
   * return a iterator to the beginning
   */
  iterator begin() {
    iterator x;
    x.which = const_cast<map *>(this);
    if (root == nullptr) {
      x.ptr = nullptr;
      return x;
    }
    auto tmp = root;
    while (tmp->left_son != nullptr) {
      tmp = tmp->left_son;
    }
    x.ptr = tmp;
    return x;
  }
  const_iterator cbegin() const {
    const_iterator x;
    x.which = const_cast<map *>(this);
    if (root == nullptr) {
      x.ptr = nullptr;
      return x;
    }
    auto tmp = root;
    while (tmp->left_son != nullptr) {
      tmp = tmp->left_son;
    }
    x.ptr = tmp;
    return x;
  }
  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() {
    iterator x(nullptr, const_cast<map *>(this));
    return x;
  }
  const_iterator cend() const {
    const_iterator x(nullptr, const_cast<map *>(this));
    return x;
  }
  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const { return total == 0; }
  /**
   * returns the number of elements.
   */
  size_t size() const { return total; }
  /**
   * clears the contents
   */
  void clear() {
    if (root == nullptr) {
      return;
    }
    root->Destroy();
    total = 0;
    delete root;
    root = nullptr;
    return;
  }
  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the
   * insertion), the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &value) {
    bool flag = false;
    Node *ans = nullptr;
    if (root == nullptr) {
      root = new Node(value.first, value.second, nullptr);
      iterator x(root, this);
      root->parent = nullptr;
      total++;
      return pair<iterator, bool>(x, true);
    }
    bool success = true;
    ans = root->InsertNode(value.first, value.second, success);
    if (success) {
      flag = true;
      total++;
    }
    iterator x;
    x.ptr = ans;
    x.which = this;
    if (root != nullptr) {
      root->parent = nullptr;
      root = root->Skew();
      root = root->Split();
    }
    return pair<iterator, bool>(x, flag);
  }
  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points an
   * element out of this)
   */
  void erase(iterator pos) {
    if (pos.ptr == nullptr || pos.which != this) {
      throw(1);
    }
    RemoveNode(pos.ptr->data->first, root);
    total--;
    if (root != nullptr) {
      root->parent = nullptr;
      root = root->Skew();
      root = root->Split();
    }
    return;
  }
  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    if (root == nullptr) {
      return 0;
    }
    Node *tmp = nullptr;
    tmp = root->FindNode(key);
    if (tmp == nullptr) {
      return 0;
    }
    return 1;
  }
  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is
   * returned.
   */
  iterator find(const Key &key) {
    if (root == nullptr) {
      iterator x;
      x.which = const_cast<map<Key, T, Compare> *>(this);
      x.ptr = nullptr;
      return x;
    }
    auto tmp = root->FindNode(key);
    iterator x;
    x.which = const_cast<map<Key, T, Compare> *>(this);
    x.ptr = tmp;
    return x;
  }
  const_iterator find(const Key &key) const {
    if (root == nullptr) {
      const_iterator x(cend());
      x.which = const_cast<map<Key, T, Compare> *>(this);
      x.ptr = nullptr;
      return x;
    }
    auto tmp = root->FindNode(key);
    const_iterator x(cend());
    x.which = const_cast<map<Key, T, Compare> *>(this);
    x.ptr = tmp;
    return x;
  }
};
} // namespace sjtu
#endif
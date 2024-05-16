#include <iostream>
#ifndef LIST_HPP
#define LIST_HPP
namespace sjtu {

/**
 * @tparam T Type of the elements.
 * Be careful that T may not be default constructable.
 *
 * @brief A list that supports operations like std::list.
 *
 * We encourage you to design the implementation yourself.
 * As for the memory management, you may use std::allocator,
 * new/delete, malloc/free or something else.
 */
template <class T> struct node_list {
  T *value = nullptr;
  node_list<T> *pre = nullptr;
  node_list<T> *nxt = nullptr;
  node_list(node_list<T> *_pr, T x, node_list<T> *nx = nullptr) {
    pre = _pr;
    value = new T(x);
    nxt = nx;
  }
  node_list() {
    value = nullptr;
    pre = nullptr;
    nxt = nullptr;
  }
  ~node_list() { delete value; }
};
template <typename T> class list {
private:
  size_t total = 0;
  node_list<T> head;
  node_list<T> tail;

public:
  class iterator;
  class const_iterator;

public:
  /**
   * Constructs & Assignments
   * At least three: default constructor, copy constructor/assignment
   * Bonus: move/initializer_list constructor/assignment
   */
  list() {
    head.nxt = &tail;
    tail.pre = &head;
    total = 0;
  }
  list(const list &other) {
    node_list<T> *last = &head;
    total = other.total;
    for (auto it = other.head.nxt; it->nxt != nullptr; it = it->nxt) {
      last->nxt = new node_list<T>(last, *(it->value));
      last = last->nxt;
    }
    last->nxt = &tail;
    tail.pre = last;
  }
  list &operator=(const list &other) {
    if (&head == &other.head) {
      return *this;
    }
    node_list<T> *last = nullptr;
    total = other.total;
    for (auto it = head.nxt; it != nullptr; it = it->nxt) {
      delete last;
      last = it;
    }
    last = &head;
    for (auto it = other.head.nxt; it->nxt != nullptr; it = it->nxt) {
      last->nxt = new node_list<T>(last, *(it->value));
      last = last->nxt;
    }
    last->nxt = &tail;
    tail.pre = last;
    return *this;
  }

  /* Destructor. */
  ~list() {
    node_list<T> *last = nullptr;
    for (auto it = head.nxt; it != nullptr; it = it->nxt) {
      delete last;
      last = it;
    }
  }

  /* Access the first / last element. */
  T &front() noexcept { return *(head.nxt->value); }
  T &back() noexcept { return *(tail.pre->value); }
  const T &front() const noexcept { return *(head.nxt->value); }
  const T &back() const noexcept { return *(tail.pre->value); }

  /* Return an iterator pointing to the first element. */
  iterator begin() noexcept {
    iterator res(head.nxt);
    return res;
  }
  const_iterator cbegin() const noexcept {
    const_iterator res(head.nxt);
    return res;
  }

  /* Return an iterator pointing to one past the last element. */
  iterator end() noexcept {
    iterator res(&tail);
    return res;
  }
  const_iterator cend() const noexcept {
    const_iterator res(const_cast<node_list<T> *>(&tail));
    return res;
  }

  /* Checks whether the container is empty. */
  bool empty() const noexcept { return !size(); }
  /* Return count of elements in the container. */
  size_t size() const noexcept { return total; }

  /* Clear the contents. */
  void clear() noexcept {
    node_list<T> *now = nullptr;
    for (auto it = head.nxt; it != nullptr; it = it->nxt) {
      delete now;
      now = it;
    }
    head.nxt = &tail;
    total = 0;
    return;
  }
  /**
   * @brief Insert value before pos (pos may be the end() iterator).
   * @return An iterator pointing to the inserted value.
   * @throw Throw if the iterator is invalid.
   */
  iterator insert(iterator pos, const T &value) {
    node_list<T> *res = new node_list<T>(pos.now->pre, value);
    res->nxt = pos.now;
    res->pre->nxt = res;
    pos.now->pre = res;
    total++;
    iterator x(res);
    return res;
  }

  /**
   * @brief Remove the element at pos (remove end() iterator is invalid).
   * returns an iterator pointing to the following element, if pos pointing to
   * the last element, end() will be returned.
   * @throw Throw if the container is empty, or the iterator is invalid.
   */
  iterator erase(iterator pos) noexcept {
    total--;
    iterator ans(pos.now->nxt);
    pos.now->nxt->pre = pos.now->pre;
    pos.now->pre->nxt = ans.now;
    delete pos.now;
    return ans;
  }

  /* Add an element to the front/back. */
  void push_front(const T &value) {
    node_list<T> *res = new node_list<T>(&head, value);
    res->nxt = head.nxt;
    head.nxt->pre = res;
    head.nxt = res;
    total++;
    return;
  }
  void push_back(const T &value) {
    node_list<T> *res = new node_list<T>(tail.pre, value);
    res->nxt = &tail;
    tail.pre = res;
    res->pre->nxt = res;
    total++;
    return;
  }

  /* Removes the first/last element. */
  void pop_front() noexcept {
    head.nxt = head.nxt->nxt;
    delete head.nxt->pre;
    head.nxt->pre = &head;
    total--;
    return;
  }
  void pop_back() noexcept {
    tail.pre = tail.pre->pre;
    delete tail.pre->nxt;
    tail.pre->nxt = &tail;
    total--;
    return;
  }

public:
  /**
   * Basic requirements:
   * operator ++, --, *, ->
   * operator ==, != between iterators and const iterators
   * constructing a const iterator from an iterator
   *
   * If your implementation meets these requirements,
   * you may not comply with the following template.
   * You may even move this template outside the class body,
   * as long as your code works well.
   *
   * Contact TA whenever you are not sure.
   */
  class iterator {
  public:
    iterator() = default;
    iterator(const iterator &x) = default;
    ~iterator() {}
    iterator(node_list<T> *x) { now = x; }
    node_list<T> *now = nullptr;
    iterator &operator++() {
      now = now->nxt;
      return *this;
    }
    iterator &operator--() {
      now = now->pre;
      return *this;
    }
    iterator operator++(int x) {
      auto res = *this;
      now = now->nxt;
      return res;
    }
    iterator operator--(int x) {
      auto res = *this;
      now = now->pre;
      return res;
    }
    T &operator*() const noexcept { return *(now->value); }
    T *operator->() const noexcept { return (now->value); }

    /* A operator to check whether two iterators are same (pointing to the same
     * memory) */
    friend bool operator==(const iterator &a, const iterator &b) {
      return a.now == b.now;
    }
    friend bool operator!=(const iterator &a, const iterator &b) {
      return a.now != b.now;
    }
  };

  /**
   * Const iterator should have same functions as iterator, just for a const
   * object. It should be able to construct from an iterator. It should be able
   * to compare with an iterator.
   */
  class const_iterator {
  public:
    const_iterator() = default;
    const_iterator(const const_iterator &x) = default;
    ~const_iterator() {}
    node_list<T> *now = nullptr;
    const_iterator(const iterator &x) { now = x.now; }
    friend bool operator==(const const_iterator &a, const const_iterator &b) {
      return a.now == b.now;
    }
    friend bool operator!=(const const_iterator &a, const const_iterator &b) {
      return a.now != b.now;
    }
    const_iterator &operator++() {
      now = now->nxt;
      return *this;
    }
    const_iterator &operator--() {
      now = now->pre;
      return *this;
    }
    const_iterator operator++(int x) const {
      auto res = *this;
      now = now->nxt;
      return res;
    }
    const_iterator operator--(int x) const {
      auto res = *this;
      now = now->pre;
      return res;
    }
    T &operator*() const noexcept { return *(now->value); }
    T *operator->() const noexcept { return *(now->value); }
  };
};

} // namespace sjtu
#endif
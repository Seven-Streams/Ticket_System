#include "../include/order.hpp"
sjtu::MemoryRiver<Order, 1> order_info("order_info");
sjtu::BPT<int> order_id("order_id");
sjtu::BPT<IndexTime> order_user("order_user");
bool IndexTime::operator<(const IndexTime &other) {
  return (stamp > other.stamp);
} // Which means when use find function, will return the resently order instead
  // of the eralier.
bool IndexTime::operator>(const IndexTime &other) {
  return (stamp < other.stamp);
}
bool IndexTime::operator==(const IndexTime &other) {
  return (stamp == other.stamp);
}
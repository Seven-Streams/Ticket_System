#include "../include/order.hpp"
sjtu::MemoryRiver<Order, 1> order_info("order_info");
sjtu::BPT<int> order_id("order_id");
sjtu::BPT<int> order_user("order_user");
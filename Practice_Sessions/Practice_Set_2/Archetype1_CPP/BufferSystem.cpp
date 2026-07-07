#include "BufferSystem.h"
#include "Utils.h"
#include <stdexcept>
// Write your BufferSystem implementations here:

BufferSystem::BufferSystem() {
  const char *env_filter = std::getenv("DEFAULT_MIN_FILTER");
  if (env_filter) {
    parse_int(env_filter, filtered_val);
    filtering = true;
  }
}

bool BufferSystem::is_filtered(int &value) {
  if (filtering) {
    if (value < filtered_val) {
      return true;
    }
  }
  return false;
}

void BufferSystem::push(int value) {
  if (is_filtered(value)) {
    return;
  }
  stack.push_back(value);
  return;
}

int BufferSystem::pop() {
  if (stack.empty()) {
    throw underflow_error("Stack Empty");
  }
}
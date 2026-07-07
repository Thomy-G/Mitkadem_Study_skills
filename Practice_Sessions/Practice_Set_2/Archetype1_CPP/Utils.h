#include <string>

using namespace std;

static bool parse_int(const string &str, int &out_val) {
  try {
    size_t idx;
    out_val = stoi(str, &idx);
    return idx == str.size();
  } catch (...) {
    return false;
  }
}
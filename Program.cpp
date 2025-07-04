
#include "Program.hpp"
#include <ostream>

std::ostream &operator<<(std::ostream &os, condition &cdn) {
  os << "Key : " << cdn.key << "\n";
  os << "Operator : " << cdn.oper << "\n";
  os << "Value : " << cdn.value << "\n";
  return os;
}

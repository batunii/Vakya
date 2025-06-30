#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
class Prompt {
public:
  const size_t step_number = 0;
  std::string step_name = "";
  std::string action = "";
  std::string context = "";
  std::string source = "";
  std::string output_format = "";
  std::vector<std::string> format_specifiers;
  std::unordered_map<std::string, std::string> ordering;
  std::unordered_map<std::string, std::string> meta_data;
  std::vector<std::string> conditions;

  Prompt(size_t step) : step_number(step), format_specifiers(std::vector<std::string>()) {}
};
inline std::ostream &operator<<(std::ostream &os,
                                std::vector<std::string> list) {
  for (auto &condition : list) {
    os << condition << " ";
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os, Prompt &pt) {
  os << "Do " << pt.action << " on the given topic - " << pt.context
     << " with source as " << pt.source
     << ".\nThe output should be in " << pt.output_format
     << "And have the following fields" << pt.format_specifiers
     << " following the conditions" << pt.conditions << std::endl;
  return os;
}

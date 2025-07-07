#ifndef PROGRAM_H
#define PROGRAM_H

#include <optional>
#include <ostream>
#include <string>
#include <vector>

// ops<T>: action_name and its associated properties
template <typename T> class ops {
public:
  std::string action_name;
  T action_props;
};

// ls_props<T>: properties divided into must, should, and could
template <typename T> class ls_props {
public:
  std::optional<std::vector<T>> must;
  std::optional<std::vector<T>> should;
  std::optional<std::vector<T>> could;
};

// condition: key-value condition expression
class condition {
public:
  std::string key = "";
  std::string oper = "";
  std::string value = "";
};

// fmt_class: formatting structure with type, order, and metadata
class fmt_class {
public:
  ops<ls_props<std::string>> *type;
  ops<ls_props<condition>> *order;
  ops<ls_props<condition>> *meta;
};

// Program: full program structure
class Program {
public:
  ops<std::string> *do_token;
  ops<std::string> *on_token;
  ops<ls_props<std::string>> *src_token;
  fmt_class *fmt_token;
  ops<ls_props<condition>> *cdn_token;
};

// ----------- ostream overloads (inlined templates) -----------

// ostream for condition
inline std::ostream &operator<<(std::ostream &os, const condition &cdn) {
  os << "Key : " << cdn.key << "\n";
  os << "Operator : " << cdn.oper << "\n";
  os << "Value : " << cdn.value << "\n";
  return os;
}

// ostream for ls_props<T>
template <typename T>
std::ostream &operator<<(std::ostream &os, const ls_props<T> &ls) {
  if (ls.must.has_value()) {
    os << "Must:\n";
    for (const auto &item : ls.must.value()) {
      os << "  - " << item << "\n";
    }
  }
  if (ls.should.has_value()) {
    os << "Should:\n";
    for (const auto &item : ls.should.value()) {
      os << "  - " << item << "\n";
    }
  }
  if (ls.could.has_value()) {
    os << "Could:\n";
    for (const auto &item : ls.could.value()) {
      os << "  - " << item << "\n";
    }
  }
  return os;
}

// ostream for ops<T>
template <typename T>
std::ostream &operator<<(std::ostream &os, const ops<T> &op) {
  os << "Action Name: " << op.action_name << "\n";
  os << "Action Props:\n" << op.action_props;
  return os;
}

// ostream for fmt_class
inline std::ostream &operator<<(std::ostream &os, const fmt_class &fmt) {
  os << "[Format Type]\n";
  if (fmt.type)
    os << *fmt.type << "\n";

  if (fmt.order) {
    os << "[Format Order]\n";
    os << *fmt.order << "\n";
  }

  os << "[Format Metadata]\n";
  if (fmt.meta)
    os << *fmt.meta << "\n";

  return os;
}

// ostream for Program
inline std::ostream &operator<<(std::ostream &os, const Program &program) {
  os << "Program Details:\n";
  if (program.do_token) {
    os << "[DO Token]\n" << *program.do_token << "\n";
  }
  if (program.on_token) {
    os << "[ON Token]\n" << *program.on_token << "\n";
  }
  if (program.src_token) {
    os << "[SRC Token]\n" << *program.src_token << "\n";
  }
  if (program.fmt_token) {
    os << "[FMT Token]\n" << *program.fmt_token << "\n";
  }
  if (program.cdn_token) {
    os << "[CDN Token]\n" << *program.cdn_token << "\n";
  }
  return os;
}

#endif // PROGRAM_H

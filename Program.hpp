#ifndef PROGRAM_H
#define PROGRAM_H

#include <ostream>
#include <string>
#include <vector>

template <typename T> class ops {
public:
  std::string action_name;
  T action_props;
};

template <typename T> class ls_props {
public:
  std::vector<T> must;
  std::vector<T> should;
  std::vector<T> could;
};

class condition {
public:
  std::string key;
  std::string oper;
  std::string value;
};

class fmt_class {
public:
  ops<ls_props<std::string>> *type;
  std::vector<ops<ls_props<std::string>>> *order;
  ops<ls_props<condition>> *meta;
};

class Program {
public:
  ops<std::string> do_token;
  ops<std::string> on_token;
  ops<ls_props<std::string>> *src_token;
  fmt_class *fmt_token;
  ops<ls_props<condition>> *cdn_token;
};

std::ostream &operator<<(std::ostream &os, condition &cdn);
std::ostream &operator<<(std::ostream &os, fmt_class &fmt_props);
std::ostream &operator<<(std::ostream &os, Program &program);
template <typename T> std::ostream &operator<<(std::ostream &os, ops<T> &ops);
template <typename T>
std::ostream &operator<<(std::ostream &os, ls_props<T> &ls_props);

#endif // !PROGRAM_H

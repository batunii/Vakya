#include "Vakya_Lexer.hpp"
#include "Vakya_Program.hpp"
class AST {
private:
  size_t curr_token = 0;
  size_t next_token = 1;
  Lexer &lexer;
  std::vector<Program *> program_steps;
  Program *curr_program;
  std::optional<Program *> get_curr_program();
  std::optional<Tokens> advance_token();
  std::optional<Tokens> peek_token();
  ops<ls_props<std::string>> *parse_parenthesis(const std::string &action_name);
  void parse_condition(condition &curr_condition, const Tokens &curr_token,
                       std::string &token_type);
  ops<ls_props<condition>> *parse_braces(std::string &&action_name);
  void parse_src();
  void parse_fmt();
  void parse_cdn();
  void parse_do();
  void parse_on();

public:
  AST(Lexer &lexter);
  std::optional<Program *> get_program();
  void print_programs();
  void start_compiler();
};

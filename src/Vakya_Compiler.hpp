#include "Vakya_Lexer.hpp"
#include "Vakya_Program.hpp"
#include <memory>
#include <vector>

class AST {
private:
  size_t curr_token = 0;
  size_t next_token = 1;
  Lexer &lexer;
  std::vector<std::shared_ptr<Program>> program_steps;
  std::shared_ptr<Program> curr_program;
  std::optional<std::shared_ptr<Program>> get_curr_program();
  std::optional<Tokens> advance_token();
  std::optional<Tokens> peek_token();
  std::unique_ptr<ops<ls_props<std::string>>>
  parse_parenthesis(const std::string &);
  std::unique_ptr<ops<ls_props<condition>>> parse_braces(std::string &&);
  void parse_condition(condition &, const Tokens &, std::string &);
  void parse_src();
  void parse_fmt();
  void parse_cdn();
  void parse_do();
  void parse_on();

public:
  AST(Lexer &);
  std::optional<std::shared_ptr<Program>> get_program();
  void print_programs();
  void start_compiler();
};

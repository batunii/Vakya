#include "Token_Utils.hpp"
#include "Vakya_Lexer.hpp"
#include <iostream>
#include <optional>

class Prompt {
private:
  Lexer &lexer;
  std::string action;
  bool is_multistep;
  std::string topic;
  std::vector<std::string> conditions;
  std::unordered_map<std::string, std::string> std_fmt_types = {
      {"table", "The user wants the output in a table format."}};
  size_t current_token;
  std::optional<Tokens> next_token() {
    if (current_token < lexer.t_list.size())
      return lexer.t_list[current_token++];
    else
      return std::nullopt;
  }
  void do_called() {}
  void on_called() {}
  void pre_def_output() {}
  void usr_def_output() {}
  void fmt_called() {}
  void cdn_called() {}
  void add_step() {}

public:
  Prompt(Lexer &lexer) : lexer(lexer) { lexer.make_tokens(); }
  void start_compiler() {
    while (auto token = this->next_token()) {
      switch (token->t_type) {
      case TokenType::TT_DO: {
        this->do_called();
        break;
      }
      case TokenType::TT_ON: {
      }
      default:
        std::cout << "Do not called \n";
      }
    }
  }
};

int main() {
  std::string code, line;
  std::cout << "Vakya (enter multiple lines, Ctrl+D to end input):\n";
  while (std::getline(std::cin, line)) {
    code += line + "\n"; // Preserve line breaks
  }

  Lexer lexer(code);
  lexer.make_tokens();
  std::cout << lexer.t_list << "\n";
  return 0;
}

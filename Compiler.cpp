#include "Vakya_Lexer.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Prompt {
private:
  std::vector<Tokens> &token_list;
  std::string action;
  bool is_multistep;
  std::string topic;
  std::vector<std::string> conditions;
  std::unordered_map<std::string, std::string> std_fmt_types = {
      {"table", "The user wants the output in a table format."}};
  int current_token;
  std::optional<Tokens> next_token() {
    ++current_token;
    if (current_token < token_list.size())
      return token_list[current_token];
    else
      return std::nullopt;
  }
  void do_called() {}

public:
  Prompt(std::vector<Tokens> &tokens) : token_list(tokens) {}
  void start_compiler() {
    while (auto token = this->next_token()) {
      switch (token->t_type) {
      case TokenType::TT_DO: {
        std::cout << "Do is called \n";
        break;
      }
      default:
        std::cout << "Do not called \n";
      }
    }
  }
};

int main() {
  std::vector<Tokens> exit_code = {Tokens(TokenType::TT_ATTR, "exit"),
                                   Tokens(TokenType::TT_LP), TokenType::TT_RP};

  std::string code, line;
  std::cout << "Vakya (enter multiple lines, Ctrl+D to end input):\n";
  while (std::getline(std::cin, line)) {
    code += line + "\n"; // Preserve line breaks
  }

  Lexer lexer(code);
  lexer.make_tokens();
  std::cout << lexer.t_list << "\n";
  Prompt().start_compiler(lexer.make_tokens());

  return 0;
}

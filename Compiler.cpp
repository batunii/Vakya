#include "Program.hpp"
#include "Vakya_Lexer.hpp"
#include "Vakya_Prompt.cpp"
#include <iostream>
#include <optional>
#include <string>

class AST {
  int curr_token = -1;
  int next_token = 0;
  Lexer *lexer;
public:
  std::optional<Tokens> advance_token() {
    if (curr_token < lexer->t_list.size()) {
      ++next_token;
      return lexer->t_list[++curr_token];
    } else
      return std::nullopt;
  }
  Tokens peek_token() { return lexer->t_list[next_token]; }
};

int main() {
  std::string code, line;
  std::cout << "Vakya (enter multiple lines, Ctrl+D to end input):\n";
  while (std::getline(std::cin, line)) {
    code += line + "\n"; // Preserve line breaks
  }

  Lexer lexer(code);
  std::cout << lexer.make_tokens() << std::endl;
  return 0;
}

#include "Program.hpp"
#include "Vakya_Lexer.hpp"
#include "Vakya_Prompt.cpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

class AST {
  size_t curr_token = 0;
  size_t next_token = 1;
  Lexer *lexer;
  std::vector<Program> program_steps;
  Program *curr_program;

public:
  std::optional<Tokens> advance_token() {
    if (curr_token < lexer->t_list.size()) {
      ++next_token;
      return lexer->t_list[this->curr_token++];
    } else
      return std::nullopt;
  }
  std::optional<Tokens> peek_token() {
    std::optional<Tokens> next_token = this->advance_token();
    --this->curr_token;
    --this->next_token;
    return next_token;
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

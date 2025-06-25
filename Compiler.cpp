#include "Vakya_Lexer.cpp"
#include <iostream>
#include <string>

int main() {
  std::vector<Tokens> exit_code = {
      Tokens(TokenType::TT_ATTR, "exit"),
      Tokens(TokenType::TT_LP),
      TokenType::TT_RP
  };

  std::string code, line;
  std::cout << "Vakya (enter multiple lines, Ctrl+D to end input):\n";
  while (std::getline(std::cin, line)) {
    code += line + "\n"; // Preserve line breaks
  }

  Lexer lexer(code);
  lexer.make_tokens();
  std::cout << lexer.t_list << "\n";

  // Optional: if last line was "exit()"
  if (lexer.t_list.size() >= 3 &&
      lexer.t_list[lexer.t_list.size() - 3] == Tokens(TokenType::TT_ATTR, "exit") &&
      lexer.t_list[lexer.t_list.size() - 2].t_type == TokenType::TT_LP &&
      lexer.t_list[lexer.t_list.size() - 1].t_type == TokenType::TT_RP)
    return 0;

  return 0;
}


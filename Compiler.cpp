#include "Vakya_Lexer.cpp"
#include <iostream>
#include <string>

int main() {
  std::vector<Tokens> exit_code = {Tokens(TokenType::TT_ATTR, "exit"),
                                   Tokens(TokenType::TT_LP),
                                   TokenType(TokenType::TT_RP)};
  while (true) {
    std::cout << "Vakya>> ";
    std::string code;
    std::getline(std::cin, code);
    Lexer lexer(code);
    lexer.make_tokens();
    std::cout << lexer.t_list << "\n";
    if (lexer.t_list == exit_code)
      break;
  }
}

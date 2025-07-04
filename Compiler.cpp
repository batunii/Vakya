#include "Vakya_Lexer.hpp"
#include "Vakya_Prompt.cpp"
#include <iostream>
#include <string>

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

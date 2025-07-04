#include "Program.hpp"
#include "Token_Utils.hpp"
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
  std::vector<Program *> program_steps;
  Program *curr_program;

  std::optional<Program *> get_curr_program() {
    if (size_t program_size = this->program_steps.size())
      return this->program_steps[program_size - 1];
    return std::nullopt;
  }

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
  }
  ops<ls_props<std::string>> *parse_parenthesis() {
    ops<ls_props<std::string>> *props = new ops<ls_props<std::string>>();
    std::vector<std::string> curr_list = props->action_props.should;
    std::optional<Tokens> next_token = this->advance_token();
    while (next_token && (next_token->t_type != TokenType::TT_RP)) {
      switch (next_token->t_type) {
      case TokenType::TT_EX: {
        curr_list = props->action_props.must;
        break;
      }
      case TokenType::TT_QM: {
        curr_list = props->action_props.could;
        break;
      }
      case TokenType::TT_CM: {
        curr_list = props->action_props.should;
        break;
      }
      case TokenType::TT_STR:
      case TokenType::TT_ATTR: {
        curr_list.emplace_back(next_token->t_val);
        break;
      }
      default:
        std::cout << "Wrong token at: " << next_token->location << "\n";
        break;
      }
      next_token = this->advance_token();
    }
    return props;
  }
  void parse_do() {}
  void parse_src() {
    std::optional<Tokens> next_token = this->advance_token();
    if (next_token && next_token->t_type == TokenType::TT_LP) {
      if (get_curr_program().has_value()) {
        this->curr_program = this->get_curr_program().value();
        this->curr_program->src_token = this->parse_parenthesis();
      } else {
        std::cout << "No current Program \n";
      }
    } else {
      std::cout << "Wrong token at " << next_token->location << "\n";
    }
  }

public:
  void start_compiler() {
    while (std::optional<Tokens> curr_token = this->advance_token()) {
      switch (curr_token->t_type) {
      case TokenType::TT_DO: {
        parse_do();
        std::cout << "DO token type at : " << curr_token->location << "\n";
        break;
      }
      case TokenType::TT_ON: {
        std::cout << "ON token type at : " << curr_token->location << "\n";
        break;
      }
      case TokenType::TT_SRC: {
        std::cout << "SRC token type at : " << curr_token->location << "\n";
        break;
      }
      default: {
        std::cout << "Wrong token type at : " << curr_token->location << "\n";
        break;
      }
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
  std::cout << lexer.make_tokens() << std::endl;

  return 0;
}

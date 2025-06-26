#include "Token_Utils.hpp"
#include "Vakya_Lexer.hpp"
#include "Vakya_Prompt.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

class AST {
private:
  Lexer &lexer;
  std::vector<Prompt> prompts;
  Prompt *curr_prompt;
  bool end_of_line = false;
  size_t current_token = 0;
  
	std::optional<Tokens> next_token() {
    if (this->current_token < this->lexer.t_list.size())
      return this->lexer.t_list[this->current_token++];
    else
      return std::nullopt;
  }
  void do_called() {
    this->curr_prompt = new Prompt(this->prompts.size() + 1);
    while (auto token = this->next_token()) {
      if (this->end_of_line)
        break;
      switch (token->t_type) {
      case TokenType::TT_ATTR:
        this->curr_prompt->action = token->t_val;
        break;
      case TokenType::TT_USR:
        this->curr_prompt->step_name = token->t_val;
        break;
      case TokenType::TT_EOL:
        this->end_of_line = true;
        break;
      default:
        std::cout << " You have entered wrong token at " << token->location
                  << "\n";
        break;
      }
    }
  }
  void on_called() {

    while (auto token = this->next_token()) {
      if (this->end_of_line)
        break;
      switch (token->t_type) {
      case TokenType::TT_ATTR:
        this->curr_prompt->context = token->t_val;
        break;
      case TokenType::TT_USR:
        std::cout << "Multi Steps are coming in next update \n";
        break;
      case TokenType::TT_EOL:
        this->end_of_line = true;
        break;
      default:
        std::cout << " You have entered wrong token at " << token->location
                  << "\n";
        break;
      }
    }
  }
  void pre_def_output() {
    auto token = this->next_token();
    if (!token || token->t_type != TokenType::TT_LP)
      std::cout << "Error at "
                << token.value_or(Tokens(TokenType::TT_ILL, 0)).location;
    else {
      auto token = this->next_token();
      while (token && token->t_type != TokenType::TT_LP) {
        this->curr_prompt->format_specifiers.emplace_back(token->t_val);
				token = this->next_token();
      }
    };
  }
  void ordering() {

	}
  void meta_data() {}

  void fmt_called() {
    while (auto token = this->next_token()) {
      if (this->end_of_line)
        break;
      switch (token->t_type) {
      case TokenType::TT_TBL:
      case TokenType::TT_LST:
      case TokenType::TT_BL:
      case TokenType::TT_USR:
        this->pre_def_output();
        break;
      case TokenType::TT_NXT:
        this->ordering();
        break;
      case TokenType::TT_LB:
        this->meta_data();
        break;
      case TokenType::TT_EOL:
        this->end_of_line = true;
        break;
      default:
        std::cout << " You have entered wrong token at " << token->location
                  << "\n";
        break;
      }
    }
  }
  void cdn_called() {}
  void add_step() {}

  std::string make_the_prompt() { return ""; }

public:
  AST(Lexer &lexer)
      : lexer(lexer),prompts(std::vector<Prompt>()){
    std::cout << lexer.make_tokens() << "\n";
  }
  void start_compiler() {
    while (auto token = this->next_token()) {
      this->end_of_line = false;
      switch (token->t_type) {
      case TokenType::TT_DO: {
        this->do_called();
        break;
      }
      case TokenType::TT_ON: {
        break;
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
  AST ast_compiler(lexer);
  ast_compiler.start_compiler();
  return 0;
}

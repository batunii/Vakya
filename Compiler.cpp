#include "Token_Utils.hpp"
#include "Vakya_Lexer.hpp"
#include <iostream>
#include <optional>
#include <string>

class Prompt {
private:
  Lexer &lexer;
  std::string action;
  bool is_multistep;
  std::string topic;
  std::vector<std::string> conditions;

  std::unordered_map<std::string, std::string> std_fmt_types = {
      {"table", "The user wants the output in a table format."}};
  size_t current_token = 0;
  std::optional<Tokens> next_token() {
    if (this->current_token < this->lexer.t_list.size())
      return this->lexer.t_list[this->current_token++];
    else
      return std::nullopt;
  }
  void do_called() {
    bool line_ended = false;
    while (auto token = this->next_token()) {
      if (line_ended)
        break;
      switch (token->t_type) {
      case TokenType::TT_ATTR:
        this->action = token->t_val;
        break;
      case TokenType::TT_USR:
        this->is_multistep = true;
        break;
      case TokenType::TT_EOL:
        line_ended = true;
        break;
      default:
        std::cout << " You have entered wrong token at " << token->location
                  << "\n";
        break;
      }
    }
  }
  void on_called() {

    bool line_ended = false;
    while (auto token = this->next_token()) {
      if (line_ended)
        break;
      switch (token->t_type) {
      case TokenType::TT_ATTR:
        this->topic = token->t_val;
        break;
      case TokenType::TT_USR:
        std::cout << "Multi Steps are coming in next update \n";
        break;
      case TokenType::TT_EOL:
        line_ended = true;
        break;
      default:
        std::cout << " You have entered wrong token at " << token->location
                  << "\n";
        break;
      }
    }
  }
  void pre_def_output() {}
  void usr_def_output() {}
  void ordering() {}
  void meta_data() {}
  void fmt_called() {

    bool line_ended = false;
    while (auto token = this->next_token()) {
      if (line_ended)
        break;
      switch (token->t_type) {
      case TokenType::TT_TBL:
      case TokenType::TT_LST:
      case TokenType::TT_BL:
        this->pre_def_output();
        break;
      case TokenType::TT_USR:
        this->usr_def_output();
        break;
      case TokenType::TT_NXT:
        this->ordering();
        break;
      case TokenType::TT_LB:
        this->meta_data();
        break;
      case TokenType::TT_EOL:
        line_ended = true;
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
  Prompt(Lexer &lexer) : lexer(lexer), current_token(0) {
    std::cout << lexer.make_tokens() << "\n";
  }
  void start_compiler() {
    while (auto token = this->next_token()) {
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
  Prompt prompt(lexer);
  prompt.start_compiler();
  return 0;
}

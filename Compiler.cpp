#include "Token_Utils.hpp"
#include "Vakya_Lexer.hpp"
#include "Vakya_Prompt.cpp"
#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

class AST {
private:
  Lexer &lexer;
  std::vector<Prompt> prompts;
  Prompt *curr_prompt;
  bool end_of_line = false;
  size_t current_token = 0;
  std::unordered_set<TokenType> ordering_table = {
      TokenType::TT_ASC, TokenType::TT_DSC, TokenType::TT_GRP};

  std::optional<Tokens> next_token() {
    if (this->current_token < this->lexer.t_list.size())
      return this->lexer.t_list[this->current_token++];
    else
      return std::nullopt;
  }
  std::optional<Tokens> peek_next_token(){
    if(this->current_token <this->lexer.t_list.size()){
      return this->lexer.t_list[this->current_token+1];}
    else
      return std::nullopt;
  }

  void illegal_input(size_t location) {
    std::cout << "You have entered wrong token at " << location << "\n";
  }
  void do_called() {
    this->curr_prompt = new Prompt(this->prompts.size() + 1);
    while (auto token = this->next_token()) {
      if (this->end_of_line) {
        --this->current_token;
        return;
      }
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
        this->illegal_input(token->location);
        break;
      }
    }
  }
  void on_called() {

    while (auto token = this->next_token()) {
      if (this->end_of_line) {
        --this->current_token;
        return;
      }
      switch (token->t_type) {
      case TokenType::TT_ATTR:
        this->curr_prompt->context += token->t_val + " ";
        break;
      case TokenType::TT_USR:
        std::cout << "Multi Steps are coming in next update \n";
        break;
      case TokenType::TT_EOL:
        this->end_of_line = true;
        break;
      default:
        this->illegal_input(token->location);
        break;
      }
    }
  }
  void pre_def_output() {
    auto token = this->next_token();
    if (!token || token->t_type != TokenType::TT_LP)
      this->illegal_input(
          token.value_or(Tokens(TokenType::TT_ILL, 0)).location);
    else {
      auto token = this->next_token();
      while (token && token->t_type != TokenType::TT_LP) {
        this->curr_prompt->format_specifiers.emplace_back(token->t_val);
        token = this->next_token();
      }
    };
  }
  void ordering() {
    auto ordering_key = this->next_token();
    auto ordering_val = this->next_token();
    if (!ordering_key.has_value() ||
        this->ordering_table.find(ordering_key->t_type) ==
            this->ordering_table.end() ||
        !ordering_val.has_value() ||
        ordering_val->t_type != TokenType::TT_USR) {
      this->illegal_input(
          ordering_key.value_or(Tokens(TokenType::TT_ILL, 0)).location);
      return;
    }
    switch (ordering_key->t_type) {
    case TokenType::TT_ASC:
      this->curr_prompt->ordering.emplace("ascending order by",
                                          ordering_val->t_val);
      break;
    case TokenType::TT_DSC:
      this->curr_prompt->ordering.emplace("descending order by",
                                          ordering_val->t_val);
      break;
    case TokenType::TT_GRP:
      this->curr_prompt->ordering.emplace("group by", ordering_val->t_val);
      break;
    default:
      illegal_input(ordering_key->location);
    }
  }
  void meta_data() {}

  void fmt_called() {
    while (auto token = this->next_token()) {
      if (this->end_of_line){
        --this->current_token;
				return;
			}
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
  AST(Lexer &lexer) : lexer(lexer), prompts(std::vector<Prompt>()) {
    std::cout << lexer.make_tokens() << "\n";
  }
  void start_compiler() {
    while (auto token = this->next_token()) {
      this->end_of_line = false;
      switch (token->t_type) {
      case TokenType::TT_DO:
        this->do_called();
        break;
      case TokenType::TT_ON:
        this->on_called();
        break;
      case TokenType::TT_FMT:
        this->fmt_called();
        break;
      default:
        this->illegal_input(token->location);
      }
    }
    std::cout << *this->curr_prompt << "\n";
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

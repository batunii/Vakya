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
  ops<ls_props<std::string>> *parse_parenthesis(std::string &&action_name) {
    ops<ls_props<std::string>> *props = new ops<ls_props<std::string>>();
    props->action_name = action_name;
    std::vector<std::string> *curr_list = nullptr;
    std::optional<Tokens> next_token = this->advance_token();
    while (next_token && (next_token->t_type != TokenType::TT_RP)) {
      switch (next_token->t_type) {
      case TokenType::TT_EX: {
        if (!props->action_props.must.has_value())
          props->action_props.must.emplace();

        curr_list = &*props->action_props.must;
        break;
      }
      case TokenType::TT_QM: {
        if (!props->action_props.could.has_value())
          props->action_props.could.emplace();
        curr_list = &*props->action_props.could;
        break;
      }
      case TokenType::TT_CM: {
        if (!props->action_props.should.has_value())
          props->action_props.should.emplace();
        curr_list = &*props->action_props.should;
        break;
      }
      case TokenType::TT_STR:
      case TokenType::TT_ATTR: {
        curr_list->emplace_back(next_token->t_val);
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
  void parse_src() {
    std::optional<Tokens> next_token = this->advance_token();
    if (next_token && next_token->t_type == TokenType::TT_LP) {
      if (get_curr_program().has_value()) {
        this->curr_program = this->get_curr_program().value();
        this->curr_program->src_token = this->parse_parenthesis("src");
      } else {
        std::cout << "No current Program \n";
      }
    } else {
      std::cout << "Wrong token at " << next_token->location << "\n";
    }
  }

  void parse_do() {
    auto action_token = this->advance_token();
    if (!action_token || (action_token->t_type != TokenType::TT_ATTR &&
                          action_token->t_type != TokenType::TT_STR)) {
      std::cout << "Expected token after @do\n";
      return;
    }
    this->curr_program = new Program();
    ops<std::string> *new_do_token = new ops<std::string>();
    curr_program->do_token = new_do_token;
    curr_program->do_token->action_name = "do";
    curr_program->do_token->action_props = action_token->t_val;
    this->program_steps.emplace_back(this->curr_program);
    this->curr_program = this->program_steps.back();
  }
  void parse_on() {
    auto action_token = this->advance_token();
    if (!action_token || (action_token->t_type != TokenType::TT_ATTR &&
                          action_token->t_type != TokenType::TT_STR)) {
      std::cout << "Expected token after @on\n";
      return;
    }
    if (this->get_curr_program().has_value()) {
      this->curr_program = this->get_curr_program().value();
      ops<std::string> *new_on_token = new ops<std::string>();
      this->curr_program->on_token = new_on_token;
      this->curr_program->on_token->action_name = "on";
      this->curr_program->on_token->action_props = action_token->t_val;
    } else {
      std::cout << "No current Program found";
      return;
    }
  }

public:
  void start_compiler(Lexer *lexer_input) {
    this->lexer = lexer_input;
    while (auto curr_token = this->advance_token()) {
      switch (curr_token->t_type) {
      case TokenType::TT_DO: {
        parse_do();
        break;
      }
      case TokenType::TT_ON: {
        parse_on();
        break;
      }
      case TokenType::TT_SRC: {
        parse_src();
        break;
      }
      default: {
        std::cout << "Wrong token type at : " << curr_token->location << "\n";
        break;
      }
      }
    }
  }
  void print_programs() {
    for (const auto &prgrm : this->program_steps) {
      std::cout << *prgrm << "\n";
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

  AST ast;
  ast.start_compiler(&lexer);
  ast.print_programs();
  return 0;
}

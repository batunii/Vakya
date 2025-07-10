#include "Token_Utils.hpp"
#include "Vakya_Error.hpp"
#include "Vakya_Lexer.hpp"
#include "Vakya_Program.hpp"
#include <iostream>

class AST {
  size_t curr_token = 0;
  size_t next_token = 1;
  Lexer &lexer;
  std::vector<Program *> program_steps;
  Program *curr_program;
  std::optional<Program *> get_curr_program() {
    if (size_t program_size = this->program_steps.size())
      return this->program_steps[program_size - 1];
    return std::nullopt;
  }

  std::optional<Tokens> advance_token() {
    if (curr_token < lexer.t_list.size()) {
      ++next_token;
      return lexer.t_list[this->curr_token++];
    } else
      return std::nullopt;
  }
  std::optional<Tokens> peek_token() {
    std::optional<Tokens> next_token = this->advance_token();
    --this->curr_token;
    --this->next_token;
    return next_token;
  }
  ops<ls_props<std::string>> *
  parse_parenthesis(const std::string &action_name) {
    if (auto next_token = this->advance_token();
        !(next_token.has_value() && next_token->t_type == TokenType::TT_LP))
      throw vakya_error("Wrong parenthesis syntax", next_token->location);
    ops<ls_props<std::string>> *props = new ops<ls_props<std::string>>();
    props->action_name = action_name;
    std::optional<std::vector<std::string>> *curr_list =
        &props->action_props.should;
    auto ensure_list_ready = [&curr_list]() -> std::vector<std::string> & {
      if (!curr_list->has_value())
        curr_list->emplace();
      return curr_list->value();
    };
    std::string curr_value = "";
    std::optional<Tokens> next_token = this->advance_token();
    while (next_token && (next_token->t_type != TokenType::TT_RP)) {
      switch (next_token->t_type) {
      case TokenType::TT_EX: {
        curr_list = &props->action_props.must;
        break;
      }
      case TokenType::TT_QM: {
        curr_list = &props->action_props.could;
        break;
      }
      case TokenType::TT_CM: {
        ensure_list_ready().emplace_back(std::move(curr_value));
        curr_value.clear();
        curr_list = &props->action_props.should;
        break;
      }
      case TokenType::TT_STR:
      case TokenType::TT_ATTR: {
        curr_value.append(" " + next_token->t_val);
        // ensure_list_ready().end()->append(next_token->t_val);
        // ensure_list_ready().emplace_back(next_token->t_val);
        break;
      }
      case TokenType::TT_CL: {
        std::optional<Tokens> new_token = this->advance_token();
        if (action_name != "source" && new_token.has_value() &&
            (new_token->t_type == TokenType::TT_ATTR ||
             new_token->t_type == TokenType::TT_STR)) {
          curr_value.append(" as " + new_token->t_val);
        } else {
          throw vakya_error("Error in parsing parenthesis, with colon",
                            new_token->location);
        }

        break;
      }
      default:
        throw vakya_error("Error in parsing parenthesis with action word " +
                              action_name,
                          next_token->location);
        break;
      }
      next_token = this->advance_token();
    }
    if (!curr_value.empty())
      ensure_list_ready().emplace_back(std::move(curr_value));
    return props;
  }

  void parse_condition(condition &curr_condition, const Tokens &curr_token,
                       std::string &token_type) {
    if (curr_condition.oper.empty() && token_type == "value_key") {
      curr_condition.key.append(" " + curr_token.t_val);
    } else if (curr_condition.oper.empty() && token_type == "isto") {
      auto it_value = isto_operators.find(curr_condition.key);
      curr_condition.oper =
          it_value != isto_operators.end() ? it_value->second : " is ";
    } else if (token_type == "oper") {
      auto it_value = operators_map.find(curr_token.t_type);
      curr_condition.oper.append(it_value != operators_map.end()
                                     ? it_value->second
                                     : curr_token.t_val);
    } else if (!curr_condition.oper.empty() && token_type == "value_key") {
      curr_condition.value.append(" " + curr_token.t_val);
    } else {
      throw vakya_error("Error in parsing new condition", curr_token.location);
    }
  }

  ops<ls_props<condition>> *parse_braces(std::string &&action_name) {
    if (auto new_token = this->advance_token();
        !(new_token.has_value() && new_token->t_type == TokenType::TT_SB))
      throw vakya_error("Wrong syntax for Condition / key Value pair",
                        new_token->location);
    ops<ls_props<condition>> *cdn_props = new ops<ls_props<condition>>();
    cdn_props->action_name = action_name;
    std::optional<std::vector<condition>> *curr_list =
        &cdn_props->action_props.should;
    std::optional<Tokens> new_token = this->advance_token();
    condition curr_condition = condition();
    auto validate_token = [&curr_condition]() -> bool {
      return ((!curr_condition.key.empty() && curr_condition.oper.empty() &&
               curr_condition.value.empty()) ||
              !(curr_condition.key.empty() || curr_condition.oper.empty() ||
                curr_condition.value.empty()));
    };
    auto get_condition = [&curr_condition, &new_token,
                          this](std::string &&token_type_var) -> void {
      parse_condition(curr_condition, new_token.value(), token_type_var);
    };
    while (new_token.has_value() && new_token->t_type != TokenType::TT_EB) {
      switch (new_token->t_type) {
      case TokenType::TT_EX: {
        if (auto peeked_token = this->peek_token();
            peek_token().has_value() &&
            user_values.find(peeked_token->t_type) != user_values.end())
          curr_list = &cdn_props->action_props.must;
        else
          get_condition("oper");
        break;
      }
      case TokenType::TT_QM: {
        curr_list = &cdn_props->action_props.could;
        break;
      }
      case TokenType::TT_EOL:
        if (!validate_token())
          break;
        [[fallthrough]];
      case TokenType::TT_CM: {
        if (validate_token()) {
          if (!curr_list->has_value()) {
            curr_list->emplace();
          }
          curr_list->value().push_back(std::move(curr_condition));
          curr_condition = condition();
        }
        curr_list = &cdn_props->action_props.should;
        break;
      }
      case TokenType::TT_STR:
      case TokenType::TT_ATTR:
      case TokenType::TT_USR: {
        get_condition("value_key");
        break;
      }
      case TokenType::TT_EQ:
      case TokenType::TT_GT:
      case TokenType::TT_LT: {
        if (action_name != "Conditions")
          throw vakya_error("Operator only allowed in cdn block",
                            new_token->location);
        get_condition("oper");
        break;
      }
      case TokenType::TT_CL: {
        if (action_name == "Conditions")
          throw vakya_error("Key : Value pairs not allowed in Condition block",
                            new_token->location);
        get_condition("isto");
        break;
      }
      default:
        throw vakya_error("Error in parsing braces", new_token->location);
      }
      new_token = this->advance_token();
    }
    if (validate_token()) {
      if (!curr_list->has_value())
        curr_list->emplace();
      curr_list->value().push_back(std::move(curr_condition));
    } else {
      throw vakya_error(
          "Error with condition arrangement",
          new_token
              .value_or(Tokens(TokenType::TT_ILL, lexer.t_list.back().location))
              .location);
    }

    return cdn_props;
  }
  void parse_src() {
    if (get_curr_program().has_value()) {
      this->curr_program = this->get_curr_program().value();
      this->curr_program->src_token = this->parse_parenthesis("source");
    } else {
      throw vakya_error("No current Program", 0);
    }
  }
  void parse_fmt() {
    if (this->get_curr_program().has_value())
      this->curr_program = this->get_curr_program().value();
    else {
      throw vakya_error("No current program found for fmt", 0);
    }
    fmt_class *curr_fmt = new fmt_class();
    this->curr_program->fmt_token = curr_fmt;
    auto next_token = this->advance_token();
    while (next_token && next_token->t_type != TokenType::TT_EOL) {
      switch (next_token->t_type) {
      case TokenType::TT_TBL:
      case TokenType::TT_BL:
      case TokenType::TT_PAR:
      case TokenType::TT_USR: {
        curr_fmt->type = parse_parenthesis(next_token->t_val);
        break;
      }
      case TokenType::TT_PRP: {
        curr_fmt->order = parse_braces("Order Properties");
        break;
      }
      case TokenType::TT_META: {
        curr_fmt->meta = parse_braces("Meta");
        break;
      }
      default:
        throw vakya_error("Wrong token at fmt", next_token->location);
      }
      next_token = this->advance_token();
    }
  }
  void parse_cdn() {
    if (this->get_curr_program().has_value()) {
      this->curr_program = this->get_curr_program().value();
    } else {
      throw vakya_error("No Program found, start program with do", 0);
    }
    this->curr_program->cdn_token = this->parse_braces("Conditions");
  }

  void parse_do() {
    this->curr_program = new Program();
    ops<std::string> *new_do_token = new ops<std::string>();
    curr_program->do_token = new_do_token;
    curr_program->do_token->action_name = "do";
    auto new_token = this->advance_token();
    while (new_token && new_token->t_type != TokenType::TT_EOL) {
      switch (new_token->t_type) {
      case TokenType::TT_STR:
      case TokenType::TT_ATTR:
        curr_program->do_token->action_props.append(" " + new_token->t_val);
        break;
      default:
        throw vakya_error("Token expect after do", new_token->location);
      }
      new_token = this->advance_token();
    }

    this->program_steps.emplace_back(this->curr_program);
    this->curr_program = this->program_steps.back();
  }

  void parse_on() {

    ops<std::string> *new_on_token = new ops<std::string>();
    curr_program->on_token = new_on_token;
    curr_program->on_token->action_name = "on";
    auto new_token = this->advance_token();
    while (new_token && new_token->t_type != TokenType::TT_EOL) {
      switch (new_token->t_type) {
      case TokenType::TT_STR:
      case TokenType::TT_ATTR:
        curr_program->on_token->action_props.append(" " + new_token->t_val);
        break;
      default:
        throw vakya_error("Token expect after on", new_token->location);
      }

      new_token = this->advance_token();
    }
  }

public:
  AST(Lexer &lexer_) : lexer(lexer_) {}
  void start_compiler() {
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
      case TokenType::TT_FMT: {
        parse_fmt();
        break;
      }
      case TokenType::TT_CDN: {
        parse_cdn();
        break;
      }
      case TokenType::TT_STRICT: {
        if (this->get_curr_program().has_value())
          this->get_curr_program().value()->strict = true;
        else
          throw vakya_error("Strict is an ending command, used without do :",
                            curr_token->location);
      }
      case TokenType::TT_EOL: {
        break;
      }
      default: {
        throw vakya_error("Unexpected token at root level",
                          curr_token->location);
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

  AST ast(lexer);
  try {
    ast.start_compiler();
    ast.print_programs();
  } catch (vakya_error &ve) {
    std::cout << ve.what() << "\n";
  }
  return 0;
}

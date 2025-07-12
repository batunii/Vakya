#include "Vakya_Lexer.hpp"
#include "Token_Utils.hpp"
#include "Vakya_Error.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

Tokens::Tokens() {}
Tokens::Tokens(TokenType t_type, size_t location, const std::string &t_val)
    : t_type(t_type), location(location), t_val(t_val) {}
bool Tokens::operator==(const Tokens &other) const {
  return this->t_type == other.t_type && this->t_val == other.t_val;
}

Lexer::Lexer(std::string &code_i)
    : code(code_i), t_list(std::vector<Tokens>()), curr_char('\0'), next_pos(0),
      prev_token() {}
char Lexer::advance() {
  if (this->next_pos < code.length())
    this->curr_char = this->code[this->next_pos++];
  else
    this->curr_char = '\0';
  if (this->t_list.size() > 0)
    prev_token = this->t_list[this->t_list.size() - 1];
  return this->curr_char;
}

void Lexer::handle_quotes() {
  std::string args = "";
  while (this->curr_char && this->curr_char != symbol(TokenType::TT_QT)) {
    args += this->curr_char;
    this->advance();
  }
  this->t_list.emplace_back(TokenType::TT_STR, this->next_pos - (args.length()),
                            args);
}
bool Lexer::is_keyword_or_usr(std::string &given_word) {
  auto it = keywords.find(given_word);
  if (it != keywords.end() && this->prev_token.t_type == TokenType::TT_AT) {
    this->t_list.pop_back();
    this->t_list.emplace_back(
        it->second, this->next_pos - (given_word.length()) - 1, given_word);
    return true;
  } else if (this->prev_token.t_type == TokenType::TT_HH) {
    this->t_list.pop_back();
    this->t_list.emplace_back(TokenType::TT_USR,
                              this->next_pos - (given_word.length()) - 1,
                              given_word);
    return true;
  } else if (this->prev_token.t_type == TokenType::TT_AT &&
             it == keywords.end()) {
		throw vakya_error(given_word+" is not a recognized system keyword", this->prev_token.location);
  }
  return false;
}

void Lexer::handle_string() {
  std::string attr = "";
  while (this->curr_char &&
         (std::isalnum(this->curr_char) || this->curr_char == '_')) {
    attr += this->curr_char;
    this->advance();
  }
  if (!this->is_keyword_or_usr(attr))
    this->t_list.emplace_back(TokenType::TT_ATTR,
                              this->next_pos - (attr.length()), attr);

  this->next_pos--;
}

std::vector<Tokens> &Lexer::make_tokens() {
  while (this->advance()) {
    switch (this->curr_char) {
    case '@':
      this->t_list.emplace_back(TokenType::TT_AT, this->next_pos - 1);
      break;
    case '#':
      this->t_list.emplace_back(TokenType::TT_HH, this->next_pos - 1);
      break;
    case '(':
      this->t_list.emplace_back(TokenType::TT_LP, this->next_pos - 1);
      break;
    case ')':
      this->t_list.emplace_back(TokenType::TT_RP, this->next_pos - 1);
      break;
    case '<':
      this->t_list.emplace_back(TokenType::TT_LT, this->next_pos - 1);
      break;
    case '>':
      this->t_list.emplace_back(TokenType::TT_GT, this->next_pos - 1);
      break;
    case '[':
      this->t_list.emplace_back(TokenType::TT_LB, this->next_pos - 1);
      break;
    case ']':
      this->t_list.emplace_back(TokenType::TT_RB, this->next_pos - 1);
      break;
    case '{':
      this->t_list.emplace_back(TokenType::TT_SB, this->next_pos - 1);
      break;
    case '}':
      this->t_list.emplace_back(TokenType::TT_EB, this->next_pos - 1);
      break;
    case '?':
      this->t_list.emplace_back(TokenType::TT_QM, this->next_pos - 1);
      break;
    case '!':
      this->t_list.emplace_back(TokenType::TT_EX, this->next_pos - 1);
      break;
    case ':':
      this->t_list.emplace_back(TokenType::TT_CL, this->next_pos - 1);
      break;
    case ';':
      if (this->advance() == ';')
        this->t_list.emplace_back(TokenType::TT_EOP, this->next_pos - 2);
      else {
        this->t_list.emplace_back(TokenType::TT_SC, this->next_pos - 1);
        this->next_pos--;
      }
      break;
    case '-': {
      if (this->advance() == '>') {
        this->t_list.emplace_back(TokenType::TT_NXT, this->next_pos - 2);
      } else {
        this->t_list.emplace_back(TokenType::TT_DH, this->next_pos - 1);
        this->next_pos--;
      }
      break;
    }
    case ',':
      this->t_list.emplace_back(TokenType::TT_CM, this->next_pos - 1);
      break;
    case '=':
      this->t_list.emplace_back(TokenType::TT_EQ, this->next_pos - 1);
      break;
    case '"':
      this->advance();
      this->handle_quotes();
      break;
    case '\n':
      this->t_list.emplace_back(TokenType::TT_EOL, this->next_pos - 1);
      break;
    default: {
      if (std::isalnum(this->curr_char))
        handle_string();
      break;
    }
    }
  }
  return this->t_list;
}

std::ostream &operator<<(std::ostream &os, Tokens &token) {

  if (token.t_val.empty())
    os << token.t_type << "{" << token.location << "}";
  else
    os << token.t_type << ":" << token.t_val << "{" << token.location << "}";
  return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<Tokens> &list) {
  if (list.empty())
    os << "The list is empty \n";
  else {
    os << "[";
    for (auto &token : list) {
      os << token << " ";
    }
    os << " ]";
  }
  return os;
}

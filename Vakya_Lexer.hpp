#ifndef LEXER_H
#define LEXER_H

#include "Token_Utils.hpp"
#include <array>
#define MAX_TOKENS 1024

class Tokens {
public:
  TokenType t_type;
  size_t location = 0;
  std::string t_val;
  Tokens();
  Tokens(TokenType, size_t, const std::string & = "");
  bool operator==(const Tokens &) const;
};

class Lexer {
public:
  std::string code;
  std::array<Tokens,MAX_TOKENS> t_list;
  char curr_char;
  size_t next_pos;
  Tokens prev_token;
	size_t curr_token_pos=0;
  Lexer(std::string &);
  Lexer(const char *);
  char advance();
  void handle_quotes();
  bool is_keyword_or_usr(std::string &);
  void handle_string();
  std::array<Tokens,MAX_TOKENS> &make_tokens();
};

std::ostream &operator<<(std::ostream &, Tokens &);
std::ostream &operator<<(std::ostream &, std::array<Tokens,MAX_TOKENS> &);

#endif

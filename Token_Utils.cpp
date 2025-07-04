#include "Token_Utils.hpp"
#include <string>
#include <unordered_map>
// Keyword token map (reserved words)
const std::unordered_map<std::string, TokenType> keywords = {
    {"do", TokenType::TT_DO},     {"on", TokenType::TT_ON},
    {"fmt", TokenType::TT_FMT},   {"cdn", TokenType::TT_CDN},
    {"table", TokenType::TT_TBL}, {"asc", TokenType::TT_ASC},
    {"dsc", TokenType::TT_DSC},   {"grp", TokenType::TT_GRP},
    {"src", TokenType::TT_SRC}};

std::string toString(TokenType type) {
  switch (type) {
  // Symbols
  case TokenType::TT_AT:
    return "TT_AT";
  case TokenType::TT_HH:
    return "TT_HH";
  case TokenType::TT_LP:
    return "TT_LP";
  case TokenType::TT_RP:
    return "TT_RP";
  case TokenType::TT_LT:
    return "TT_LT";
  case TokenType::TT_GT:
    return "TT_GT";
  case TokenType::TT_DH:
    return "TT_DH";
  case TokenType::TT_QT:
    return "TT_QT";
  case TokenType::TT_LB:
    return "TT_LB";
  case TokenType::TT_RB:
    return "TT_RB";
  case TokenType::TT_QM:
    return "TT_QM";
  case TokenType::TT_EX:
    return "TT_EX";
  case TokenType::TT_CL:
    return "TT_CL";
  case TokenType::TT_SC:
    return "TT_SC";
  case TokenType::TT_CM:
    return "TT_CM";
  case TokenType::TT_SB:
    return "TT_SB";
  case TokenType::TT_EB:
    return "TT_EB";
  case TokenType::TT_EQ:
    return "TT_EQ";
  case TokenType::TT_EOL:
    return "TT_EOL";
  case TokenType::TT_NXT:
    return "TT_NXT";
  case TokenType::TT_EOP:
    return "TT_EOP";

  // Keywords
  case TokenType::TT_DO:
    return "TT_DO";
  case TokenType::TT_ON:
    return "TT_ON";
  case TokenType::TT_FMT:
    return "TT_FMT";
  case TokenType::TT_CDN:
    return "TT_CDN";
  case TokenType::TT_TBL:
    return "TT_TBL";
  case TokenType::TT_ASC:
    return "TT_ASC";
  case TokenType::TT_DSC:
    return "TT_DSC";
  case TokenType::TT_GRP:
    return "TT_GRP";
  case TokenType::TT_ATTR:
    return "TT_ATTR";
  case TokenType::TT_STR:
    return "TT_STR";
  case TokenType::TT_USR:
    return "TT_USR";
  case TokenType::TT_PAR:
    return "TT_PAR";
  case TokenType::TT_BL:
    return "TT_BL";
  case TokenType::TT_LST:
    return "TT_LST";
  case TokenType::TT_SRC:
    return "TT_SRC";
  default:
    return "ILLEGAL";
  }
}

char symbol(TokenType type) {
  switch (type) {
  case TokenType::TT_AT:
    return '@';
  case TokenType::TT_HH:
    return '#';
  case TokenType::TT_LP:
    return '(';
  case TokenType::TT_RP:
    return ')';
  case TokenType::TT_LT:
    return '<';
  case TokenType::TT_GT:
    return '>';
  case TokenType::TT_DH:
    return '$';
  case TokenType::TT_QT:
    return '"';
  case TokenType::TT_LB:
    return '[';
  case TokenType::TT_RB:
    return ']';
  case TokenType::TT_QM:
    return '?';
  case TokenType::TT_EX:
    return '!';
  case TokenType::TT_CL:
    return ':';
  case TokenType::TT_SC:
    return ';';
  case TokenType::TT_SB:
    return '{';
  case TokenType::TT_EB:
    return '}';
  case TokenType::TT_EQ:
    return '=';
  case TokenType::TT_CM:
    return ',';
  case TokenType::TT_EOL:
    return '\n';
  default:
    return '\0';
  }
}

// Overload ostream << for TokenType
std::ostream &operator<<(std::ostream &os, TokenType type) {
  return os << toString(type);
}

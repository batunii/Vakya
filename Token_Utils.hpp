
#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <string>
#include <unordered_map>

enum class TokenType {
  TT_ILL,
  // Symbols
  TT_AT,
  TT_HH,
  TT_LP,
  TT_RP,
  TT_LT,
  TT_GT,
  TT_DH,
  TT_QT,
  TT_LB,
  TT_RB,
  TT_QM,
  TT_EX,
  TT_CL,
  TT_SC,
  TT_SB,
  TT_EB,
  TT_EQ,
  TT_CM,
  TT_NXT,
  TT_EOL,
  TT_EOP,

  // Keywords
  TT_DO,
  TT_ON,
  TT_FMT,
  TT_CDN,
  TT_TBL,
  TT_PAR,
  TT_BL,
  TT_LST,
  TT_ASC,
  TT_DSC,
  TT_GRP,
  TT_ATTR,
  TT_STR,
  TT_USR,
  TT_SRC,
	TT_PRP,
	TT_META,
	TT_STRICT
};

// Keyword token map (reserved words)
extern const std::unordered_map<std::string, TokenType> keywords;
extern const std::unordered_map<TokenType, std::string> operators_map;
extern const std::unordered_map<std::string, std::string> isto_operators;
// Convert TokenType to string name (e.g., TT_AT -> "TT_AT")
std::string toString(TokenType type);

// Convert TokenType to its symbol (e.g., TT_AT -> "@")
char symbol(TokenType type);
std::ostream &operator<<(std::ostream &os, TokenType type);

#endif // TOKEN_UTILS_H

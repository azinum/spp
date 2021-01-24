// token.h

#ifndef _TOKEN_H
#define _TOKEN_H

#include "common.h"

// NOTE(lucas): Most of these token types are not used in the parsing stage, but I'll keep them here in case one would like to use them in the future.
enum Token_type {
  T_UNKNOWN,

  // Arithmetic operators
  T_ASSIGN,         // '='
  T_ADD,
  T_SUB,
  T_MULT,
  T_DIV,
  T_INCREMENT,  // '++'
  T_DECREMENT,  // '--'
  T_MOD,  // '%'

  // Comparison operatos
  T_EQ,   // '=='
  T_NEQ,  // '!='
  T_LT,   // '<'
  T_GT,   // '>'
  T_LEQ,  // '<='
  T_GEQ,  // '>='

  // Bitwise operators
  T_BAND,   // Bitwise '&'
  T_BOR,    // '|'
  T_BXOR,   // '^'
  T_BNOT,   // '~'
  T_BLEFTSHIFT,  // '<<'
  T_BRIGHTSHIFT, // '>>'

  // Logical operators
  T_NOT,  // '!'
  T_AND,  // '&&'
  T_OR,   // '||'

  // Compound assignment operators
  T_ADD_ASSIGN,
  T_SUB_ASSIGN,
  T_MULT_ASSIGN,
  T_DIV_ASSIGN,
  T_MOD_ASSIGN,    // '%='
  T_BAND_ASSIGN,   // '&='
  T_BOR_ASSIGN,    // '|='
  T_BXOR_ASSIGN,   // '^='
  T_BLEFTSHIFT_ASSIGN,  // '<<='
  T_BRIGHTSHIFT_ASSIGN, // '>>='

  T_OPENPAREN,      // '('
  T_CLOSEDPAREN,    // ')'
  T_OPENBRACKET,    // '['
  T_CLOSEDBRACKET,  // ']'
  T_BLOCKBEGIN,     // '{'
  T_BLOCKEND,       // '}'
  T_SEMICOLON,      // ';'
  T_COLON,          // ':'
  T_COMMA,          // ','
  T_DOT,            // '.'
  T_ARROW,          // '->'
  T_DOLLAR,         // '$'

  T_NEWLINE,
  T_MACRO,

  T_LAMBDA_BODY,
  T_LAMBDA,
  T_ARGLIST,
  T_RETURN_TYPE,
  T_BODY,

  T_STRING,
  T_CHAR,
  T_SYMBOL, // Any other symbol (keyword, identifier, number e.t.c.)
  T_EOF,
};

struct Token {
  char* string;
  i32 length;
  i32 type;

  i32 id; // TODO(lucas): Temporary
};

void token_print(FILE* file, struct Token token);

void token_printline(FILE* file, struct Token token);

#endif

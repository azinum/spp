// lexer.h

#ifndef _LEXER_H
#define _LEXER_H

#include "token.h"

typedef struct Lexer {
  char* index;
  i32 line;
  i32 count;
  struct Token token;
  const char* filename;
} Lexer;

void lexer_init(Lexer* l, char* input, const char* filename);

struct Token next_token(Lexer* l);

struct Token get_token(Lexer* l);

#endif

// parser.h

#ifndef _PARSER_H
#define _PARSER_H

typedef struct Parser {
  struct Lexer* l;
  Ast* ast;
  Ast lambda_branch;
  i32 lambda_id_counter;
  i32 status;
} Parser;

i32 parser_parse(char* input, char* filename, Ast* ast);

#endif

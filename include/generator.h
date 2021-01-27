// generator.h

#ifndef _GENERATOR_H
#define _GENERATOR_H

#define TAB_SIZE 2

typedef struct Generator {
  FILE* file;
  char* source;
  i32 source_size;
} Generator;

i32 generator_init(Generator* g, FILE* file, char* source, i32 source_size);

i32 generate_from_ast(Generator* g, Ast* ast);

#endif

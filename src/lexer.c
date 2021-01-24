// lexer.c

#include "common.h"
#include "lexer.h"

#define lexer_error(fmt, ...) \
  fprintf(stderr, "[ERROR]: %s:%i:%i: " fmt, l->filename, l->line, l->count, ##__VA_ARGS__)

static i32 is_alpha(char ch);
static i32 is_number(char ch);
static i32 end_of_line(Lexer* l);
static struct Token read_symbol(Lexer* l);
static void next(Lexer* l);

i32 is_alpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

i32 is_number(char ch) {
  return ch >= '0' && ch <= '9';
}

i32 end_of_line(Lexer* l) {
  return *l->index == '\n' || *l->index == '\r';
}

struct Token read_symbol(Lexer* l) {
  while (
    is_alpha(*l->index) ||
    is_number(*l->index) ||
    *l->index == '_'
  ) {
    l->index++;
    l->count++;
  }
  l->token.length = l->index - l->token.string;
  l->token.type = T_SYMBOL;
  return l->token;
}

void next(Lexer* l) {
  l->token.string = l->index++;
  l->token.length = 1;
  l->count++;
}

void lexer_init(Lexer* l, char* input, const char* filename) {
  assert(input);
  l->index = &input[0];
  l->line = 0;
  l->count = 1;
  l->token = (struct Token) { .type = T_EOF };
  l->filename = filename;
}

struct Token next_token(Lexer* l) {
  for (;;) {
begin_loop:
    next(l);
    char ch = *l->token.string;
    switch (ch) {
      case '\n':
      case '\r':
        l->line++;
        l->count = 1;
        l->token.type = T_NEWLINE;
        return l->token;

      case ' ':
      case '\t':
      case '\v':
      case '\f':
        break;

      case '#':
        while (*l->index != '\0') {
          l->index++;
          l->count++;
          if (*l->index == '\\') {
            l->index++;
            l->line++;
            l->count = 1;
          }
          else if (end_of_line(l)) {
            l->line++;
            l->count = 1;
            break;
          }
          l->index++;
          l->count++;
        }
        l->token.length = l->index - l->token.string;
        l->token.type = T_MACRO;
        return l->token;

      case '=':
        l->token.type = T_ASSIGN;
        if (*l->index == '=') {
          l->token.type = T_EQ;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '+':
        l->token.type = T_ADD;
        if (*l->index == '=') {
          l->token.type = T_ADD_ASSIGN;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '+') {
          l->token.type = T_INCREMENT;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '-':
        l->token.type = T_SUB;
        if (*l->index == '=') {
          l->token.type = T_SUB_ASSIGN;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '-') {
          l->token.type = T_DECREMENT;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '>') {
          l->token.type = T_ARROW;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '*':
        l->token.type = T_MULT;
        if (*l->index == '=') {
          l->token.type = T_MULT_ASSIGN;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '/':
        if (*l->index == '/') {
          next(l);
          while (!end_of_line(l) && *l->index != '\0') {
            next(l);
          }
          l->line++;
          l->count = 1;
          next(l);
          break;
        }
        else if (*l->index == '*') {
          next(l);
          while (*l->index != '\0') {
            if (*l->index == '*') {
              next(l);
              if (*l->index == '/') {
                next(l);
                goto begin_loop;
              }
            }
            else if (end_of_line(l)) {
              l->line++;
              l->count = 1;
              next(l);
            }
            else {
              next(l);
            }
          }
          return l->token;
        }
        else {
          l->token.type = T_DIV;
          if (*l->index == '=') {
            l->token.type = T_DIV_ASSIGN;
            l->token.length++;
            l->index++;
          }
        }
        return l->token;
      case '%':
        l->token.type = T_MOD;
        if (*l->index == '=') {
          l->token.type = T_MOD_ASSIGN;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '<':
        l->token.type = T_LT;
        if (*l->index == '=') {
          l->token.type = T_LEQ;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '<') {
          l->token.type = T_BLEFTSHIFT;
          l->token.length++;
          l->index++;
          if (*l->index == '=') {
            l->token.type = T_BLEFTSHIFT_ASSIGN;
            l->token.length++;
            l->index++;
          }
        }
        return l->token;
      case '>':
        l->token.type = T_GT;
        if (*l->index == '=') {
          l->token.type = T_GEQ;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '>') {
          l->token.type = T_BRIGHTSHIFT;
          l->token.length++;
          l->index++;
          if (*l->index == '=') {
            l->token.type = T_BRIGHTSHIFT_ASSIGN;
            l->token.length++;
            l->index++;
          }
        }
        return l->token;
      case '&':
        l->token.type = T_BAND;
        if (*l->index == '=') {
          l->token.type = T_BAND_ASSIGN;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '&') {
          l->token.type = T_AND;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '|':
        l->token.type = T_BOR;
        if (*l->index == '=') {
          l->token.type = T_BOR_ASSIGN;
          l->token.length++;
          l->index++;
        }
        else if (*l->index == '|') {
          l->token.type = T_OR;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '^':
        l->token.type = T_BXOR;
        if (*l->index == '=') {
          l->token.type = T_BXOR_ASSIGN;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '~':
        l->token.type = T_BNOT;
        return l->token;
      case '!':
        l->token.type = T_NOT;
        if (*l->index == '=') {
          l->token.type = T_NEQ;
          l->token.length++;
          l->index++;
        }
        return l->token;
      case '(':
        l->token.type = T_OPENPAREN;
        return l->token;
      case ')':
        l->token.type = T_CLOSEDPAREN;
        return l->token;
      case '[':
        l->token.type = T_OPENBRACKET;
        return l->token;
      case ']':
        l->token.type = T_CLOSEDBRACKET;
        return l->token;
      case '{':
        l->token.type = T_BLOCKBEGIN;
        return l->token;
      case '}':
        l->token.type = T_BLOCKEND;
        return l->token;
      case ';':
        l->token.type = T_SEMICOLON;
        return l->token;
      case ':':
        l->token.type = T_COLON;
        return l->token;
      case ',':
        l->token.type = T_COMMA;
        return l->token;
      case '.':
        l->token.type = T_DOT;
        return l->token;
      case '$':
        l->token.type = T_DOLLAR;
        return l->token;

      case '\'':
        l->token.type = T_CHAR;
        l->token.length++;
        l->index++;
        if (*l->index != '\'') {
          lexer_error("Unfinished or invalid char symbol\n");
          l->token.type = T_EOF;
          return l->token;
        }
        l->token.length++;
        l->index++;
        return l->token;

      case '"':
        for (;;) {
          if (*l->index == '\0') {
            lexer_error("Unfinished string\n");
            l->token.type = T_EOF;
            return l->token;
          }
          else if (*l->index == '"') {
            break;
          }
          l->index++;
          l->count++;
        }
        l->index++;
        l->count++;
        l->token.type = T_STRING;
        l->token.length = l->index - l->token.string;
        return l->token;

      case '\0':
        l->token.type = T_EOF;
        return l->token;

      default: {
#if 0
        if (is_number(ch) || is_alpha(ch) || ch == '_') {
          return read_symbol(l);
        }
        else {
          lexer_error("Unrecognized character\n");
          l->token.type = T_EOF;
          return l->token;
        }
        break;
#else
        return read_symbol(l);
#endif
      }
    }
  }
  return l->token;
}

struct Token get_token(Lexer* l) {
  return l->token;
}

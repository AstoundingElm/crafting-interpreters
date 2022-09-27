#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

struct Scanner{

  const b8 * start;
  const b8 * current;
  i32 line;

};

 enum Token_Type{
 // Single-character tokens
 TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
 TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
 TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
 TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
 // One or two character tokens.
 TOKEN_BANG, TOKEN_BANG_EQUAL,
 TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
 TOKEN_GREATER, TOKEN_GREATER_EQUAL,
 TOKEN_LESS, TOKEN_LESS_EQUAL,
 // Literals.
 TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
 // Keywords.
 TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
 TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
 TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
 TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
 TOKEN_ERROR,
 TOKEN_EOF
 };

struct Token{

  Token_Type type;
  const b8 * start;
  i32 length;
  i32 line;
};

static Scanner scanner;

PINLINE bool is_at_end(){

  return *scanner.current == '\0';
};

PINLINE Token makeToken(Token_Type type){

  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (i32)(scanner.current - scanner.start);
  token.line = scanner.line;
  return token;
  
};

PINLINE Token error_token(const b8 * message){

  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (i32)strlen(message);
  token.line - scanner.line;

  return token;

};

PINLINE b8 advance(){

  scanner.current++;
  return scanner.current[-1];
};

PINLINE bool match(b8 expected){

  if(is_at_end()) return false;
  if(*scanner.current != expected) return false;

  scanner.current++;
  return true;
};

PINLINE b8 peek(){

  retun *scanner.current;
};

PINLINE b8 peek_next(){

  if(is_at_end()) return '\0';
  return scanner.current[1];
}

PINLINE void skip_whitespace(){

  for(;;){

    char c = peek();

    switch(c){

    case ' ':
    case '\r':
    case '\t':
      advance();
      break;

    case: '\n':
      scanner.line++;
      advance();
      break;

    case '/':
      if(peek_next() == '/'){
	while(peek() != '\n' && !is_at_end()) advance();
	
      } else{
	return;
      }
      break;
    default: return;
    }
    
  }
  
};

PINLINE Token string(){

  while(peek() != '"' && !is_at_end()){

    if(peek() == '\n')scanner.line++;
    advance();
  }
  if(is_at_end())return error_token("unterminated string");
  advance();
  return make_token(TOKEN_STRING);
};

PINLINE bool is_digit(b8 c){

  return c >= '0' && c <= '9';
}

PINLINE Token number(){

  while(is_digit(peek())) advance();

  if (peek() == '.' && is_digit(peek_next())){

    advance();

    while(is_digit(peek())) advance();

  }

  return make_token(TOKEN_NUMBER);

}

PINLINE bool is_alpha(char c){

  return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    c == '_';
}

PINLINE Token_Type check_keyword(i32 start, i32 length,
				 const b8 * rest, Token_Type type){

  if(scanner.current - scanner.start == start + length &&
     memcmp(scanner.start + start, rest, length) == 0){

    return type;
  }
  return TOKEN_IDENTIFIER;
  
}

PINLINE Token_Type identifier_type(){

   switch (scanner.start[0]) {
 case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
 case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
 case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
   case 'f':
 if (scanner.current - scanner.start > 1) {
 switch (scanner.start[1]) {
 case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
 case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
 case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
 }
 }
 break;
 case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
 case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
 case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
 case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
 case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
 case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
   case 't':
 if (scanner.current - scanner.start > 1) {
 switch (scanner.start[1]) {
 case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
 case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
 }
 }
 break;
 case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
 case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
 }
  return TOKEN_IDENTIFIER;
}

PINLINE Token identifier(){

  while (is_alpha(peek()) || is_digit(peek()) advance();
	 return make_token(identifier_type());
  
}

Token scan_token(){

  skip_whitespace();

  scanner.start = scanner.current;
  if(is_at_end()) return make_token(TOKEN_EOF);

  char c = advance();

  if(is_alpha(c)) return identifier();
  if(is_digit(c)) return number();
  switch (c){

  case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
 case '{': return makeToken(TOKEN_LEFT_BRACE);
 case '}': return makeToken(TOKEN_RIGHT_BRACE);
 case ';': return makeToken(TOKEN_SEMICOLON);
 case ',': return makeToken(TOKEN_COMMA);
 case '.': return makeToken(TOKEN_DOT);
 case '-': return makeToken(TOKEN_MINUS);
 case '+': return makeToken(TOKEN_PLUS);
 case '/': return makeToken(TOKEN_SLASH);
 case '*': return makeToken(TOKEN_STAR);
   case '*': return makeToken(TOKEN_STAR);
 case '!':
 return makeToken(
 match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
 case '=':
 return makeToken(
 match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
 case '<':
 return makeToken(
 match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
 case '>':
 return makeToken(
 match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  case '"': return string();
  };
  return error_token("unexpected character");

};



PINLINE void init_scanner(const b8 * source){

  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
};

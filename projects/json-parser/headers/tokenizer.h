//
// Created by Elijah Wandimi on 05/04/2026.
//

#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
    TOKEN_LBRACE,    // {
    TOKEN_RBRACE,    // }
    TOKEN_LBRACKET,  // [
    TOKEN_RBRACKET,  // ]
    TOKEN_STRING,    // "hello"
    TOKEN_NUMBER,    // 42, 3.14
    TOKEN_TRUE,      // true
    TOKEN_FALSE,     // false
    TOKEN_NULL,      // null
    TOKEN_COLON,     // :
    TOKEN_COMMA,     // ,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;  // Points into source string
    size_t length;
} Token;

// Uses arena for token storage
Token* tokenize(Arena *arena, const char *source, size_t *token_count);

#endif //TOKENIZER_H

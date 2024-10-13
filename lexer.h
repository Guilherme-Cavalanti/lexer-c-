#include <string.h>

typedef enum {
    TOKEN_EOF,
    ID,
    NUMBER,
    WHITESPACE,
    SEPARATOR,
    KEYWORD,
    OPERATOR,
    RELOP,
    COMMENT,
    TOKEN_ERROR
} token_type;

typedef struct {
    token_type type;    // O tipo do token (identificador, constante, operador, etc.)
    char* lexeme;      // O lexema do token (o texto real que foi lido, por exemplo, "x", "42")
    int lineNumber;    // Número da linha onde o token foi encontrado (para ajudar na geração de mensagens de erro)
    char* attribute;
} Token;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lexer.c"

int main(int argc,  char* argv[]){
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        error("Error: Could not open file.");
    }

        printf("File opened successfully: %s\n", argv[1]);

    int lineNumber = 1;  // Inicia a contagem de linhas
    Token token;

    // Loop para ler tokens do arquivo até encontrar EOF
    do {
        token = getNextToken(file, &lineNumber);
        printf("Token: %s, Line: %d, Atributo: %s Tipo: %i \n", token.lexeme, token.lineNumber, token.attribute, token.type);
    } while (token.type != TOKEN_EOF);
    free(token.lexeme);  // Libera a memória alocada para o lexema
    free(token.attribute);
    // Fecha o arquivo ao final
    fclose(file);
    return 0;
}
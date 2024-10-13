#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "./lexer.h"

void error(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

//FUNÇÂO PARA RELOPS
Token getRelop(FILE *file){
    Token retToken;
    int state = 0;
    char c;
    while(1) { 
        /* repeat character processing until a return or failure occurs */
        switch(state) {
            case 0: c = fgetc(file);
                    if ( c == '<') state = 1;
                    else if( c == '=') state = 4;
                    else if( c == '>') state = 7;
                    else if(c == '!') state = 10;
                    break;
            //Símbolo de menor
            case 1: c = fgetc(file);
                    if (c == '=') state = 2;
                    else state = 3;
                    break;
            // Menor ou igual
            case 2: retToken.lexeme = "<=";
                    retToken.attribute = "LE";
                    retToken.type = RELOP;
                    return retToken;
            // Menor que
            case 3: if (c != EOF) fseek(file, -1, SEEK_CUR);
                    retToken.type = RELOP;
                    retToken.lexeme = "<";
                    retToken.attribute = "LT";
                    return retToken;
            // Simbolo de igual
            case 4: c=fgetc(file);
                    if (c == '=') state = 5;
                    else state = 6;
                    break;
            // Igualdade
            case 5: retToken.type = RELOP;
                    retToken.lexeme = "==";
                    retToken.attribute = "EQ";
                    return retToken;
            // Atribuição
            case 6: if (c != EOF) fseek(file, -1, SEEK_CUR);
                    retToken.type = RELOP;
                    retToken.lexeme = "=";
                    retToken.attribute = "E";
                    return retToken;
            //Simbolo de maior
            case 7: c = fgetc(file);
                    if (c == '=') state = 8;
                    else state = 9; 
                    break;
              // Maior ou igual
            case 8: retToken.lexeme = ">=";
                    retToken.attribute = "GE";
                    retToken.type = RELOP;
                    return retToken;
            // Maior que
            case 9: if (c != EOF) fseek(file, -1, SEEK_CUR);
                    retToken.type = RELOP;
                    retToken.lexeme = ">";
                    retToken.attribute = "GT";
                    return retToken;
            //Diferente
            case 10:c = fgetc(file);
                    if(c == '='){
                        retToken.type = RELOP;
                        retToken.lexeme = "!=";
                        retToken.attribute = "NE";
                        return(retToken);
                    }
                    if (c != EOF) fseek(file, -1, SEEK_CUR);
                    retToken.type = TOKEN_ERROR;
                    retToken.lexeme = "!";
                    retToken.attribute = "Erro";
                    return(retToken);
        }
    }
} 
// Quando achou uma letra, encontrar o token com o identificador completo
Token getID(FILE *file, char first){
    Token retToken;
    char c;
    char* id;
    id = strdup((char[]){first,'\0'});
    while(1){
        c = fgetc(file);
        if(isalpha(c) || isdigit(c)){
            //id = strdup((char[]){id,c});
            id = strcat(id, (char[]){c,'\0'});
        }
        else {
            if(c != EOF) fseek(file, -1, SEEK_CUR);
            break;
        }
    }
    retToken.lexeme = id;
    retToken.attribute = "tabela";
    retToken.type = ID;
    return retToken;
}
// Quando achou um digito, encontrar o token com o número completo
Token getNumber(FILE *file, char first){
    Token retToken;
    char c;
    char* number;
    number = strdup((char[]){first,'\0'});
    while(1){
        c = fgetc(file);
        if(isdigit(c)){
            number = strcat(number, (char[]){c,'\0'});
        }
        else {
            if(c != EOF) fseek(file, -1, SEEK_CUR);
            break;
        }
    }
    retToken.lexeme = number;
    retToken.attribute = "sla";
    retToken.type = NUMBER;
    return retToken;
}

//PULANDO COMENTARIOS
void SkipComment(FILE *file){
    char c;
    while (1) {
        c = fgetc(file);
        if (c == '*') {
            c = fgetc(file);
            if (c == '/') return;
        }
        if (c == EOF) return;
    }
}

//PEGANDO OPERADORES
Token getOp(char op){
    Token retToken;
    retToken.lexeme = strdup((char[]){op,'\0'});
    retToken.type = OPERATOR;
    switch(op){
        case '+': 
                retToken.attribute = "plus";
                break;
        case '-': 
                retToken.attribute = "minus";
                break;
        case'*':
                retToken.attribute = "multiplication";
                break;
        case'/':
                retToken.attribute = "division";
                break;
    }
    return retToken;
}

//PEGANDO SEPARADORES
Token getSeparator(char sep){
    Token retToken;
    retToken.lexeme = strdup((char[]){sep,'\0'});
    retToken.type = SEPARATOR;
    switch(sep){
        case ';': 
                retToken.attribute = "semicolon";
                break;
        case ',': 
                retToken.attribute = "comma";
                break;
        case'(':
                retToken.attribute = "open_par";
                break;
        case')':
                retToken.attribute = "close_par";
                break;
        case'{':
                retToken.attribute = "open_braces";
                break;
        case'}':
                retToken.attribute = "close_braces";
                break;
        case'[':
                retToken.attribute = "open_brack";
                break;
        case']':
                retToken.attribute = "close_brack";
                break;
    }
    return retToken;
}

// Função para a leitura de tokens do arquivo
Token getNextToken(FILE *file, int *lineNumber) {
    Token token;
    char c = fgetc(file);

    //Pular comentário
    if (c == '/') {
        char n = fgetc(file);
        if (n == '*'){
            printf("Entrei no skip de comentario\n");
            SkipComment(file);
            c = fgetc(file);
            printf("Sai do skip de comentario\n");
        }
        else  if(n != EOF) fseek(file, -1, SEEK_CUR);
    }

    // Pegar EOF
    if (c == EOF) {
        token.type = TOKEN_EOF;
        token.lexeme = strdup("EOF");
        token.lineNumber = *lineNumber;
        return token;
    }
    
    // Pegar relop
    if (c == '<' || c == '>' || c == '=' || c == '!'){
        fseek(file, -1, SEEK_CUR);
        token = getRelop(file);
        token.lineNumber = *lineNumber;
        return token;
    }

    // Pegar ID
    if (isalpha(c)){
        token = getID(file, c);
        token.lineNumber = *lineNumber;
        return token;
    }

    // Pegar number
    if (isdigit(c)){
        token = getNumber(file, c);
        token.lineNumber = *lineNumber;
        return token;
    }

    //Pegar Operadores
    if (c == '/' || c == '*' || c == '+' || c == '-') {
        token = getOp(c);
        token.lineNumber = *lineNumber;
        return token;
    }

    //Pegar Separadores
    if (c == ';' || c == ',' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
        token = getSeparator(c);
        token.lineNumber = *lineNumber;
        return token;
    }

    // Lidando com whitespaces (último caso que sobrou)
    token.type = WHITESPACE;
    token.attribute = "";
     if (c != '\n') token.lexeme = strdup((char[]){c, '\0'});
    token.lineNumber = *lineNumber;
    // Incrementa o número da linha ao encontrar uma nova linha
    if (c == '\n') {
        (*lineNumber)++;
        token.lexeme = strdup("\\n");
    }
    return token;
}
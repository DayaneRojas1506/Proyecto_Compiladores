//
// Created by Jazmin on 4/11/2024.
//

#ifndef COMPILADORES_TOKEN_H
#define COMPILADORES_TOKEN_H


#include <string>

class Token {
public:
    enum Type {
        PLUS, MINUS, MUL, DIV, NUM, ERR, PD, PI, END, ID, PRINTF, ASSIGN, PC,LT, LE, EQ, IF, ELSE,WHILE,DO,COMA,IFEXP,
        FOR, TRUE, FALSE, RETURN,  LKEY, RKEY, INCLUDE, MICHI, PERCENT, POINT, GREATER, COMILLA, SALTO, PLUSPLUS, COMENTARIO, FORMAT, LIBRARY, TIPO
    };


    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};



#endif //COMPILADORES_TOKEN_H

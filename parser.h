//
// Created by Jazmin on 6/11/2024.
//

#ifndef COMPILADORES_PARSER_H
#define COMPILADORES_PARSER_H


#include "scanner.h"
#include "exp.h"

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    list<Stm*> parseStmList();
    Exp* parseCExp();
    Exp* parseExpression();
    Exp* parseTerm();
    Exp* parseFactor();
public:
    Parser(Scanner* scanner);
    Program* parseProgram();
    Stm* parseStatement();
    StatementList* parseStatementList();
    VarDec* parseVarDec();
    VarDecList* parseVarDecList();
    Body* parseBody();
    FunDecList* parseFunDecList();
    FunDec* parseFunDec();
    //New
    LibraDec* parseLibraDec();
    LibraDecList* parseLibraDecList();

};





#endif //COMPILADORES_PARSER_H

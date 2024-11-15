//
// Created by Jazmin on 6/11/2024.
//

#include "parser.h"


#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

LibraDec* Parser::parseLibraDec() {
    LibraDec* ld = NULL;
    //cout<<previous->text<<endl;
    if(!match(Token::MICHI)){
        cout<<previous->text<<endl;
        cout<<"Error: se esperaba # en la definicion de la libreria."<<endl;
        exit(1);
    }
    if(!match(Token::INCLUDE)){
        cout<<"Error: se esperaba INCLUDE despues de #. "<<endl;
        exit(1);
    }

    if(!match(Token::LT)){
        cout<<"Error: se esperaba < despues del INCLUDE. "<<endl;
        exit(1);
    }

    if(!match(Token::ID)){
        cout<<"Error: Se esperaba ID despues de <"<<endl;
        exit(1);
    }

    auto name = previous->text;

    if(!match(Token::POINT)){
        cout<<"Error: Se esperaba . despues de id"<<endl;
        exit(1);
    }

    if(!match(Token::ID)){
        cout<<"Error: Se esperaba id despues de ."<<endl;
        exit(1);
    }

    auto extension = previous->text;


    if(!match(Token::GREATER)){
        cout<<"Error: se esperaba > despues del  "<<endl;
        exit(1);
    }

    ld = new LibraDec(name, extension);
    return ld;

}

LibraDecList* Parser::parseLibraDecList(){
    LibraDecList* fdl = new LibraDecList();
    LibraDec* aux;
    aux = parseLibraDec();
    while (aux != NULL) {
        fdl->add(aux);
        aux = parseLibraDec();
    }
    return fdl;
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = NULL;
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después de 'var'." << endl;
        exit(1);
    }
    string type = previous->text;
    list<string> ids;
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después de 'var'." << endl;
        exit(1);
    }
    ids.push_back(previous->text);
    while (match(Token::COMA)) {
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de ','." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
    }
    if (!match(Token::PC)) {
        cout << "Error: se esperaba un ';' al final de la declaración." << endl;
        exit(1);
    }
    vd = new VarDec(type, ids);

    return vd;
}





VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();
    VarDec* aux;
    aux = parseVarDec();
    while (aux != NULL) {
        vdl->add(aux);
        aux = parseVarDec();
    }
    return vdl;
}

StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)) {
        sl->add(parseStatement());
    }
    return sl;
}


Body* Parser::parseBody() {
    VarDecList* vdl = parseVarDecList();
    StatementList* sl = parseStatementList();
    return new Body(vdl, sl);
}

FunDec* Parser::parseFunDec() {
    FunDec* fd = NULL;

    if (!match(Token::ID)) {
        cout << "Error: se esperaba un Tipo de funcion." << endl;
        exit(1);
    }
    string type = previous->text;

    if (!match(Token::ID)) {
        cout << "Error: se esperaba un identificador después de 'fun'." << endl;
        exit(1);
    }
    string name = previous->text; // Nombre de la funcion
    list<pair<string, string>> params; // Parametros

    if (!match(Token::PI)) {
        cout << "Error: se esperaba '(' después del identificador." << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string paramType = previous->text;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de '('." << endl;
            exit(1);
        }
        string paramName = previous->text;
        params.push_back(make_pair(paramType, paramName));

        while (match(Token::COMA)) {
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después de ','." << endl;
                exit(1);
            }
            string paramType = previous->text;
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después de ','." << endl;
                exit(1);
            }
            string paramName = previous->text;
            params.push_back(make_pair(paramType, paramName));
        }
    }

    if (!match(Token::PD)) {
        cout << "Error: se esperaba ')' después de los parámetros." << endl;
        exit(1);
    }


    if (!match(Token::LKEY)) {
        cout << "Error: se esperaba '{' después de los parámetros." << endl;
        exit(1);
    }

    Body* b = parseBody();

    if (!match(Token::RKEY)) {
        cout << "Error: se esperaba '}' al final de la declaración." << endl;
        exit(1);
    }

    fd = new FunDec(type, name, params, b);

    return fd;
}


FunDecList* Parser::parseFunDecList() {
    FunDecList* fdl = new FunDecList();
    FunDec* aux;
    aux = parseFunDec();
    while (aux != NULL) {
        fdl->add(aux);
        aux = parseFunDec();
    }
    return fdl;
}

Program* Parser::parseProgram() {
    LibraDecList* ldl = parseLibraDecList();
    VarDecList* vdl = parseVarDecList();
    FunDecList* fdl = parseFunDecList();
    return new Program(ldl,vdl, fdl);
}

list<Stm*> Parser::parseStmList() {
    list<Stm*> slist;
    slist.push_back(parseStatement());
    while(match(Token::PC)) {
        slist.push_back(parseStatement());
    }
    return slist;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case

    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text;

        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        e = parseCExp();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINTF)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }

        if (!match(Token::COMILLA)) {
            cout << "Error: se esperaba comilla después de '('." << endl;
            exit(1);
        }
        if (!match(Token::FORMAT)) {
            cout << "Error: se esperaba un especificador de formato." << endl;
            exit(1);
        }


        string format = previous->text;

        if (!match(Token::COMILLA)) {
            cout << "Error: se esperaba comilla después de FORMAT." << endl;
            exit(1);
        }

        if(!match(Token::COMA)) {
            cout << "Error: se esperaba una coma después de COMILLAS." << endl;
            exit(1);
        }


        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(format, e);
    }
    else if (match(Token::IF)) {
        if(!match(Token::PI)){
            cout << "Error: se esperaba '(' después de la expresión." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::LKEY)) {
            cout << "Error: se esperaba '{' después de ')." << endl;
            exit(1);
        }

        tb = parseBody();

        if (!match(Token::RKEY)) {
            cout << "Error: se esperaba '}' después del cuerpo." << endl;
            exit(1);
        }

        if (match(Token::ELSE)) {
            if (!match(Token::LKEY)) {
                cout << "Error: se esperaba '{' después de la expresion." << endl;
                exit(1);
            }
            fb = parseBody();
        }
        if (!match(Token::RKEY)) {
            cout << "Error: se esperaba '}' al final de la declaración." << endl;
            exit(1);
        }
        s = new IfStatement(e, tb, fb);

    }
    else if (match(Token::WHILE)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de la expresión." << endl;
            exit(1);
        }
        e = parseCExp();

        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::LKEY)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::RKEY)) {
            cout << "Error: se esperaba '}' al final de la declaración." << endl;
            exit(1);
        }
        s = new WhileStatement(e, tb);

    }
    else if(match(Token::FOR)){
        if(!match(Token::PI)){
            cout << "Error: se esperaba '(' después de 'for'." << endl;
            exit(1);
        }
        Exp* start = parseCExp();
        if (!match(Token::COMA)) {
            cout << "Error: se esperaba ',' después de la expresión." << endl;
            exit(1);
        }
        Exp* end = parseCExp();
        if (!match(Token::COMA)) {
            cout << "Error: se esperaba ',' después de la expresión." << endl;
            exit(1);
        }
        Exp* step = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::LKEY)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }

        tb = parseBody();
        if (!match(Token::RKEY)) {
            cout << "Error: se esperaba '}' al final de la declaración." << endl;
            exit(1);
        }
        s = new ForStatement(start, end, step, tb);
    }
    else if (match(Token::RETURN)){

        e = e = parseCExp();
        if (!match(Token::PC)){
            cout << "Error: se esperaba ';' después de la expresión." << endl;
            exit(1);

        }
        s = new ReturnStatement(e);
    }
    else {
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }
        else if (previous->type == Token::EQ){
            op = EQ_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    Exp* e1;
    Exp* e2;
    if (match(Token::TRUE)){
        return new BoolExp(1);
    }else if (match(Token::FALSE)){
        return new BoolExp(0);
    }
    else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::ID)) {
        string name = previous->text;
        if (match (Token::PI)){
            list<Exp*> args;
            if (!match(Token::PD)){
                args.push_back(parseCExp());
                while (match(Token::COMA)){
                    args.push_back(parseCExp());
                }
                if (!match(Token::PD)){
                    cout << "Falta paréntesis derecho" << endl;
                    exit(0);
                }
            }
            return new FCallExp(name, args);
        }
        else{
            return new IdentifierExp(previous->text);
        }
    }
    else if (match(Token::IFEXP)) {
        match(Token::PI);
        e=parseCExp();
        match(Token::COMA);
        e1=parseCExp();
        match(Token::COMA);
        e2=parseCExp();
        match(Token::PD);
        return new IFExp(e,e1,e2);
    }
    else if (match(Token::PI)){
        e = parseCExp();
        if (!match(Token::PD)){
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}
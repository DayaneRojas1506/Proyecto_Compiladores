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

bool Parser::matchnotAdvance(Token::Type ttype) {
    if (check(ttype)) {
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


VarDec* Parser::parseVarDec() {
    VarDec* vd = NULL;
    if (check(Token::TIPO)) {
        advance();
        string type = previous->text;
        if (!check(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'vaar'." << endl;
            exit(1);
        }

        list<string> ids;
        //cout<<current->text<<"TTT"<<endl;
        if (!match(Token::ID)) {
            //cout<<previous->text << endl;
            //cout<<current->text<<endl;
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
        while (check(Token::COMA)) {
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
    }
    //cout<<current->text<< "VBBBB"<<endl;
    return vd;
}




VarDecList* Parser::parseVarDecList() {

    VarDecList* vdl = new VarDecList();


    while (current != NULL && current->type == Token::TIPO) {
        VarDec* vd = parseVarDec();
        if (vd != NULL) {
            vdl->add(vd);
        }
    }

    return vdl;
}

StatementList* Parser::parseStatementList() {

    StatementList* sl = new StatementList();

    while (current != NULL && current->type != Token::RKEY) {  //Detenerse si encuentra '}' (fin de bloque)
        Stm* stm = parseStatement();
        if (stm != NULL) {
            sl->add(stm);
        }
    }

    return sl;
}


Body* Parser::parseBody() {
    VarDecList* vdl = NULL;
    StatementList* sl = NULL;
    vdl = parseVarDecList();
    sl = parseStatementList();

    return new Body(vdl, sl);
}


FunDec* Parser::parseFunDec() {
    FunDec* fd = NULL;
    if (match(Token::TIPO)) {
        Body* body = NULL;
        string rtype = previous->text;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un nombre de funcion." << endl;
            exit(1);
        }



        string fname = previous->text;
        if (!match(Token::PI)) {
            //cout<<previous->text<<endl;
            cout << "Error: se esperaba un '(' después del nombre de la función." << endl;
            exit(1);
        }
        list<string> types;
        list<string> vars;
        if (!check(Token::PD)) {
            if (!match(Token::TIPO)) {
                cout << "Error: se esperaba un tipo después del '('." << endl;
                exit(1);
            }
            types.push_back(previous->text);
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después del tipo." << endl;
                exit(1);
            }
            vars.push_back(previous->text);
            while(match(Token::COMA)) {
                if(!match(Token::TIPO)) {
                    cout << "Error: se esperaba un tipo después de ','." << endl;
                    exit(1);
                }
                types.push_back(previous->text);
                if (!match(Token::ID)) {
                    cout << "Error: se esperaba un identificador después del tipo." << endl;
                    exit(1);
                }
                vars.push_back(previous->text);
            }
        }
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
            exit(1);
        }

        if(!match(Token::LKEY)){
            cout<<"Error: se esperaba { dentro de la funcion fundec"<<endl;
            exit(1);
        }

        //cout<<current->text<<"A"<<endl;

        body = parseBody();

        //cout<<current->text<<"DDD"<<endl;
        if (!match(Token::RKEY)){
            //cout<<previous->text<<endl;
            //cout<<current->text<<endl;
            cout << "Error: se esperaba 'endfun' al final de la declaración." << endl;
            exit(1);
        }
        fd = new FunDec(fname, types, vars, rtype, body);
    }
    return fd;
}



FunDecList* Parser::parseFunDecList() {
    FunDecList* fdl = new FunDecList();
    FunDec* aux;
    aux = parseFunDec();
    while (aux != NULL ) {
        fdl->add(aux);
        aux = parseFunDec();
    }
    return fdl;
}

Program* Parser::parseProgram() {
    if(!match(Token::LIBRARY)){
        cout<<"Error: Se esperaba la cabecera de la libreria include"<<endl;
        exit(1);
    }
    //VarDecList* vdl = parseVarDecList();
    FunDecList* fdl = parseFunDecList();
    //cout<<"AAAAAAAAAA"<<endl;
    return new Program(fdl);
}



Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case
    string namefor;
    string tipofor;

    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    //if(true){cout<<current->text<<endl;}

    if (match(Token::ID)) {
        string lex = previous->text;


        if (!match(Token::ASSIGN)) {
            //cout<<previous->text<<endl;
            //cout<<current->text<<endl;
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }


        e = parseCExp();


        if(!match(Token::PC)){

            cout<<"Error: se esperaba un ; despues de parserCexp"<<endl;
            exit(1);
        }

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

        if (!match(Token::PC)) {
            cout << "Error: se esperaba un ';' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(format, e);
    }
    else if (match(Token::IF)) {
        //cout<<current->text<<"AAA"<<endl;
        if(!match(Token::PI)){
            //cout<<previous->text<<endl;
            cout << "Error: se esperaba '(' después de la expresión." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout<<previous->text<<endl;
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
            //cout<<previous->text<<"GG"<<endl;
            cout << "Error: se esperaba '(' después de 'for'." << endl;
            exit(1);
        }
        if(check(Token::TIPO)){
            tipofor = current->text;
            advance();
            namefor = current->text;
            if(!match(Token::ID)){
                cout<<"Error: se esperaba id despues de int del for"<<endl;
                exit(1);
            }

        }else if(check(Token::ID)){
            namefor = current->text;
            advance();
        }

        if(!match(Token::ASSIGN)){
            cout<<"Error: Se esperaba un = despues de id del for"<<endl;
        }

        Exp* start = parseCExp();
        if (!match(Token::PC)) {
            cout << "Error: se esperaba ';' después de la expresión." << endl;
            exit(1);
        }
        Exp* end = parseCExp();
        if (!match(Token::PC)) {
            cout << "Error: se esperaba ';' después de la expresión." << endl;
            exit(1);
        }
        Exp* step = parseUnaryFactor();
        if (!match(Token::PD)) {
            cout<<previous->text<<"GG"<<endl;
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
        s = new ForStatement(tipofor, namefor, start, end, step, tb);
    }
    else if (match(Token::RETURN)){

        e =  parseCExp();
        //if(true){ cout<<current->text<<endl; cout<<previous->text<<endl; exit(1);}
        if (!match(Token::PC)){
            //cout<<previous->text<<endl;
            cout << "Error: se esperaba ';' después de la expresión." << endl;
            exit(1);

        }
        s = new ReturnStatement(e);
    }
    else if(check(Token::COMENTARIO)){
        advance();
    }
    else {
        cout<<previous->text<<"A"<<endl;
        cout<<current->text<<endl;
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}



Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::GREATER) || match(Token::EQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }else if(previous->type == Token::GREATER){
            op = GREATER_OP;
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


Exp* Parser::parseUnaryFactor(){

    if(match(Token::ID)){
        string name = previous->text;
        if(match(Token::PLUSPLUS)){
            //cout<<"Name:"<<name<<endl;
            return new UnaryExp(name, UnaryOp::PLUSPLUS_OP);
        }
    }
    exit(0);
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
    cout<<current->text<<endl;
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}
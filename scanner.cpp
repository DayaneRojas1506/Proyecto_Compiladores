//
// Created by Jazmin on 4/11/2024.
//



#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;



Scanner::Scanner(const char* s):input(s),first(0), current(0) { }


bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    string word1;
    string ch;
    while (current < input.length() &&  is_white_space(input[current]) ) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c  = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }

    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first);
        if (word == "printf") {
            token = new Token(Token::PRINTF, word, 0, word.length());
        } else if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        } else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        } else if (word == "ifexp") {
            token = new Token(Token::IFEXP, word, 0, word.length());
        } else if (word == "while") {
            token = new Token(Token::WHILE, word, 0, word.length());
        }else if (word == "do") {
            token = new Token(Token::DO, word, 0, word.length());
        }else if (word == "for") {
            token = new Token(Token::FOR, word, 0, word.length());
        }else if(word == "int" or word == "char" or word == "string" or word == "float" or word == "long"){
            token = new Token(Token::TIPO, word, 0, word.length());
        }else if (word == "true") {
            token = new Token(Token::TRUE, word, 0, word.length());
        }
        else if (word == "false") {
            token = new Token(Token::FALSE, word, 0, word.length());
        }
        else if (word == "return") {
            token = new Token(Token::RETURN, word, 0, word.length());
        }
        else if (word == "fun") {
            //token = new Token(Token::FUN, word, 0, word.length());

        }else if(word == "include"){
            token= new Token(Token::INCLUDE, word, 0, word.length());
        }

        else {
            token = new Token(Token::ID, word, 0, word.length());

        }
    }

    else if (strchr("+-*/()=;,<#%{}.> \\\" ", c)) {
        string format_char;
        switch(c) {
            case '+':
                if (current + 1 < input.length() && input[current + 1] == '+') {
                    token = new Token(Token::PLUSPLUS, "++", 0, 2);
                    current++;

                } else {
                    token = new Token(Token::PLUS, c);
                }
                break;

            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/':
                if (current + 1 < input.length() && input[current + 1] =='/') {
                    //token = new Token(Token::COMENTARIO, "//", 0, 2);
                    current += 1;
                    int init = current;
                    while (current < input.length() && input[current] != '\n') {
                        current++; 
                    }
                    int fin = current;
                    token = new Token(Token::COMENTARIO, "comentario", 0, fin - init +1 );
                }else {
                    token = new Token(Token::DIV, c);
                }
                break;

            case ',': token = new Token(Token::COMA, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case ';': token = new Token(Token::PC, c); break;
            case '{': token= new Token(Token::LKEY,c); break;
            case '}': token = new Token(Token::RKEY,c); break;
            case '%':
                if (current + 1 < input.length() && (input[current+1] == 'd' || input[current+1] == 'f' || input[current+1] == 'l')) {
                    current++;
                    format_char = input[current];
                    if(input[current+1] == 'd'){
                        current++;
                        format_char += input[current];
                    }
                    //cout<<"A"<<endl;
                    //format_char = input[current];  // Guardamos el especificador
                }
                //cout<<input[current+1]<<endl;
                if (current+1 < input.length() && input[current+1] == '\\' && input[current+2] == 'n') {
                    //Falta tratar el error

                    token = new Token(Token::FORMAT, "%" + format_char + "\\"  , 0, format_char.length()+2 );
                    current+=2;
                } else {
                    token = new Token(Token::PERCENT, c);
                }
                break;
            case '.': token= new Token(Token::POINT,c); break;
            case '>': token = new Token(Token::GREATER,c); break;
            case '#':
                //current++; // Saltamos el '#'
                if (input.substr(current+1, 7) == "include"){
                    current += 8; // Saltamos "include"
                    if (current < input.length() && input[current] == '<') { // Verificamos '<'
                        current++;
                        std::string library;
                        while (current < input.length() && isalpha(input[current])) { // Construimos el nombre de la librería
                            library += input[current];
                            current++;
                        }
                        if (library == "stdio" && current < input.length() && input[current] == '.') {
                            current++; // Saltamos '.'
                            if (current < input.length() && input[current] == 'h') { // Verificamos 'h'
                                current++; // Saltamos 'h'
                                if (current < input.length() && input[current] == '>') { // Verificamos '>'
                                    current++; // Saltamos '>'

                                    token = new Token(Token::LIBRARY, "#include<studio.h>", 0, 18  );
                                    break;
                                }
                            }
                        }
                    }
                }
                // Si no es un include válido, no se genera ningún token especial
                token = new Token(Token::ERR, c);
                break;

            case '"': token = new Token(Token::COMILLA,c); break;
            default:
                cout << "No debería llegar acá" << endl;
                token = new Token(Token::ERR, c);
        }
        current++;
    }
    else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}
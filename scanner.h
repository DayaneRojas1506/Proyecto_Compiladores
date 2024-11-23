//
// Created by Jazmin on 4/11/2024.
//

#ifndef COMPILADORES_SCANNER_H
#define COMPILADORES_SCANNER_H


#include <string>
#include "token.h"


class Scanner {
private:
    std::string input;
    int first, current;
public:
    Scanner(const char* in_s);
    Token* nextToken();
    void reset();
    ~Scanner();
};

void test_scanner(Scanner* scanner);


#endif //COMPILADORES_SCANNER_H
